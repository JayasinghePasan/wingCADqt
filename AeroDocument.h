#ifndef AERODOCUMENT_H
#define AERODOCUMENT_H

//Aero
//OCCT
#include <TopoDS.hxx>
#include <TDocStd_Document.hxx>

//QT
#include <QString>

class AeroView;

class AeroDocument
{
public:
	AeroDocument();
	~AeroDocument();

public:
	// Make a sphere with given radius
	// @param sphereShape - Return generated box shape
	// @param radius - Radius of the sphere
	// @return true if sphere generation is success
	bool makeSphere(TopoDS_Shape& sphereShape, double radius);

	// Make a box with given length, width, radius
	// @param boxShape - Return generated box shape
	// @param length - length of the box
	// @param width  - width of the box
	// @param height - height of the box
	// @return true if box generation is success
	bool makeDrillBox(TopoDS_Shape& boxShape, double length, double width, double height);

};

#endif AERODOCUMENT_H

