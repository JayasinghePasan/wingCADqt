#ifndef AEROCOOLTOOLS_H
#define AEROCOOLTOOLS_H

//Aero
//OCCT
#include <Standard_ErrorHandler.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>

#include <gp_Pnt.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Surface.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepTools.hxx>
#include <BRep_Tool.hxx>

#include <TopExp_Explorer.hxx>

#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepExtrema_ExtPC.hxx>
#include <BRepExtrema_ExtPF.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BRepExtrema_ExtCF.hxx>
#include <BRepExtrema_ExtFF.hxx>

#include <GeomAPI_Interpolate.hxx>

#include <GeomAPI_IntCS.hxx>
#include <GeomAPI_IntSS.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>

#include <BRepPrimAPI_MakeRevol.hxx>

#include <TopTools_ListOfShape.hxx>
#include <TColStd_Array1OfReal.hxx>

//QT
#include <QTextStream>
#include <QColor>
#include <QVector>

// Convert a 3D point in the format (X, Y, Z) to 2D point in the format (0, R, Z)
// Here R = sqrt(X*X +Y*Y)
// @param point - 3D point to be converted to RZ point
// @return the converted point
#define RZPOINT(point) (gp_Pnt(0.0, sqrt(point.X()*point.X() + point.Y()*point.Y()), point.Z()))

// Convert Degree value to Radian value
// @param deg - Degree value to be converted to Radian value
// @return the converted Radian value
#define RADIAN(deg) ((deg) * PI / 180.0)

// Convert Radian value to Degree value
// @param rad - Radian value to be converted to Degree value
// @return the converted Degree value
#define DEGREE(rad) ((rad) * 180.0 / PI)

enum Coordinate
{
	NoCoord = 0,
	X,
	Y,
	Z
};

class TGenCoolTools
{
public:
	// Get Quantity_Color(OCCT) from QColor(QT)
	static Quantity_Color toQuantityColor(const QColor& color);

	// Get QColor(QT) from Quantity_Color(OCCT)
	static QColor toQColor(const Quantity_Color& color);

	// Get AIS_Shape from AIS_InteractiveObject
	static Handle(AIS_Shape) toAISShape(Handle(AIS_InteractiveObject) aisInteractiveObject);

	// Generate equal distance points from the curve
	// Distance measured along the curve
	// @param curve - Curve to get points
	// @param numOfPnts - Number of points to generate, if numOfPnts < 2, then generates number points equal to 4 times of curve length
	// @return vector of generated points(gp_Pnt). 
	static std::vector<gp_Pnt> generatePointsFromCurve(const Handle(Geom_Curve)& curve, int numOfPnts = 0);

	// Generate equal distance points from the edge
	// Distance measured along the edge
	// @param edge - Edge to get points
	// @param numOfPnts - Number of points to generate, if numOfPnts < 2, then generates number points equal to 4 times of curve length
	// @return vector of generated points(gp_Pnt). 
	static std::vector<gp_Pnt> generatePointsFromEdge(TopoDS_Edge edge, int numOfPnts = 0);

	// Get length of edge
	// @param edge to calculate length
	// @return length of the edge
	static double getEdgeLength(TopoDS_Edge edge);

	// Get length of curve
	// @param curve to calculate length
	// @return length of the curve
	static double getCurveLength(Handle(Geom_Curve) curve);

	// Get length of curve between two parameters
	// @param curve to calculate length
	// @param startParam - Start point parameter to calculate length
	// @param endParam - End point parameter to calculate length (startParam < endParam)
	// @return length of the curve
	static double getCurveLength(Handle(Geom_Curve) curve, double startParam, double endParam);

	// Get length of curve between two points
	// @param curve to calculate length
	// @param startPoin - Start point to calculate length
	// @param endParam - End point to calculate length 
	// @return length of the curve
	static double getCurveLength(Handle(Geom_Curve) curve, gp_Pnt startPoint, gp_Pnt endPoint);

	// Extend curve by length percentage
	// @param curve - Curve to extend
	// @param lengthFraction - Fraction (0 to 1) of the current curve length to extend
	// @param atEnd - true : Extend the curve at end , false : Extend the curve at start 
	// @return extended curve.
	static Handle(Geom_Curve) extendCurveByLengthFraction(Handle(Geom_Curve) curve, double lengthFraction, bool atEnd);

