#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <stdexcept>
#include <BRepPrimAPI_MakePrism.hxx>
#include <gp_Vec.hxx>
#include <TopoDS_Shape.hxx>

class wingClass
{
private:
    int readFiles();
    int chordLength;
    int spanLength;
    int updateVector(); // update the data vector by chordlength
    TopoDS_Wire Create2DProfile(const std::vector<std::pair<double, double>>& dataVector);
    TopoDS_Shape ExtrudeProfile(const TopoDS_Wire& profile, double l);

public:
    wingClass(std::string wingName, double chord, double span);

    std::string wingName;                                   //stores wing name
    std::vector <std::pair<double, double>> dataVector;     //stores coordinates in 0-1 frame


};

