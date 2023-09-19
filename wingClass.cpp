#include "wingClass.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>
#include <windows.h>

#include "coolTools.h"



//Constructor

wingClass::wingClass(double chord = 1.0, double span = 1.0, QString textCoords = "") {
    //this->wingName    = wingName;
    this->chordLength = chord;
    this->spanLength = span;
    if (textCoords == "")
    {
        if (this->readFiles())
        {
            //############## QT Console ##################
            std::cout << "Wing Object " << this->wingName << " created successfully." << std::endl;
            //############################################
        }
    }
    else
    {
        this->readFromQString(textCoords);
    }
    if (this->updateVector())
    {
        //############## QT Console ##################
        std::cout << "Wing Object " << this->wingName << " updated successfully." << std::endl;
        //############################################
    }
    TopoDS_Wire profile = Create2DProfile(dataVector);
    extrudedShape = ExtrudeProfile(profile, spanLength);
}


//read the wing data file located at AeroFoilDat directory
//and add all the (x,y) coordinates to the vector<> dataVector
int wingClass::readFiles()
{
    std::string fileLocation = "tempAerofoilData.txt";
    std::ifstream file(fileLocation);

    if (!file.is_open())
    {
        //############## QT Console ##################
        std::cout << "Error opening the file at " << fileLocation << std::endl;
        //############################################
        return 0;
    }



    //Recheck the name at the 1st line of the txt file
    //return 0 and print error msg
    std::string line;
    getline(file, line);
    if (coolTools::trim(line) != this->wingName)
    {
        //############## QT Console ##################
        std::cout << "Text file name mismatch with the content." << std::endl;
        //############################################
        //return 0;
    }

    // Read and process the remaining lines
    while (getline(file, line))
    {
        std::istringstream iss(line);
        double x, y;
        if (iss >> x >> y)
        {
            this->dataVector.push_back({ x, y });
        }
    }
    file.close();
    return 1;
}


int wingClass::readFromQString(const QString& content)
{
    // Convert the QString to a stream for line-by-line processing
    std::istringstream stream(content.toStdString());

    // Recheck the name at the 1st line of the string content
    std::string line;
    getline(stream, line);
    if (coolTools::trim(line) != this->wingName)
    {
        //############## QT Console ##################
        std::cout << "String content name mismatch." << std::endl;
        //############################################
        //return 0;
    }

    // Read and process the remaining lines from the stream
    while (getline(stream, line))
    {
        std::istringstream iss(line);
        double x, y;
        if (iss >> x >> y)
        {
            this->dataVector.push_back({ x, y });
        }
    }

    return 1;
}



//update the data vecor with chordlength
int wingClass::updateVector()
{
    if (dataVector.empty()) return 0;
    for (int i = 0 ; i < dataVector.size() ; i++)
    {
        dataVector[i].first *= chordLength;
    }
    return 1;
}


//create a 2d profile of the points at dataVector
TopoDS_Wire wingClass::Create2DProfile(const std::vector<std::pair<double, double>>& dataVector)
{
    // 1. Transfer the points from dataVector to a format OpenCASCADE can work with
    TColgp_Array1OfPnt pointArray(1, static_cast<Standard_Integer>(dataVector.size()));

    Standard_Integer index = 1;
    for (const auto& pair : dataVector)
    {
        gp_Pnt p3d(pair.first, pair.second, 0.0);
        pointArray.SetValue(index, p3d);
        index++;
    }

    // 2. Create the B-spline curve interpolating the points
    Handle(Geom_BSplineCurve) bspline;
    Handle(TColgp_HArray1OfPnt) hArray = new TColgp_HArray1OfPnt(pointArray);
    GeomAPI_Interpolate interpolator(hArray, Standard_False, Precision::Confusion());
    interpolator.Perform();
    bspline = interpolator.Curve();

    // 3. Convert the B-spline curve to an edge and then a wire
    BRepBuilderAPI_MakeEdge edgeMaker(bspline);
    BRepBuilderAPI_MakeWire wireMaker;
    wireMaker.Add(edgeMaker.Edge());

    if (!wireMaker.IsDone())
    {
        // Handle error: the wire wasn't created correctly
        throw std::runtime_error("Failed to create the 2D B-spline profile.");
    }

    return wireMaker.Wire();
}



TopoDS_Shape wingClass::ExtrudeProfile(const TopoDS_Wire& profile, double l)
{
    // Create a face from the 2D profile
    BRepBuilderAPI_MakeFace faceMaker(profile);
    if (!faceMaker.IsDone()) {
        // Handle error
        throw std::runtime_error("Failed to create face from the 2D profile.");
    }
    TopoDS_Face face = faceMaker.Face();

    // Now, extrude the face to get a solid
    gp_Vec extrusionDirection(0, 0, l);  // Extrude in the Z direction
    BRepPrimAPI_MakePrism prismMaker(face, extrusionDirection);
    if (!prismMaker.IsDone()) {
        // Handle error
        throw std::runtime_error("Failed to extrude the face.");
    }
    return prismMaker.Shape();
}



//select the cad export format
void wingClass::ExportFile(std::string filename, int index)
{
    if (index == 0) ExportToSTEP(this->extrudedShape, filename);
    if (index == 1) ExportToIGES(this->extrudedShape, filename);
    if (index == 2) ExportToBREP(this->extrudedShape, filename);
}


//create .brep file under given file name
void wingClass::ExportToBREP(TopoDS_Shape& shape, std::string filename)
{
    filename = "CAD_Exports/" + filename + ".brep";
    BRepTools::Write(shape, filename.c_str());
}


//create .iges file under given file name
void wingClass::ExportToIGES(TopoDS_Shape& shape, std::string filename)
{
    filename = "CAD_Exports/" + filename + ".iges";
    IGESControl_Controller::Init();
    IGESControl_Writer writer("MM", 0);
    writer.AddShape(shape);
    writer.ComputeModel();
    writer.Write(filename.c_str());
}


//create .step file under given file name
void wingClass::ExportToSTEP(TopoDS_Shape& shape, std::string filename)
{
    filename = "CAD_Exports/" + filename + ".step";
    STEPControl_Writer writer;
    IFSelect_ReturnStatus status = writer.Transfer(shape, STEPControl_AsIs);
    status = writer.Write(filename.c_str());
}