	// Get the parameter in the curve, where the point located in the curve.
	// @param parameter - Return the parameter of point in the curve
	// @param curve - Curve to find parameter
	// @param point - Point to find parameter from curve
	// @param tolerance - Maximum difference between the point and point in the curve
	// @return false if failed to find parameter with tolerance.
	static bool getCurveParameter(double& parameter, Handle(Geom_Curve) curve, gp_Pnt pnt, double tolerance = 1e-7);

	// Get the parameter in the curve, where curve segment from first parameter to that parameter gives the length.
	// @param parameter - Return the parameter of point in the curve in the given  curve length
	// @param curve - Curve to find parameter
	// @param length - length to find parameter from curve
	// @param tolerance - Maximum difference between the length input and resultant length
	// @return false if failed to find parameter with tolerance.
	static bool getCurveParameter(double& parameter, Handle(Geom_Curve) curve, double length, double tolerance = 1e-7);

	// Find the parameter in the curve, where coordinate value matching
	// @param parameter - Curve parameter of the point which has the input coordinate value
	// @param curve - Curve to find parameter
	// @param coordinateAxis - Coordinate axis (coordinateAxis = 1 -> match X value, coordinateAxis = 2 -> match Y value, coordinateAxis = 3 -> match Z value and coordinateAxis = 4 -> match Radius
	// @param value - Value of the coordiante to find
	// @param tolerance - Maximum difference between the value input and find value.
	// @return true if successfully find the coordinate in the curve
	static bool getCurveParameterOfCoordinateValue(double& parameter, Handle(Geom_Curve) curve, int coordinatAxis, double value, double tolerance = 1.0e-6);

	// Find the point with the coordinate value in the coordinate axis from the curve
	// Create a surface with coordinateAxis direction and coordinateValue point and get intersection with the curve to get the point.
	// @param point - Return the point with the coordinat  
	// @param curve : Curve to serach for the coordinateValue
	// @param coordinateAxis :  1 -> match X value, 2 -> match Y value, 3 -> match Z value
	// @param coordinateValue : Value to search.
	// @return true if successfully find the point
	static bool getCurvePointOfCoordinateValue(gp_Pnt& point, Handle(Geom_Curve) curve, int coordinateAxis, double coordinateValue);

	// Get the parameters in the surface, where the point located in the surface.
	// @param uParameter - Return the U parameter of point in the surface
	// @param vParameter - Return the V parameter of point in the surface
	// @param surface - Surface to find parameter
	// @param point - Point to find parameter from surface
	// @param tolerance - Maximum difference between the point and point in the surface
	// @return false if failed to find parameter with tolerance.
	static bool getSurfaceParameter(double& uParameter, double& vParameter, Handle(Geom_Surface) surface, gp_Pnt point, double tolerance);

	// Find the parameters of closest points between two curves
	// @param curve1 - First curve
	// @param curve2 - Second curve
	// @param parameterInCurve1 - Parameter of the point in the first curve
	// @param parameterInCurve2 - Parameter of the point in the second curve
	// @retrun true if successfully find the closest point
	static bool findClosestPoint(Handle(Geom_Curve) curve1, Handle(Geom_Curve) curve2, double& parameterInCurve1, double& parameterInCurve2);

	// Create face from the wire using base surface
	// @param baseSurface - Support surface to create face
	// @param wire - Wire to create face
	// @return generated face
	static TopoDS_Face makeFaceFromWire(Handle(Geom_Surface) baseSurface, TopoDS_Wire wire);

	// Scale the shape by given factor from the origin(gp_Pnt(0.0,0.0,0.0))
	// @param shapeToScale - TopoDS_Shape to scale
	// @param scaleFactor - Factor to scale
	// @return scaled shape
	static TopoDS_Shape scaleShape(TopoDS_Shape& shapeToScale, double scaleFactor);

	// Get the boolean cut of given list of shapes and list of cutting tools
	// @param listOfShapes - Shapes to be perform boolean cut
	// @param listOfTools - Shapes of cutting tools
	// @param resultShape - Return resultant shape of the boolean cut
	// @return true if boolean cut operation is successful
	static bool booleanCutTwoShapes(TopTools_ListOfShape listOfShapes, TopTools_ListOfShape listOfTools, TopoDS_Shape& cutShape);

