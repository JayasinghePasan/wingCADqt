#pragma once
#include <iostream>
#include <string>
#include <QString>
#include <vector>

#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <stdexcept>
#include <BRepPrimAPI_MakePrism.hxx>
#include <gp_Vec.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
//#include <IGESControl_Controller.hxx>
//#include <IGESControl_Writer.hxx>


class wingClass
{
private:
    int readFiles();
    int chordLength;
    int spanLength;
    int updateVector(); // update the data vector by chordlength
    TopoDS_Wire Create2DProfile(const std::vector<std::pair<double, double>>& dataVector);
    TopoDS_Shape ExtrudeProfile(const TopoDS_Wire& profile, double l);
    void ExportFile(std::string filename, int index, TopoDS_Shape& extrudedShape);
    void ExportToBREP(TopoDS_Shape& shape, std::string filename);
    

public:
    wingClass(std::string wingName, double chord, double span, int index, std::string filename);
    std::string wingName;                                   //stores wing name
    std::vector <std::pair<double, double>> dataVector;     //stores coordinates in 0-1 frame
    
};

