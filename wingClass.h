#pragma once
#include <iostream>
#include <string>
#include <QString>
#include <vector>

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Precision.hxx>
#include <stdexcept>
#include <BRepPrimAPI_MakePrism.hxx>
#include <gp_Vec.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Writer.hxx>


class wingClass
{
private:
    int readFiles();
    int readFromQString(const QString&);
    int chordLength;
    int spanLength;
    int updateVector(); // update the data vector by chordlength
    TopoDS_Wire Create2DProfile(const std::vector<std::pair<double, double>>& dataVector);
    TopoDS_Shape ExtrudeProfile(const TopoDS_Wire& profile, double l);
    
    void ExportToSTEP(TopoDS_Shape& shape, std::string filename);
    void ExportToIGES(TopoDS_Shape& shape, std::string filename);
    void ExportToBREP(TopoDS_Shape& shape, std::string filename);
    

public:
    wingClass(double chord, double span, QString textCoords);
    std::string wingName;                                   //stores wing name
    std::vector <std::pair<double, double>> dataVector;     //stores coordinates in 0-1 frame

    TopoDS_Shape extrudedShape;
    TopoDS_Shape getShape() const { return extrudedShape; }
    void ExportFile(std::string filename = "No filename received", int index = 0);
    
};