	// Rotate the face around Z Axis (OZ) by given angle
	// @param face - TopoDS_Face to rotate
	// @param angle - Angle (in radians) to rotate.
	// @return the rotated face
	static TopoDS_Face rotateFaceAroundOZ(TopoDS_Face face, double angle);

	// Create 2D fillet between two edges with given radius.
	// @param filletEdge - Return the generated fillet
	// @param edge1 - First edge as input and return the modified edge after filleting
	// @param edge2 - Second edge as input and return the modified edge after filleting
	// @param commonPoint - The point where the fillet should be generate and it shoud be common to both edge1 and edge2
	// @param radius - Radius of the fillet
	// @param plane - The plane where the both edge1 and edge2 lay
	// @return true if filleting is successful 
	static bool make2DFillet(TopoDS_Edge& filletEdge, TopoDS_Edge& edge1, TopoDS_Edge& edge2, gp_Pnt commonPoint, Standard_Real radius, gp_Ax3 plane);

	// Find the common edge between face1 and face2
 // @param commonEdge - Return the edge common to both face1 and face2
 // @param face1 - First face
 // @param face2 - Second face
 // @return true if a coomon edge found
	static bool findCommonEdge(TopoDS_Edge& commonEdge, TopoDS_Face face1, TopoDS_Face face2);

	// Get the parameter of a point in referenceCurve which is normal to the main curve in given parameter of mainCurve.
	// @param mainCurve - Curve to create normal from
	// @param parameterInMainCurve - Location of the main curve normal has to be created
	// @param referenceCurve - Curve to find the normal point
	// @param parameterInReferenceCurve - Return the parameter of normal point if exist in reference curve.
	// @return true if successfully found the normal point parameter in reference curve.
	static bool getParameterOfNormalPointToCurve(Handle(Geom_Curve) mainCurve, double parameterInMainCurve, Handle(Geom_Curve) referenceCurve, double& parameterInReferenceCurve);

	// Get the point in referenceCurve which is normal to the main curve in given parameter of mainCurve.
	// @param mainCurve - Curve to create normal from
	// @param parameterInMainCurve - Location of the main curve normal has to be created
	// @param referenceCurve - Curve to find the normal point
	// @param pointInReferenceCurve - Return the normal point if exist in reference curve.
	// @return true if successfully found the normal point in reference curve.
	static bool getNormalPointToCurve(Handle(Geom_Curve) mainCurve, double parameterInMainCurve, Handle(Geom_Curve) referenceCurve, gp_Pnt& pointInReferenceCurve);

	// Get the point in referenceCurve which is normal to the main curve in given point of mainCurve.
	// @param mainCurve - Curve to create normal from
	// @param pointInMainCurve - Location of the main curve normal has to be created
	// @param referenceCurve - Curve to find the normal point
	// @param pointInReferenceCurve - Return the normal point if exist in reference curve.
	// @return true if successfully found the normal point in reference curve.
	static bool getNormalPointToCurve(Handle(Geom_Curve) mainCurve, gp_Pnt pointInMainCurve, Handle(Geom_Curve) referenceCurve, gp_Pnt& pointInReferenceCurve);

	// Split a given face into two  
	// @param splittedFaces - vector of splitted faces 
	// @param originalFace - original face before splitting 
	// @param splittingFace - splittingFace face use to split the original face
	static bool splitFace(QVector<TopoDS_Face>& splittedFaces, TopoDS_Face originalFace, gp_Pln myPlane);

	// Get the plane of a 2d face
	// @param face - 2d face
	// @param plane - corresponding plane of the 2d face
	static bool getPlaneof2DFace(TopoDS_Face face, gp_Pln& plane);

