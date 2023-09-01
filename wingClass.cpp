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
wingClass::wingClass(std::string wingName, double chord, double span, int index, std::string filename) {
    this->wingName    = wingName;
    this->chordLength = chord;
    this->spanLength  = span;

    if (this->readFiles())
    {
        //############## QT Console ##################
        std::cout << "Wing Object " << this->wingName << " created successfully." << std::endl;
        //############################################
    }
    if (this->updateVector())
    {
        //############## QT Console ##################
        std::cout << "Wing Object " << this->wingName << " updated successfully." << std::endl;
        //############################################
    }

    TopoDS_Wire profile = Create2DProfile(dataVector);
    TopoDS_Shape extrudedShape = ExtrudeProfile( profile, spanLength);
    ExportFile(filename, index, extrudedShape);

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


//update the data vecor with chordlength
int wingClass::updateVector()
{
    if (dataVector.empty()) return 0;
    for (int i = 0 ; i < dataVector.size() ; i++)
    {
        //under develpment
    }
    return 1;
}


//create a 2d profile of the points at dataVector
TopoDS_Wire wingClass::Create2DProfile(const std::vector<std::pair<double, double>>&dataVector)
{
    BRepBuilderAPI_MakePolygon polygonMaker;

    for (const auto& pair : dataVector)
    {
        // Convert the 2D point (stored as a pair) to a 3D point (Z coordinate is 0)
        gp_Pnt p3d(pair.first, pair.second, 0.0);
        polygonMaker.Add(p3d);
    }

    // Close the polygon
    polygonMaker.Close();

    if (!polygonMaker.IsDone())
    {
        // Handle error: the polygon wasn't created correctly
        throw std::runtime_error("Failed to create the 2D profile.");
    }

    return polygonMaker.Wire();
}


//create a extrusion of a given 2d profile by length of l
TopoDS_Shape wingClass::ExtrudeProfile(const TopoDS_Wire& profile, double l)
{
    gp_Vec extrusionDirection(0, 0, l);  // Extrude in the Z direction
    BRepPrimAPI_MakePrism prismMaker(profile, extrusionDirection);
    return prismMaker.Shape();
}


void wingClass::ExportFile(std::string filename, int index, TopoDS_Shape& extrudedShape)
{
    if (index == 0) ExportToBREP(extrudedShape, filename);
    if (index == 1) ExportToBREP(extrudedShape, filename);
    if (index == 2) ExportToBREP(extrudedShape, filename);
}

void wingClass::ExportToBREP(TopoDS_Shape& shape, std::string filename)
{
    filename = filename + ".brep";
    BRepTools::Write(shape, filename.c_str());
}
