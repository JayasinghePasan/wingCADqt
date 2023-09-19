#include "AeroDocument.h"

//Aero
//OCCT
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

//QT

AeroDocument::AeroDocument()
{
}

AeroDocument::~AeroDocument()
{
}

// Make a sphere with given radius
// @param sphereShape - Return generated box shape
// @param radius - Radius of the sphere
// @return true if sphere generation is success
bool AeroDocument::makeSphere(TopoDS_Shape& sphereShape, double radius)
{
    if (radius <= 0.0)
        return false;

    try
    {
        OCC_CATCH_SIGNALS
            sphereShape = BRepPrimAPI_MakeSphere(radius).Shape();
    }
    catch (Standard_Failure)
    {
        return false;
    }

    return true;
}

bool makeDrillBox(TopoDS_Shape& drillBoxShape, double length, double width, double height)
{
    if (length <= 0.0 || height <= 0.0 || width <= 0.0)
    {
        return false;
    }
    try
    {
        OCC_CATCH_SIGNALS
            drillBoxShape = BRepPrimAPI_MakeBox(length, width, height).Shape();
    }
    catch (Standard_Failure)
    {
        return false;
    }
}