	// Generate a straight BSpline curve going throug given end points
	// @param bSplineCurve - return generated BSpline curve
	// @param startPoint - Start point of the curve
	// @param endPoint - End point of the curve
	// return true if interpolation is successful
	static bool makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& geomBSplineCurve, gp_Pnt startPoint, gp_Pnt endPoint);

	// Generate a BSpline curve going throug given points using interpolation
	// @param bSplineCurve - return generated BSpline curve
	// @param pointsForCurve - Points to interpolate
	// @param tolerance - Tolerance for interpolation which is the minimum distance between two consecutive points
	// return true if interpolation is successful
	static bool makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& bSplineCurve, std::vector<gp_Pnt> pointsForCurve, double tolerance = 1.0e-15);

	// Generate BSpline curve going throug the given points using interpolation
	// @param bSplineCurve - return generated BSpline curve
	// @param pointsForCurve - Points to interpolate
	// @param tolerance - Tolerance for interpolation which is the minimum distance between two consecutive points
	// return true if interpolation is successful
	static bool makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& bSplineCurve, QVector<gp_Pnt> pointsForCurve, double tolerance = 1.0e-15);

	// Extend surface by length percentage
	// @param surface - surface to extend
	// @param lengthFraction - Fraction (0 to 1) of the current curve length to extend
	// @param atEnd - true : Extend the curve at end , false : Extend the curve at start 
	// @param atU - true : Extend the curve at U direction , false : Extend the curve at V direction 
	// @return extended surface.
	static Handle(Geom_Surface) extendSurfaceByLengthFraction(Handle(Geom_Surface) surface, double lengthFraction, bool atU, bool atEnd);

	// Segment the bspline curve between two parameters
	// @param segmentedCurve - Return the segmented curve
	// @param curveToBeSegmented - Input curve to be segmented
	// @param paramFrom - Start parameter of the curve segment in curve to be segmeted -> paramFrom < paramTo
	// @param paramTo - End parameter of the curve segment in curve to be segmeted -> paramFrom < paramTo
	// @return true if the segmentation is successful
	static bool segmentGeomBSplineCurve(Handle(Geom_BSplineCurve)& segmentedCurve, const Handle(Geom_BSplineCurve)& curveToBeSegmented, double paramFrom, double paramTo);

	// Convert non bspline curve to bspline curve
 // @param geomCurve - Curve to be converted as BSpline curve
 // @param bspCurve - Returns converted BSpline curve
 // @return true if curve is successfully converted to BSpline curve
	static bool convertToGeomBSplineCurve(const Handle(Geom_Curve)& geomCurve, Handle(Geom_BSplineCurve)& bspCurve);

	// Compare the orientation of curve with orientation of rzCurve, if the orientations do not match, reverse the orientation of curve.
	// @param curve - Curve to check the orientation, and return the oriented curve. This curve should be in rzCurve revolved surface
	// @param rzCurve - RZ Curve (2D curve -> (0.0, R, Z) )
	// @return true if orientation comparision is done.
	static bool reverseCurveBasedOnRZCurve(Handle_Geom_Curve& curve, const Handle_Geom_Curve& rzCurve);

	// Extend the mean line curve to cover the stream line (flow path meridional curve) along stream line revolved surface
	// @param meanLineCurve - Mean line curve to be extended and return the extended meanline curve
	// @param streamLineCurve - Stream line (Flow path meridional) curve of the mean line curve
	// @return true if successfully extend mean line curve
	static bool extendMeanLineCurveAlongStreamLineSurface(Handle_Geom_Curve& meanLineCurve, const Handle_Geom_Curve& streamLineCurve);

	// Create 2D RZCurve from a 3D curve
	// Points in the generated 2D curve have gp_Pnt(0.0,R,Z) format. 
	// Here conversion happens from gp_Pnt(X,Y,Z) to gp_Pnt(0.0,R,Z) --> R = sqrt(X*X +Y*Y)
	// @param rzCurve - Returns the generated curve
	// @param curve - 3D curve to be converted to RZ curve
	// @return true if rzCurve created successfully
	static bool createRZCurve(Handle(Geom_Curve)& rzCurve, const Handle(Geom_Curve)& curve);

	// Calculate theta values and related curve(U) parameters along the curve
	// Here theta values are in radian and continuous. Not limited to 0 t0 2*PI
	// @param thetaValues - Returns the calculated theta values
	// @param uValues - Returns the curve parameters to of the theta values calculated points
	// @param curve - Curve to be calculate the theta values
	// @param numOfValuesNeeded - Number of theta values need to be calculated
	// @return true if theta calculation is success
	static bool calculateThetaValuesOfCurve(std::vector<double>& thetaValues, std::vector<double>& uValues, const Handle_Geom_Curve& curve, int numOfValuesNeeded);

	// Calculate continuous theta values from 0 to 2*PI limited theta values
	// @param thetaValues - 0 to 2*PI limited theta values of continuous points
	// @return calculated continuous theta values of limited theta values
	static std::vector<double> calculateContinuousThetaValues(const std::vector<double>& thetaValues);
};

#endif // AEROCOOLTOOLS_H
