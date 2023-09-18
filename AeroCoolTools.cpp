#include "AeroCoolTools.h"

//Aero
//OCCT
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <GCPnts_QuasiUniformAbscissa.hxx>
#include <GeomLib_Tool.hxx>
#include <BRepOffset_Tool.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <ChFi2d_FilletAPI.hxx>
#include <gce_MakePln.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <GeomLProp_CLProps.hxx>
#include <Geom_BoundedSurface.hxx>
#include <AIS_TextLabel.hxx>
#include <BRepAlgoAPI_Check.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomLib.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <Geom_Line.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepFeat_SplitShape.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_OBB.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

//QT
#include <QColor>

// Get Quantity_Color(OCCT) from QColor(QT)
Quantity_Color TGenCoolTools::toQuantityColor(const QColor& color)
{
    return Quantity_Color(color.red() / 255., color.green() / 255., color.blue() / 255., Quantity_TOC_RGB);
}

// Get QColor(QT) from Quantity_Color(OCCT)
QColor TGenCoolTools::toQColor(const Quantity_Color& color)
{
    return QColor((int)color.Red() * 255., (int)color.Green() * 255., (int)color.Blue() * 255.);
}

// Get AIS_Shape from AIS_InteractiveObject
Handle(AIS_Shape) TGenCoolTools::toAISShape(Handle(AIS_InteractiveObject) aisInteractiveObject)
{
    return Handle(AIS_Shape)::DownCast(aisInteractiveObject);
}

// Generate equal distance points from the curve
// Distance measured along the curve
// @param curve - Curve to get points
// @param numOfPnts - Number of points to generate, if numOfPnts < 2, then generates number points equal to 4 times of curve length
// @return vector of generated points(gp_Pnt). 
std::vector<gp_Pnt> TGenCoolTools::generatePointsFromCurve(const Handle(Geom_Curve)& curve, int numOfPnts)
{
    std::vector<gp_Pnt> pntVector;
    if (!curve.IsNull())
    {
        if (numOfPnts < 2)
        {
            double curveLen = getCurveLength(curve);
            numOfPnts = ceil(curveLen) * 4;
        }

        BRepAdaptor_Curve adaptor(BRepBuilderAPI_MakeEdge(curve).Edge());
        GCPnts_QuasiUniformAbscissa uniformAbs(adaptor, numOfPnts);
        if (uniformAbs.IsDone())
        {
            int numPoints = uniformAbs.NbPoints();
            for (int i = 1; i <= numPoints; i++)
            {
                pntVector.push_back(curve->Value(uniformAbs.Parameter(i)));
            }
        }
    }
    return pntVector;
}

// Generate equal distance points from the edge
// Distance measured along the edge
// @param edge - Edge to get points
// @param numOfPnts - Number of points to generate, if numOfPnts < 2, then generates number points equal to 4 times of edge length
// @return vector of generated points(gp_Pnt). 
std::vector<gp_Pnt> TGenCoolTools::generatePointsFromEdge(TopoDS_Edge edge, int numOfPnts)
{
    std::vector<gp_Pnt> pntVector;
    if (!edge.IsNull())
    {
        if (numOfPnts < 2)
        {
            double edgeLen = TGenCoolTools::getEdgeLength(edge);
            numOfPnts = ceil(edgeLen) * 4;
        }

        double fParam;
        double lParam;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, fParam, lParam);
        if (!curve.IsNull())
        {
            BRepAdaptor_Curve adaptor(edge);
            GCPnts_QuasiUniformAbscissa uniformAbs(adaptor, numOfPnts, fParam, lParam);
            if (uniformAbs.IsDone())
            {
                int numPoints = uniformAbs.NbPoints();
                for (int i = 1; i <= numPoints; i++)
                    pntVector.push_back(curve->Value(uniformAbs.Parameter(i)));
            }
        }
    }

    return pntVector;
}

// Get length of edge
// @param edge to calculate length
// @return length of the edge
double TGenCoolTools::getEdgeLength(TopoDS_Edge edge)
{
    if (edge.IsNull())
        return 0.0;

    GProp_GProps lProps;
    BRepGProp::LinearProperties(edge, lProps);
    return lProps.Mass();
}

// Get length of curve
// @param curve to calculate length
// @return length of the curve
double TGenCoolTools::getCurveLength(Handle(Geom_Curve) curve)
{
    if (curve.IsNull())
        return 0.0;

    return getEdgeLength(BRepBuilderAPI_MakeEdge(curve).Edge());
}

// Get length of curve between two parameters
// @param curve to calculate length
// @param startParam - Start point parameter to calculate length
// @param endParam - End point parameter to calculate length (startParam < endParam)
// @return length of the curve
double TGenCoolTools::getCurveLength(Handle(Geom_Curve) curve, double startParam, double endParam)
{
    if (curve.IsNull())
        return 0.0;

    double fParam = curve->FirstParameter();
    double lParam = curve->LastParameter();
    double length;

    if (startParam == endParam)
        length = 0.0;
    else if (startParam<fParam || startParam>lParam || endParam <fParam || endParam>lParam)
        length = 0.0;
    else
    {
        BRepAdaptor_Curve adaptorCurve(BRepBuilderAPI_MakeEdge(curve).Edge());

        if (startParam < endParam)
            length = GCPnts_AbscissaPoint::Length(adaptorCurve, startParam, endParam);
        else
            length = GCPnts_AbscissaPoint::Length(adaptorCurve, endParam, startParam);
    }

    return length;
}

// Get length of curve between two points which are in the curve.
// @param curve to calculate length
// @param startPoin - Start point to calculate length
// @param endParam - End point to calculate length 
// @return length of the curve
double TGenCoolTools::getCurveLength(Handle(Geom_Curve) curve, gp_Pnt startPoint, gp_Pnt endPoint)
{
    if (curve.IsNull())
        return 0.0;

    //Get parameter of the start point in the curve
    double startParam;
    if (getCurveParameter(startParam, curve, startPoint))
    {
        //Get parameter of the end point in the curve
        double endParam;
        if (getCurveParameter(endParam, curve, endPoint))
        {
            return getCurveLength(curve, startParam, endParam);
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

// Extend curve by length percentage
// @param curve - Curve to extend
// @param lengthFraction - Fraction (0 to 1) of the current curve length to extend
// @param atEnd - true : Extend the curve at end , false : Extend the curve at start 
// @return extended curve.
Handle(Geom_Curve) TGenCoolTools::extendCurveByLengthFraction(Handle(Geom_Curve) curve, double lengthFraction, bool atEnd)
{
    if (curve.IsNull())
        return curve;

    gp_Pnt firstPoint, lastPoint;
    gp_Vec firstVec, lastVec;

    curve->D1(curve->FirstParameter(), firstPoint, firstVec);
    curve->D1(curve->LastParameter(), lastPoint, lastVec);

    //get the curve length
    double curveLength = getCurveLength(curve);
    double lengthToExtend = lengthFraction * curveLength; //distance to extend

    Handle(Geom_BoundedCurve) boundedCurve = Handle(Geom_BoundedCurve)::DownCast(curve->Copy());
    if (boundedCurve.IsNull())
        return curve;

    if (atEnd) //extend the curve at the end
    {
        lastVec.Normalize();
        lastVec = lastVec * lengthToExtend;
        lastPoint.Translate(lastVec);
        GeomLib::ExtendCurveToPoint(boundedCurve, lastPoint, 2, Standard_True);
    }
    else // extend the curve at the start
    {
        firstVec.Normalize();
        firstVec = firstVec * lengthToExtend * (-1);
        firstPoint.Translate(firstVec);
        GeomLib::ExtendCurveToPoint(boundedCurve, firstPoint, 2, Standard_False);
    }

    curve = boundedCurve;
    return curve;
}

// Get the parameter in the curve, where the point located in the curve.
// @param parameter - Return the parameter of point in the curve
// @param curve - Curve to find parameter
// @param point - Point to find parameter from curve
// @param tolerance - Maximum difference between the point and point in the curve
// @return false if failed to find parameter with tolerance.
bool TGenCoolTools::getCurveParameter(double& parameter, Handle(Geom_Curve) curve, gp_Pnt point, double tolerance)
{
    if (curve.IsNull())
        return false;

    if (!GeomLib_Tool::Parameter(curve, point, tolerance, parameter))
    {
        double firstParam = curve->FirstParameter();
        double lastParam = curve->LastParameter();
        double tempParam = firstParam - (lastParam - firstParam);

        double distance = DBL_MAX;
        BRepExtrema_DistShapeShape dss(BRepBuilderAPI_MakeVertex(point).Vertex(), BRepBuilderAPI_MakeEdge(curve).Edge());
        if (dss.IsDone() && dss.NbSolution() > 0)
        {
            distance = dss.Value();
        }

        double minDist = DBL_MAX;
        BRepExtrema_ExtPC epc(BRepBuilderAPI_MakeVertex(point).Vertex(), BRepBuilderAPI_MakeEdge(curve).Edge());
        if (epc.IsDone() && epc.NbExt() > 0)
        {
            for (int i = 1; i <= epc.NbExt(); i++)
            {
                double dist = sqrt(epc.SquareDistance(i));
                if (dist < minDist)
                {
                    minDist = dist;
                    tempParam = epc.Parameter(i);
                }
            }
        }

        if (tempParam < firstParam || tempParam > lastParam || distance * 1.2 < minDist)
        {
            gp_Pnt curveFPnt = curve->Value(firstParam);
            gp_Pnt curveLPnt = curve->Value(lastParam);
            double d1 = curveFPnt.Distance(point);
            double d2 = curveLPnt.Distance(point);

            double minDist = std::min(d1, d2);
            if (minDist < TGenCoolTools::getCurveLength(curve))
            {
                if (minDist == d1)
                    parameter = firstParam;
                else
                    parameter = lastParam;
            }
            else
                return false;
        }
        else
            parameter = tempParam;
    }

    return true;
}

// Get the parameter in the curve, where curve segment from first parameter to that parameter gives the length.
// @param parameter - Return the parameter of point in the curve in the given  curve length
// @param curve - Curve to find parameter
// @param length - length to find parameter from curve
// @param tolerance - Maximum difference between the length input and resultant length
// @return false if failed to find parameter with tolerance.
bool TGenCoolTools::getCurveParameter(double& parameter, Handle(Geom_Curve) curve, double length, double tolerance)
{
    if (curve.IsNull())
        return false;

    double curveLength = TGenCoolTools::getCurveLength(curve);

    if (length < 0.0 || length > curveLength)
        return false;

    double fParam = curve->FirstParameter();
    double lParam = curve->LastParameter();

    if (length == 0.0)
        parameter = fParam;
    else if (length == curveLength)
        parameter = lParam;
    else
    {
        double startParam = fParam;
        double endParam = lParam;
        double midParam = fParam;
        double lengrthCalc = 0.0;

        while (fabs(length - lengrthCalc) > tolerance)
        {
            midParam = (startParam + endParam) / 2.0;

            Handle(Geom_TrimmedCurve) trimCurve = new Geom_TrimmedCurve(curve, fParam, midParam);
            lengrthCalc = TGenCoolTools::getCurveLength(trimCurve);

            if (length < lengrthCalc)
                endParam = midParam;
            else
                startParam = midParam;
        }

        parameter = midParam;
    }

    return true;
}

// Find the parameter in the curve, where coordinate value matching
// @param parameter - Curve parameter of the point which has the input coordinate value
// @param curve - Curve to find parameter
// @param coordinateAxis - Coordinate axis (coordinateAxis = 1 -> match X value, coordinateAxis = 2 -> match Y value, coordinateAxis = 3 -> match Z value and coordinateAxis = 4 -> match Radius
// @param value - Value of the coordiante to find
// @param tolerance - Maximum difference between the value input and find value.
// @return true if successfully find the coordinate in the curve
bool TGenCoolTools::getCurveParameterOfCoordinateValue(double& parameter, Handle(Geom_Curve) curve, int coordinateAxis, double value, double tolerance)
{
    if (curve.IsNull())
        return false;

    //if coordinateAxis = 1 -> match X value
    //if coordinateAxis = 2 -> match Y value
    //if coordinateAxis = 3 -> match Z value
    //if coordinateAxis = 4 -> match Radius

    double fParam = curve->FirstParameter();
    double lParam = curve->LastParameter();

    gp_Pnt fPoint = curve->Value(fParam);
    gp_Pnt lPoint = curve->Value(lParam);

    double paramTolerance = 1.0 / fabs(lParam - fParam) * tolerance * 1.0e-3;

    bool isFound = false;
    if (coordinateAxis == 1)
    {
        if (fabs(fPoint.X() - value) < tolerance)
        {
            parameter = fParam;
            isFound = true;
        }
        else if (fabs(lPoint.X() - value) < tolerance)
        {
            parameter = lParam;
            isFound = true;
        }
        else
        {
            bool condition1 = (fPoint.X() < value && value < lPoint.X());
            bool condition2 = (lPoint.X() < value && value < fPoint.X());

            if (condition1 || condition2)
            {
                double midParam;
                double paramDiff = fabs(lParam - fParam);
                while (paramDiff > paramTolerance)
                {
                    midParam = (fParam + lParam) / 2.0;
                    gp_Pnt midPoint = curve->Value(midParam);

                    if (fabs(midPoint.X() - value) < tolerance)
                    {
                        isFound = true;
                        parameter = midParam;
                        break;
                    }

                    if (condition1)
                    {
                        if (fPoint.X() < value && value < midPoint.X())
                            lParam = midParam;
                        else //((midPoint.X() < value && value < lPoint.X())
                            fParam = midParam;
                    }
                    else
                    {
                        if (lPoint.X() < value && value < midPoint.X())
                            fParam = midParam;
                        else //((midPoint.X() < value && value < fPoint.X())
                            lParam = midParam;
                    }

                    paramDiff = fabs(lParam - fParam);
                }
            }
        }
    }
    else if (coordinateAxis == 2)
    {
        if (fabs(fPoint.Y() - value) < tolerance)
        {
            parameter = fParam;
            isFound = true;
        }
        else if (fabs(lPoint.Y() - value) < tolerance)
        {
            parameter = lParam;
            isFound = true;
        }
        else
        {
            bool condition1 = (fPoint.Y() < value && value < lPoint.Y());
            bool condition2 = (lPoint.Y() < value && value < fPoint.Y());

            if (condition1 || condition2)
            {
                double midParam;
                double paramDiff = fabs(lParam - fParam);
                while (paramDiff > paramTolerance)
                {
                    midParam = (fParam + lParam) / 2.0;
                    gp_Pnt midPoint = curve->Value(midParam);

                    if (fabs(midPoint.Y() - value) < tolerance)
                    {
                        isFound = true;
                        parameter = midParam;
                        break;
                    }

                    if (condition1)
                    {
                        if (fPoint.Y() < value && value < midPoint.Y())
                            lParam = midParam;
                        else //((midPoint.Y() < value && value < lPoint.Y())
                            fParam = midParam;
                    }
                    else
                    {
                        if (lPoint.Y() < value && value < midPoint.Y())
                            fParam = midParam;
                        else //((midPoint.Y() < value && value < fPoint.Y())
                            lParam = midParam;
                    }

                    paramDiff = fabs(lParam - fParam);
                }
            }
        }
    }
    else if (coordinateAxis == 3)
    {
        if (fabs(fPoint.Z() - value) < tolerance)
        {
            parameter = fParam;
            isFound = true;
        }
        else if (fabs(lPoint.Z() - value) < tolerance)
        {
            parameter = lParam;
            isFound = true;
        }
        else
        {
            bool condition1 = (fPoint.Z() < value && value < lPoint.Z());
            bool condition2 = (lPoint.Z() < value && value < fPoint.Z());

            if (condition1 || condition2)
            {
                double midParam;
                double paramDiff = (lParam - fParam);
                while (paramDiff > paramTolerance)
                {
                    midParam = (fParam + lParam) / 2.0;
                    gp_Pnt midPoint = curve->Value(midParam);

                    if (fabs(midPoint.Z() - value) < tolerance)
                    {
                        isFound = true;
                        parameter = midParam;
                        break;
                    }

                    if (condition1)
                    {
                        if (fPoint.Z() < value && value < midPoint.Z())
                            lParam = midParam;
                        else //((midPoint.Z() < value && value < lPoint.Z())
                            fParam = midParam;
                    }
                    else
                    {
                        if (lPoint.Z() < value && value < midPoint.Z())
                            fParam = midParam;
                        else //((midPoint.Z() < value && value < fPoint.Z())
                            lParam = midParam;
                    }

                    paramDiff = fabs(lParam - fParam);
                }
            }
        }
    }
    else if (coordinateAxis == 4)
    {
        double r1 = sqrt(fPoint.X() * fPoint.X() + fPoint.Y() * fPoint.Y());
        double r2 = sqrt(lPoint.X() * lPoint.X() + lPoint.Y() * lPoint.Y());

        if (fabs(r1 - value) < tolerance)
        {
            parameter = fParam;
            isFound = true;
        }
        if (fabs(r2 - value) < tolerance)
        {
            parameter = lParam;
            isFound = true;
        }

        bool condition1 = (r1 < value && value < r2);
        bool condition2 = (r2 < value && value < r1);

        if (condition1 || condition2)
        {
            double midParam;
            double paramDiff = (lParam - fParam);
            while (paramDiff > paramTolerance)
            {
                midParam = (fParam + lParam) / 2.0;
                gp_Pnt midPoint = curve->Value(midParam);
                double midR = sqrt(midPoint.X() * midPoint.X() + midPoint.Y() * midPoint.Y());

                if (fabs(midR - value) < tolerance)
                {
                    isFound = true;
                    parameter = midParam;
                    break;
                }

                if (condition1)
                {
                    if (r1 < value && value < midR)
                        lParam = midParam;
                    else //(midParam < value && value < r2)
                        fParam = midParam;
                }
                else
                {
                    if (r2 < value && value < midR)
                        fParam = midParam;
                    else //(midParam < value && value < r1)
                        lParam = midParam;
                }

                paramDiff = fabs(lParam - fParam);
            }
        }
    }
    else
    {
        //printErrorOnly("Invalid coordinateAxis. It must be 1, 2, 3 or 4.");
    }

    return isFound;
}

// Find the point with the coordinate value in the coordinate axis from the curve
// Create a surface with coordinateAxis direction and coordinateValue point and get intersection with the curve to get the point.
// @param point - Return the point with the coordinat  
// @param curve : Curve to serach for the coordinateValue
// @param coordinateAxis :  1 -> match X value, 2 -> match Y value, 3 -> match Z value
// @param coordinateValue : Value to search.
// @return true if successfully find the point
bool TGenCoolTools::getCurvePointOfCoordinateValue(gp_Pnt& point, Handle(Geom_Curve) curve, int coordinateAxis, double coordinateValue)
{
    if (curve.IsNull())
        return false;

    gp_Dir tangentDir;// (0.0, 0.0, 0.0);
    gp_Pnt pointInAxis;// (0.0, 0.0, 0.0);
    if (coordinateAxis == 1)
    {
        tangentDir.SetX(1.0);
        pointInAxis.SetX(coordinateValue);
    }
    else if (coordinateAxis == 2)
    {
        tangentDir.SetY(1.0);
        pointInAxis.SetY(coordinateValue);
    }
    else if (coordinateAxis == 3)
    {
        tangentDir.SetZ(1.0);
        pointInAxis.SetZ(coordinateValue);
    }
    else
    {
        //printErrorOnly("Invalid coordinateAxis. It must be 1,2 or 3.");
        return false;
    }

    /*Creating the Normal Plane */
    gce_MakePln makePlane(pointInAxis, tangentDir);
    gp_Pln normalPlane = makePlane.Value();

    /*Creating a shape on the plane */
    BRepBuilderAPI_MakeFace mkFace(normalPlane);
    TopoDS_Face planeFace;
    if (mkFace.IsDone())
    {
        planeFace = mkFace.Face();
    }
    else
    {
        //eprintf("Failed to create Face from a plane ");
        return false;
    }

    /*Creating a trimed surface on the shape */
    BRepAdaptor_Surface surface(planeFace);
    GeomAdaptor_Surface adaptorSurface = surface.Surface();
    Handle(Geom_RectangularTrimmedSurface) trimmedSurface = new Geom_RectangularTrimmedSurface(adaptorSurface.Surface(), adaptorSurface.FirstUParameter(), adaptorSurface.LastUParameter(), adaptorSurface.FirstVParameter(), adaptorSurface.LastVParameter());

    GeomAPI_IntCS Intersector(curve, trimmedSurface);
    Standard_Integer nb = Intersector.NbPoints();
    if (nb == 1)
    {
        point = Intersector.Point(1);
        return true;
    }
    else if (nb > 1)
    {
        //printErrorOnly("Multiple intersections exist.");
        return false;
    }
    else
    {
        return false;
    }
}

// Get the parameters in the surface, where the point located in the surface.
// @param uParameter - Return the U parameter of point in the surface
// @param vParameter - Return the V parameter of point in the surface
// @param surface - Surface to find parameter
// @param point - Point to find parameter from surface
// @param tolerance - Maximum difference between the point and point in the surface
// @return false if failed to find parameter with tolerance.
bool TGenCoolTools::getSurfaceParameter(double& uParameter, double& vParameter, Handle(Geom_Surface) surface, gp_Pnt point, double tolerance)
{
    if (surface.IsNull())
        return false;

    if (!GeomLib_Tool::Parameters(surface, point, tolerance, uParameter, vParameter))
    {
        double surfaceU1, surfaceU2, surfaceV1, surfaceV2;
        surface->Bounds(surfaceU1, surfaceU2, surfaceV1, surfaceV2);
        double tempUParam = surfaceU1 - (surfaceU2 - surfaceU1);
        double tempVParam = surfaceV1 - (surfaceV2 - surfaceV1);

        BRepExtrema_ExtPF epf(BRepBuilderAPI_MakeVertex(point).Vertex(), BRepBuilderAPI_MakeFace(surface, 1.0e-5).Face());
        if (epf.IsDone() && epf.NbExt() > 0)
        {
            double minDist = DBL_MAX;
            for (int i = 1; i <= epf.NbExt(); i++)
            {
                double dist = epf.SquareDistance(i);
                if (dist < minDist)
                {
                    minDist = dist;
                    epf.Parameter(i, tempUParam, tempVParam);
                }
            }
        }

        if (tempUParam < surfaceU1 || tempUParam > surfaceU2 || tempVParam < surfaceV1 || tempVParam > surfaceV2)
            return false;
        else
        {
            uParameter = tempUParam;
            vParameter = tempVParam;
        }
    }

    return true;
}

// Find the parameters of closest points between two curves
// @param curve1 - First curve
// @param curve2 - Second curve
// @param parameterInCurve1 - Parameter of the point in the first curve
// @param parameterInCurve2 - Parameter of the point in the second curve
// @retrun true if successfully find the closest point
bool TGenCoolTools::findClosestPoint(Handle(Geom_Curve) curve1, Handle(Geom_Curve) curve2, double& parameterInCurve1, double& parameterInCurve2)
{
    if (curve1.IsNull() || curve2.IsNull())
        return false;

    GeomAPI_ExtremaCurveCurve extCC(curve1, curve2);
    int numPoints = extCC.NbExtrema();

    if (numPoints < 1)
        return false;

    //pick the best point
    double minDistance = DBL_MAX;
    for (int i = 1; i <= numPoints; i++)
    {
        Standard_Real dist = extCC.Distance(i);
        if (dist < minDistance)
        {
            minDistance = dist;
            extCC.Parameters(i, parameterInCurve1, parameterInCurve2);
        }
    }

    return true;
}

// Create face from the wire using base surface
// @param baseSurface - Support surface to create face
// @param wire - Wire to create face
// @return generated face
TopoDS_Face TGenCoolTools::makeFaceFromWire(Handle(Geom_Surface) baseSurface, TopoDS_Wire wire)
{
    if (wire.IsNull())
        return TopoDS_Face();

    //fix the wire properly
    ShapeFix_Wire fixWire(wire, BRepBuilderAPI_MakeFace(baseSurface, 1.0e-10).Face(), 1.0e-10);
    fixWire.ClosedWireMode() = Standard_True;
    fixWire.FixAddPCurveMode() = Standard_True;
    fixWire.FixAddCurve3dMode() = Standard_True;
    fixWire.FixIntersectingEdgesMode() = Standard_True;
    fixWire.Perform();
    wire = fixWire.Wire();
    //end wire fixing

    TopoDS_Face face1, face2;
    double surfArea1 = -1.0e-10;
    double surfArea2 = -1.0e-10;
    BRepBuilderAPI_MakeFace mkBaseFace1(baseSurface, wire, 0);
    BRepBuilderAPI_MakeFace mkBaseFace2(baseSurface, wire, 1);

    if (mkBaseFace1.IsDone())
    {
        face1 = mkBaseFace1.Face();
        ShapeFix_Face fixFace(face1);
        fixFace.Perform();
        face1 = fixFace.Face();
        GProp_GProps SProps1;
        BRepGProp::SurfaceProperties(face1, SProps1);
        surfArea1 = SProps1.Mass();
    }
    if (mkBaseFace2.IsDone())
    {
        face2 = mkBaseFace2.Face();
        ShapeFix_Face fixFace(face2);
        fixFace.Perform();
        face2 = fixFace.Face();
        GProp_GProps SProps2;
        BRepGProp::SurfaceProperties(face2, SProps2);
        surfArea2 = SProps2.Mass();
    }

    if (surfArea1 < 0.0 && surfArea2 < 0.0)
        return TopoDS_Face();
    else if (surfArea1 > surfArea2)
        return face1;
    else
        return face2;
}

// Scale the shape by given factor from the origin(gp_Pnt(0.0,0.0,0.0))
// @param shapeToScale - TopoDS_Shape to scale
// @param scaleFactor - Factor to scale
// @return scaled shape
TopoDS_Shape TGenCoolTools::scaleShape(TopoDS_Shape& shapeToScale, double scaleFactor)
{
    gp_Trsf  scale;
    scale.SetScale(gp_Pnt(0, 0, 0), scaleFactor);
    BRepBuilderAPI_Transform scaled(shapeToScale, scale);

    return scaled.Shape();
}

// Get the boolean cut of given list of shapes and list of cutting tools
// @param listOfShapes - Shapes to be perform boolean cut
// @param listOfTools - Shapes of cutting tools
// @param resultShape - Return resultant shape of the boolean cut
// @return true if boolean cut operation is successful
bool TGenCoolTools::booleanCutTwoShapes(TopTools_ListOfShape listOfShapes, TopTools_ListOfShape listOfTools, TopoDS_Shape& cutShape)
{
    Standard_Boolean bRunParallel;
    Standard_Boolean iErr;
    Standard_Real aFuzzyValue;
    BRepAlgoAPI_Cut aBuilder;

    bRunParallel = Standard_True;
    aFuzzyValue = 2.1e-5;
    //
    // set the arguments  
    aBuilder.SetArguments(listOfShapes);
    aBuilder.SetTools(listOfTools);
    //    
    // set parallel processing mode 
    // if  bRunParallel= Standard_True :  the parallel processing is switched on
    // if  bRunParallel= Standard_False :  the parallel processing is switched off
    aBuilder.SetRunParallel(bRunParallel);
    //
    // set Fuzzy value
    // if aFuzzyValue=0.: the Fuzzy option is off
    //  if aFuzzyValue>0.: the Fuzzy option is on
    aBuilder.SetFuzzyValue(aFuzzyValue);
    //
    // run the algorithm 
    aBuilder.Build();
    iErr = aBuilder.HasErrors();
    if (iErr) {
        // an error treatment
        return false;
    }
    cutShape = aBuilder.Shape();
    return true;
}

// Rotate the face around Z Axis (OZ) by given angle
// @param face - TopoDS_Face to rotate
// @param angle - Angle (in radians) to rotate.
// @return the rotated face
TopoDS_Face TGenCoolTools::rotateFaceAroundOZ(TopoDS_Face face, double angle)
{
    gp_Trsf  rotate;
    rotate.SetRotation(gp::OZ(), angle);
    BRepBuilderAPI_Transform rotated(face, rotate);

    TopoDS_Face rotatedFace;
    for (TopExp_Explorer explr(rotated.Shape(), TopAbs_FACE); explr.More(); explr.Next())
        rotatedFace = TopoDS::Face(explr.Current());

    return rotatedFace;
}

// Create 2D fillet between two edges with given radius.
// @param filletEdge - Return the generated fillet
// @param edge1 - First edge as input and return the modified edge after filleting
// @param edge2 - Second edge as input and return the modified edge after filleting
// @param commonPoint - The point where the fillet should be generate and it shoud be common to both edge1 and edge2
// @param radius - Radius of the fillet
// @param plane - The plane where the both edge1 and edge2 lay
// @return true if filleting is successful 
bool TGenCoolTools::make2DFillet(TopoDS_Edge& filletEdge, TopoDS_Edge& edge1, TopoDS_Edge& edge2, gp_Pnt commonPoint, Standard_Real radius, gp_Ax3 plane)
{
    if (radius <= 0)
        return false;

    ChFi2d_FilletAPI fillet(edge1, edge2, plane);
    fillet.Perform(radius);
    Standard_Integer j = fillet.NbResults(commonPoint);

    if (j != 0)
    {
        filletEdge = fillet.Result(commonPoint, edge1, edge2);
    }
    else
    {
        return false;
    }

    if (edge1.IsNull() || edge2.IsNull() || filletEdge.IsNull())
    {
        return false;
    }

    return true;
}

// Find the common edge between face1 and face2
// @param commonEdge - Return the edge common to both face1 and face2
// @param face1 - First face
// @param face2 - Second face
// @return true if a coomon edge found
bool TGenCoolTools::findCommonEdge(TopoDS_Edge& commonEdge, TopoDS_Face face1, TopoDS_Face face2)
{
    if (face1.IsNull() || face2.IsNull())
        return false;

    BRepExtrema_DistShapeShape dss(face1, face2);
    dss.Perform();
    if (dss.IsDone())
    {
        double distValue = dss.Value();
        if (distValue > 1.0e-1)
            return false;
    }

    double sewingTol = 1.0e-6;
    while (sewingTol < 1.0e-2)
    {
        BRepBuilderAPI_Sewing sewer(sewingTol);
        sewer.Add(face1);
        sewer.Add(face2);
        sewer.Perform();

        std::vector<TopoDS_Face> tempFaces;
        for (TopExp_Explorer explr(sewer.SewedShape(), TopAbs_FACE); explr.More(); explr.Next())
        {
            tempFaces.push_back(TopoDS::Face(explr.Current()));
        }

        TopTools_ListOfShape edgeList, vertexList;
        if (BRepOffset_Tool::FindCommonShapes(tempFaces[0], tempFaces[1], edgeList, vertexList))
        {
            for (TopTools_ListIteratorOfListOfShape iter(edgeList); iter.More(); iter.Next())
            {
                TopoDS_Shape commonShape = iter.Value();
                commonEdge = TopoDS::Edge(commonShape);
            }
        }

        if (!commonEdge.IsNull())
            break;

        sewingTol *= 10;
    }

    if (commonEdge.IsNull())
        return false;

    return true;
}

// Get the parameter of a point in referenceCurve which is normal to the main curve in given parameter of mainCurve.
// @param mainCurve - Curve to create normal from
// @param parameterInMainCurve - Location of the main curve normal has to be created
// @param referenceCurve - Curve to find the normal point
// @param parameterInReferenceCurve - Return the parameter of normal point if exist in reference curve.
// @return true if successfully found the normal point parameter in reference curve.
bool TGenCoolTools::getParameterOfNormalPointToCurve(Handle(Geom_Curve) mainCurve, double parameterInMainCurve, Handle(Geom_Curve) referenceCurve, double& parameterInReferenceCurve)
{
    if (mainCurve.IsNull() || referenceCurve.IsNull())
        return false;

    if (parameterInMainCurve < mainCurve->FirstParameter() || mainCurve->LastParameter() < parameterInMainCurve)
        return false;

    //Get the point and tangent for the parameter value in the main curve
    gp_Pnt mainCurvePoint;
    gp_Vec mainCurveVec;
    mainCurve->D1(parameterInMainCurve, mainCurvePoint, mainCurveVec);

    // Creating the plane normal to main curve
    gp_Dir tangentDir(mainCurveVec);
    gce_MakePln makeNormalPlane(mainCurvePoint, tangentDir);
    gp_Pln normalPlane = makeNormalPlane.Value();

    // Creating face on the normal plane
    BRepBuilderAPI_MakeFace mkFace(normalPlane);
    TopoDS_Face normalFace;
    if (mkFace.IsDone())
    {
        normalFace = mkFace.Face();
    }
    else
    {
        //printWarningOnly("Failed to create a normal face to main curve.");
        return false;
    }

    //Creating a trimed surface from the norma face  
    GeomAdaptor_Surface adaptorSurface = BRepAdaptor_Surface(normalFace).Surface();
    Handle(Geom_RectangularTrimmedSurface) trimmedSurface = new Geom_RectangularTrimmedSurface(adaptorSurface.Surface(), adaptorSurface.FirstUParameter(), adaptorSurface.LastUParameter(), adaptorSurface.FirstVParameter(), adaptorSurface.LastVParameter());

    //Getting intersection between the normal surface and reference curve
    GeomAPI_IntCS intcs(referenceCurve, trimmedSurface);
    if (intcs.IsDone() && intcs.NbPoints() > 0)
    {
        int nb = intcs.NbPoints();
        if (nb == 1)
        {
            double u, v;
            intcs.Parameters(1, u, v, parameterInReferenceCurve);
        }
        else
        {
            //printWarningOnly("More than one normal point found in reference curve.");
            return false;
        }
    }
    else
    {
        //printWarningOnly("Normal point is not found in reference curve.");
        return false;
    }

    return true;
}

// Get the point in referenceCurve which is normal to the main curve in given parameter of mainCurve.
// @param mainCurve - Curve to create normal from
// @param parameterInMainCurve - Location of the main curve normal has to be created
// @param referenceCurve - Curve to find the normal point
// @param pointInReferenceCurve - Return the normal point if exist in reference curve.
// @return true if successfully found the normal point in reference curve.
bool TGenCoolTools::getNormalPointToCurve(Handle(Geom_Curve) mainCurve, double parameterInMainCurve, Handle(Geom_Curve) referenceCurve, gp_Pnt& pointInReferenceCurve)
{
    double parameteraInReferenceCurve;
    if (getParameterOfNormalPointToCurve(mainCurve, parameterInMainCurve, referenceCurve, parameteraInReferenceCurve))
        pointInReferenceCurve = referenceCurve->Value(parameteraInReferenceCurve);
    else
        return false;

    return true;
}

// Get the point in referenceCurve which is normal to the main curve in given point of mainCurve.
// @param mainCurve - Curve to create normal from
// @param pointInMainCurve - Location of the main curve normal has to be created
// @param referenceCurve - Curve to find the normal point
// @param pointInReferenceCurve - Return the normal point if exist in reference curve.
// @return true if successfully found the normal point in reference curve.
bool TGenCoolTools::getNormalPointToCurve(Handle(Geom_Curve) mainCurve, gp_Pnt pointInMainCurve, Handle(Geom_Curve) referenceCurve, gp_Pnt& pointInReferenceCurve)
{
    double parameterInMainCurve;
    if (getCurveParameter(parameterInMainCurve, mainCurve, pointInMainCurve))
    {
        double parameteraInReferenceCurve;
        if (getParameterOfNormalPointToCurve(mainCurve, parameterInMainCurve, referenceCurve, parameteraInReferenceCurve))
            pointInReferenceCurve = referenceCurve->Value(parameteraInReferenceCurve);
        else
            return false;
    }
    else
        return false;

    return true;
}


// Split a given face into set of faces by a plane  
// @param splittedFaces - vector of splitted faces
// @param originalFace - original face before splitting 
// @param splittingFace - splitting face use to split the original face
bool TGenCoolTools::splitFace(QVector<TopoDS_Face>& splittedFaces, TopoDS_Face originalFace, gp_Pln myPln)
{
    BRepAlgoAPI_Section asect(originalFace, myPln, Standard_False);
    asect.ComputePCurveOn1(Standard_True);
    asect.Approximation(Standard_True);
    asect.Build();
    TopoDS_Shape splitShape = asect.Shape();

    BRepFeat_SplitShape asplitFirst(originalFace);

    for (TopExp_Explorer Ex(splitShape, TopAbs_EDGE); Ex.More(); Ex.Next())
    {
        TopoDS_Shape currentShape = Ex.Current();
        TopoDS_Edge currentEdge = TopoDS::Edge(currentShape);

        TopoDS_Shape firstShape;
        if (asect.HasAncestorFaceOn1(currentShape, firstShape))
        {
            TopoDS_Face firstFace = TopoDS::Face(firstShape);
            asplitFirst.Add(currentEdge, firstFace);
        }
    }
    asplitFirst.Build();

    originalFace.Reverse();
    BRepFeat_SplitShape asplitSecond(originalFace);

    for (TopExp_Explorer Ex(splitShape, TopAbs_EDGE); Ex.More(); Ex.Next())
    {
        TopoDS_Shape currentShape = Ex.Current();
        TopoDS_Edge currentEdge = TopoDS::Edge(currentShape);

        TopoDS_Shape secondShape;
        if (asect.HasAncestorFaceOn1(currentShape, secondShape))
        {
            TopoDS_Face secondFace = TopoDS::Face(secondShape);
            asplitSecond.Add(currentEdge, secondFace);
        }
    }
    asplitSecond.Build();

    TopTools_ListIteratorOfListOfShape shapeListFirst = asplitFirst.DirectLeft();
    TopTools_ListIteratorOfListOfShape shapeListSecond = asplitSecond.DirectLeft();

    for (TopTools_ListIteratorOfListOfShape iterFst(shapeListFirst); iterFst.More(); iterFst.Next())
    {
        TopoDS_Face currentFace = TopoDS::Face(iterFst.Value());
        splittedFaces.push_back(currentFace);
    }

    for (TopTools_ListIteratorOfListOfShape iterSecond(shapeListSecond); iterSecond.More(); iterSecond.Next())
    {
        TopoDS_Face currentFace = TopoDS::Face(iterSecond.Value());
        splittedFaces.push_back(currentFace);
    }

    return true;
}

// Get the plane of a 2d face
// @param face - 2d face
// @param plane - corresponding plane of the 2d face
bool TGenCoolTools::getPlaneof2DFace(TopoDS_Face face, gp_Pln& plane)
{
    //Get the center of mass
    GProp_GProps LProps;
    BRepGProp::LinearProperties(face, LProps);
    gp_Pnt massPnt = LProps.CentreOfMass();

    //Get any edge of the face 
    for (TopExp_Explorer explr(face, TopAbs_EDGE); explr.More(); explr.Next())
    {
        TopoDS_Edge currentEdge = TopoDS::Edge(explr.Current());
        double uF, uL;
        Handle(Geom_Curve) currentCurve = BRep_Tool::Curve(currentEdge, uF, uL);
        gp_Pnt fstPnt = currentCurve->Value(uF);
        gp_Pnt lstPnt = currentCurve->Value(uL);

        gp_Vec vec1(massPnt, fstPnt);
        gp_Vec vec2(massPnt, lstPnt);

        gp_Vec crossVec = vec1.Crossed(vec2);
        crossVec.Normalize();

        plane = gp_Pln(massPnt, crossVec);
        return true;
    }

    return false;
}

// Generate a straight BSpline curve going throug given end points
// @param bSplineCurve - return generated BSpline curve
// @param startPoint - Start point of the curve
// @param endPoint - End point of the curve
// return true if interpolation is successful
bool TGenCoolTools::makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& geomBSplineCurve, gp_Pnt startPoint, gp_Pnt endPoint)
{
    TColgp_Array1OfPnt bspPoles(1, 2);
    bspPoles.SetValue(1, startPoint);
    bspPoles.SetValue(2, endPoint);

    TColStd_Array1OfReal bspKnots(1, 2);
    bspKnots.SetValue(1, 0.0);
    bspKnots.SetValue(2, 1.0);

    TColStd_Array1OfInteger bspMults(1, 2);
    bspMults.SetValue(1, 2);
    bspMults.SetValue(2, 2);

    int bspDegree = 1;

    try
    {
        OCC_CATCH_SIGNALS
            geomBSplineCurve = new Geom_BSplineCurve(bspPoles, bspKnots, bspMults, bspDegree);
    }
    catch (Standard_ConstructionError)
    {
        //printWarningOnly("Distance between two consecutive points in the table Points is less than or equal to Tolerance.");
        return false;
    }
    catch (Standard_Failure)
    {
        //eprintf("BSpline curve generation failed");
        return false;
    }
    catch (...)
    {
        //eprintf("Unhandled exception in generating BSplineCurve");
        return false;
    }

    return true;
}

// Generate BSpline curve going throug the given points using interpolation
// @param bSplineCurve - return generated BSpline curve
// @param pointsForCurve - Points to interpolate
// @param tolerance - Tolerance for interpolation which is the minimum distance between two consecutive points
// return true if interpolation is successful
bool TGenCoolTools::makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& bSplineCurve, std::vector<gp_Pnt> pointsForCurve, double tolerance)
{
    int numberOfPoints = (int)pointsForCurve.size();

    if (numberOfPoints <= 1)
        return false;

    //check the first and last points for finite. No time to check all
    if (!std::isfinite(pointsForCurve[0].X()) || !std::isfinite(pointsForCurve[0].Y()) || !std::isfinite(pointsForCurve[0].Z()))
    {
        //printErrorOnly("Infinite values found in point array for interpolation");
        return false;
    }

    //Check the points vector for minimum distance between consecutive points
    std::vector<gp_Pnt> validatedPoints;
    if (numberOfPoints > 2)
    {
        gp_Pnt prevPoint = pointsForCurve[0];
        gp_Pnt curPoint;
        double dist;

        validatedPoints.push_back(prevPoint);

        for (int i = 1; i < numberOfPoints - 1; i++)
        {
            curPoint = pointsForCurve[i];
            dist = prevPoint.Distance(curPoint);

            if (dist > tolerance)
            {
                prevPoint = curPoint;
                validatedPoints.push_back(curPoint);
            }
        }

        curPoint = pointsForCurve[numberOfPoints - 1];
        dist = prevPoint.Distance(curPoint);

        if (dist > tolerance)
            validatedPoints.push_back(curPoint);
        else
        {
            validatedPoints.pop_back();
            validatedPoints.push_back(curPoint);
        }
    }
    else
        validatedPoints = pointsForCurve;

    int validatedNumberOfPoints = (int)validatedPoints.size();

    if (validatedNumberOfPoints <= 1)
        return false;

    Handle(TColgp_HArray1OfPnt) pPoints = new TColgp_HArray1OfPnt(1, validatedNumberOfPoints);
    for (int i = 1; i <= validatedNumberOfPoints; i++)
        pPoints->SetValue(i, validatedPoints[i - 1]);

    try
    {
        OCC_CATCH_SIGNALS
            GeomAPI_Interpolate interp(pPoints, Standard_False, tolerance);
        interp.Perform();
        if (interp.IsDone())
        {
            bSplineCurve = interp.Curve();
        }
    }
    catch (Standard_ConstructionError)
    {
        //eprintf("Distance between two consecutive points in the points vector is less than or equal to tolerance.");
        //printWarningOnly("BSpline curve interpolation failed");
        return false;
    }
    catch (Standard_OutOfRange)
    {
        //eprintf("There are less than two points to generate BSpline Curve.");
        //printWarningOnly("BSpline curve interpolation failed");
        return false;
    }
    catch (Standard_Failure)
    {
        //printWarningOnly("BSpline curve interpolation failed");
        return false;
    }
    catch (...)
    {
        //printWarningOnly("Unhandled exception in generating BSplineCurve");
        return false;
    }

    return true;
}

// Generate BSpline curve going throug the given points using interpolation
// @param bSplineCurve - return generated BSpline curve
// @param pointsForCurve - Points to interpolate
// @param tolerance - Tolerance for interpolation which is the minimum distance between two consecutive points
// return true if interpolation is successful
bool TGenCoolTools::makeGeomBSplineCurveFromPoints(Handle(Geom_BSplineCurve)& bSplineCurve, QVector<gp_Pnt> pointsForCurve, double tolerance)
{
    const int numberOfPoints = pointsForCurve.size();

    if (numberOfPoints <= 1)
        return false;

    //check the first and last points for finite. No time to check all
    if (!std::isfinite(pointsForCurve[0].X()) || !std::isfinite(pointsForCurve[0].Y()) || !std::isfinite(pointsForCurve[0].Z()))
    {
        //printErrorOnly("Infinite values found in point array for interpolation");
        return false;
    }

    //Check the points vector for minimum distance between consecutive points
    QVector<gp_Pnt> validatedPoints;
    if (numberOfPoints > 2)
    {
        gp_Pnt prevPoint = pointsForCurve[0];
        gp_Pnt curPoint;
        double dist;

        validatedPoints.push_back(prevPoint);

        for (int i = 1; i < numberOfPoints - 1; i++)
        {
            curPoint = pointsForCurve[i];
            dist = prevPoint.Distance(curPoint);

            if (dist > tolerance)
            {
                prevPoint = curPoint;
                validatedPoints.push_back(curPoint);
            }
        }

        curPoint = pointsForCurve[numberOfPoints - 1];
        dist = prevPoint.Distance(curPoint);

        if (dist > tolerance)
            validatedPoints.push_back(curPoint);
        else
        {
            validatedPoints.pop_back();
            validatedPoints.push_back(curPoint);
        }
    }
    else
        validatedPoints = pointsForCurve;

    const int validatedNumberOfPoints = validatedPoints.size();

    if (validatedNumberOfPoints <= 1)
        return false;

    Handle(TColgp_HArray1OfPnt) pPoints = new TColgp_HArray1OfPnt(1, validatedNumberOfPoints);
    for (int i = 1; i <= validatedNumberOfPoints; i++)
        pPoints->SetValue(i, validatedPoints[i - 1]);

    try
    {
        OCC_CATCH_SIGNALS
            GeomAPI_Interpolate interp(pPoints, Standard_False, 1.0e-15);
        interp.Perform();
        if (interp.IsDone())
        {
            bSplineCurve = interp.Curve();
        }
    }
    catch (Standard_ConstructionError)
    {
        //eprintf("BSpline curve interpolation failed");
        //eprintf("Distance between two consecutive points in the table Points is less than or equal to Tolerance.");
        return false;
    }
    catch (Standard_OutOfRange)
    {
        //eprintf("BSpline curve interpolation failed");
        //eprintf("There are less than two points to generate BSpline Curve.");
        return false;
    }
    catch (Standard_Failure)
    {
        //eprintf("BSpline curve interpolation failed");
        return false;
    }
    catch (...)
    {
        //eprintf("Unhandled exception in generating BSplineCurve");
        return false;
    }

    return true;
}

// Extend surface by length percentage
// @param surface - surface to extend
// @param lengthFraction - Fraction (0 to 1) of the current curve length to extend
// @param atEnd - true : Extend the curve at end , false : Extend the curve at start 
// @param atU - true : Extend the curve at U direction , false : Extend the curve at V direction 
// @return extended surface.
Handle(Geom_Surface) TGenCoolTools::extendSurfaceByLengthFraction(Handle(Geom_Surface) surface, double lengthFraction, bool atU, bool atEnd)
{
    Handle(Geom_BoundedSurface)  surfBsp = Handle(Geom_BoundedSurface)::DownCast(surface);
    Standard_Real U1, U2, V1, V2;
    surfBsp->Bounds(U1, U2, V1, V2);

    Handle(Geom_Curve) curve;
    if (!atU)
        curve = surfBsp->UIso(U1);
    else
        curve = surfBsp->VIso(V1);

    double length = getCurveLength(curve);
    double lengthExtend = length * lengthFraction;

    GeomLib::ExtendSurfByLength(surfBsp, lengthExtend, 2, atU, atEnd);

    return surfBsp;
}

// Segment the bspline curve between two parameters
// @param segmentedCurve - Return the segmented curve
// @param curveToBeSegmented - Input curve to be segmented
// @param paramFrom - Start parameter of the curve segment in curve to be segmeted -> paramFrom < paramTo
// @param paramTo - End parameter of the curve segment in curve to be segmeted -> paramFrom < paramTo
// @return true if the segmentation is successful
bool TGenCoolTools::segmentGeomBSplineCurve(Handle(Geom_BSplineCurve)& segmentedCurve, const Handle(Geom_BSplineCurve)& curveToBeSegmented, double paramFrom, double paramTo)
{
    if (curveToBeSegmented.IsNull())
        return false;

    double tolerance = Precision::PConfusion();
    double paramDiff = fabs(paramFrom - paramTo);
    if (paramDiff < tolerance)
        return false;

    segmentedCurve = Handle(Geom_BSplineCurve)::DownCast(curveToBeSegmented->Copy());

    try
    {
        OCC_CATCH_SIGNALS
            if (paramFrom < paramTo)
                segmentedCurve->Segment(paramFrom, paramTo, tolerance);
            else
                segmentedCurve->Segment(paramTo, paramFrom, tolerance);
    }
    catch (Standard_Failure)
    {
        return false;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// Convert non bspline curve to bspline curve
// @param geomCurve - Curve to be converted as BSpline curve
// @param bspCurve - Returns converted BSpline curve
// @return true if curve is successfully converted to BSpline curve
bool TGenCoolTools::convertToGeomBSplineCurve(const Handle(Geom_Curve)& geomCurve, Handle(Geom_BSplineCurve)& bspCurve)
{
    if (geomCurve.IsNull())
        return false;

    //Check the curve type
    Handle_Standard_Type curveType = geomCurve->DynamicType();
    QString curveTypeName(curveType->Name());

    if (curveTypeName.contains("Geom_BSplineCurve"))
        bspCurve = Handle_Geom_BSplineCurve::DownCast(geomCurve->Copy());
    else if (curveTypeName.contains("Geom_BezierCurve") || curveTypeName.contains("Geom_TrimmedCurve")) //Geom_BoundedCurve
    {
        Handle_Geom_BoundedCurve boundedCurve = Handle_Geom_BoundedCurve::DownCast(geomCurve->Copy());
        GeomConvert_CompCurveToBSplineCurve convertToBSP(boundedCurve);
        bspCurve = convertToBSP.BSplineCurve();
    }
    else
    {
        int numberOfPoints = ceil(getCurveLength(geomCurve));
        if (numberOfPoints < 10)
            numberOfPoints = 10;

        std::vector<gp_Pnt> pointsForBspCurve = generatePointsFromCurve(geomCurve, numberOfPoints);
        if (!makeGeomBSplineCurveFromPoints(bspCurve, pointsForBspCurve))
            return false;
    }

    return true;
}


// Compare the orientation of curve with orientation of rzCurve, if the orientations do not match, reverse the orientation of curve.
// @param curve - Curve to check the orientation, and return the oriented curve. This curve should be in rzCurve revolved surface
// @param rzCurve - RZ Curve (2D curve -> (0.0, R, Z) )
// @return true if orientation comparision is done.
bool TGenCoolTools::reverseCurveBasedOnRZCurve(Handle_Geom_Curve& curve, const Handle_Geom_Curve& rzCurve)
{
    if (curve.IsNull() || rzCurve.IsNull())
        return false;

    gp_Pnt curveFPoint = RZPOINT(curve->Value(curve->FirstParameter()));
    gp_Pnt curveLPoint = RZPOINT(curve->Value(curve->LastParameter()));

    gp_Pnt rzCurveFPoint = rzCurve->Value(rzCurve->FirstParameter());
    gp_Pnt rzCurveLPoint = rzCurve->Value(rzCurve->LastParameter());

    double paramF;
    if (!getCurveParameter(paramF, rzCurve, curveFPoint, 1e-3))
        return false;

    double paramL;
    if (!getCurveParameter(paramL, rzCurve, curveLPoint, 1e-3))
        return false;

    if (paramF > paramL)
        curve->Reverse();

    return false;
}

// Extend the mean line curve to cover the stream line (flow path meridional curve) along stream line revolved surface
// @param meanLineCurve - Mean line curve to be extended and return the extended meanline curve
// @param streamLineCurve - Stream line (Flow path meridional) curve of the mean line curve
// @return true if successfully extend mean line curve
#define DEBUG_AIFOIL 0
bool TGenCoolTools::extendMeanLineCurveAlongStreamLineSurface(Handle_Geom_Curve& meanLineCurve, const Handle_Geom_Curve& streamLineCurve)
{
    if (meanLineCurve.IsNull() || streamLineCurve.IsNull())
        return false;

    Handle_Geom_Curve meridionalCurve;
    if (!createRZCurve(meridionalCurve, meanLineCurve))
        return false;

#if DEBUG_AIFOIL
    QString filePath = "G:/Turbo Tides/Development/Airfoil_TGenCoolTools/ExtMean-";
    BRepTools::Write(BRepBuilderAPI_MakeEdge(meanLineCurve).Edge(), qPrintable(filePath + "meanLine.brep"));
    BRepTools::Write(BRepBuilderAPI_MakeEdge(streamLineCurve).Edge(), qPrintable(filePath + "streamLine.brep"));
    BRepTools::Write(BRepBuilderAPI_MakeEdge(meridionalCurve).Edge(), qPrintable(filePath + "meridionalCurve.brep"));
    BRepTools::Write(BRepPrimAPI_MakeRevol(BRepBuilderAPI_MakeEdge(streamLineCurve).Edge(), gp::OZ()).Shape(), qPrintable(filePath + "streamLineRevSurface.brep"));
#endif

    double meanLineLength = getCurveLength(meanLineCurve);
    double meridionalLength = getCurveLength(meridionalCurve);
    double streamLineLength = getCurveLength(streamLineCurve);

    if (fabs(meridionalLength - streamLineLength) < 1.0e-6)
        return true;

    double streamLineCurveFParam = streamLineCurve->FirstParameter();
    double streamLineCurveLParam = streamLineCurve->LastParameter();

    double meridionalCurveFParam = meridionalCurve->FirstParameter();
    double meridionalCurveLParam = meridionalCurve->LastParameter();

    gp_Pnt streamLineCurveFPoint = streamLineCurve->Value(streamLineCurveFParam);
    gp_Pnt streamLineCurveLPoint = streamLineCurve->Value(streamLineCurveLParam);

    gp_Pnt meridionalCurveFPoint = meridionalCurve->Value(meridionalCurveFParam);
    gp_Pnt meridionalCurveLPoint = meridionalCurve->Value(meridionalCurveLParam);

    double noseDistance = meridionalCurveFPoint.Distance(streamLineCurveFPoint);
    double tailDistance = meridionalCurveLPoint.Distance(streamLineCurveLPoint);

    if (noseDistance < 1.0e-6 && tailDistance < 1.0e-6)
        return true;

    double merLenToMeanLenFraction = meanLineLength / meridionalLength;

    int numberOfPoints = ceil(meridionalLength);
    if (numberOfPoints < 20)
        numberOfPoints = 20;

    numberOfPoints = numberOfPoints * ceil(merLenToMeanLenFraction);

    TopoDS_Edge merEdge = BRepBuilderAPI_MakeEdge(meridionalCurve).Edge();
    BRepAdaptor_Curve adaptor(merEdge);
    GCPnts_QuasiUniformAbscissa uniformAbs(adaptor, numberOfPoints);

    TopoDS_Edge meanLineEdge = BRepBuilderAPI_MakeEdge(meanLineCurve).Edge();

    std::vector<double> thetaValues;

    gp_Pnt meanLineCurveFPoint = meanLineCurve->Value(meanLineCurve->FirstParameter());
    double x = meanLineCurveFPoint.X();
    double y = meanLineCurveFPoint.Y();
    double theta = atan2(y, x);
    if (theta < 0)
    {
        theta += 2 * M_PI;
    }
    thetaValues.push_back(theta);

    for (int i = 2; i < numberOfPoints; i++)
    {
        double merParam = uniformAbs.Parameter(i);
        gp_Pnt pointInMerCurve = meridionalCurve->Value(merParam);

        //Find point meanline curve
        TopoDS_Shape merPointRevolvedShape = BRepPrimAPI_MakeRevol(BRepBuilderAPI_MakeVertex(pointInMerCurve).Vertex(), gp::OZ());
        TopoDS_Edge merPointRevolvedEdge;
        for (TopExp_Explorer exp(merPointRevolvedShape, TopAbs_EDGE); exp.More(); exp.Next())
            merPointRevolvedEdge = TopoDS::Edge(exp.Current());

#if DEBUG_AIFOIL
        //BRepTools::Write(merPointRevolvedEdge, qPrintable(filePath + "merPointRevolvedEdge-" + QString::number(i) + ".brep"));
#endif

        BRepExtrema_ExtCC  ecc(meanLineEdge, merPointRevolvedEdge);
        if (ecc.IsDone() && ecc.NbExt() > 0)
        {
            double minDist = DBL_MAX;
            double paramInMean;
            for (int i = 1; i <= ecc.NbExt(); i++)
            {
                double dist = ecc.SquareDistance(i);
                if (dist < minDist)
                {
                    minDist = dist;
                    paramInMean = ecc.ParameterOnE1(i);
                }
            }

            gp_Pnt pointInMean = meanLineCurve->Value(paramInMean);
            x = pointInMean.X();
            y = pointInMean.Y();
            theta = atan2(y, x);
            if (theta < 0)
            {
                theta += 2 * M_PI;
            }
            thetaValues.push_back(theta);
        }
    }

    gp_Pnt meanLineCurveLPoint = meanLineCurve->Value(meanLineCurve->LastParameter());
    x = meanLineCurveLPoint.X();
    y = meanLineCurveLPoint.Y();
    theta = atan2(y, x);
    if (theta < 0)
    {
        theta += 2 * M_PI;
    }
    thetaValues.push_back(theta);

    thetaValues = calculateContinuousThetaValues(thetaValues);

    numberOfPoints = (int)thetaValues.size();
    if (numberOfPoints < 10)
        return false;

    double meanFPointParamInStreamLineCurve = streamLineCurveFParam - 1.0;
    double meanLPointParamInStreamLineCurve = streamLineCurveLParam + 1.0;

    double merLineStepLen = meridionalLength / (numberOfPoints - 1);

    Handle_Geom_BoundedCurve boundedMeanCurve = Handle_Geom_BoundedCurve::DownCast(meanLineCurve);
    if (boundedMeanCurve.IsNull())
        return false;

    GeomConvert_CompCurveToBSplineCurve compCurveToBsp(boundedMeanCurve);

    //Extending nose part 
    if (noseDistance > 1.0e-6 && getCurveParameter(meanFPointParamInStreamLineCurve, streamLineCurve, meridionalCurveFPoint))
    {
        std::vector<gp_Pnt> pointsForNoseCurve;
        pointsForNoseCurve.push_back(meanLineCurveFPoint);
        if (streamLineCurveFParam < meanFPointParamInStreamLineCurve && meanFPointParamInStreamLineCurve < streamLineCurveLParam)
        {
            double currentTheta = thetaValues[0];
            double thetaDiff = thetaValues[0] - thetaValues[1];

            noseDistance = getCurveLength(streamLineCurve, streamLineCurveFParam, meanFPointParamInStreamLineCurve);
            int nPts = ceil(noseDistance / merLineStepLen);
            double streamStepLen = noseDistance / nPts;

            if (fabs(nPts * thetaDiff) > M_PI)
            {
                if (thetaDiff < 0)
                    thetaDiff = M_PI / nPts * -1;
                else
                    thetaDiff = M_PI / nPts;
            }

            for (int i = 1; i < nPts; i++)
            {
                currentTheta = currentTheta + thetaDiff;
                double rotAngle = currentTheta - M_PI / 2.0;

                double paramInStreamLine;
                if (getCurveParameter(paramInStreamLine, streamLineCurve, noseDistance - i * streamStepLen))
                {
                    gp_Pnt pointInStream = streamLineCurve->Value(paramInStreamLine);
                    gp_Pnt pointInMean = pointInStream.Rotated(gp::OZ(), rotAngle);
                    pointsForNoseCurve.insert(pointsForNoseCurve.begin(), pointInMean);
                }
            }

            currentTheta = currentTheta + thetaDiff;
            double rotAngle = currentTheta - M_PI / 2.0;
            gp_Pnt pointInMean = streamLineCurveFPoint.Rotated(gp::OZ(), rotAngle);
            pointsForNoseCurve.insert(pointsForNoseCurve.begin(), pointInMean);
        }

        Handle_Geom_BSplineCurve bspCurve;
        if (makeGeomBSplineCurveFromPoints(bspCurve, pointsForNoseCurve))
        {
            compCurveToBsp.Add(bspCurve, 1.0e-6, false);
#if DEBUG_AIFOIL
            BRepTools::Write(BRepBuilderAPI_MakeEdge(bspCurve).Edge(), qPrintable(filePath + "NoseCurve.brep"));
#endif
        }
    }

    //Extending tail part
    if (tailDistance > 1.0e-6 && getCurveParameter(meanLPointParamInStreamLineCurve, streamLineCurve, meridionalCurveLPoint))
    {
        std::vector<gp_Pnt> pointsForTailCurve;
        pointsForTailCurve.push_back(meanLineCurveLPoint);
        if (streamLineCurveFParam < meanLPointParamInStreamLineCurve && meanLPointParamInStreamLineCurve < streamLineCurveLParam)
        {
            double currentTheta = thetaValues[thetaValues.size() - 1];
            double thetaDiff = thetaValues[thetaValues.size() - 1] - thetaValues[thetaValues.size() - 2];

            tailDistance = getCurveLength(streamLineCurve, meanLPointParamInStreamLineCurve, streamLineCurveLParam);
            int nPts = ceil(tailDistance / merLineStepLen);
            double streamStepLen = tailDistance / nPts;

            if (fabs(nPts * thetaDiff) > M_PI / 2.0)
            {
                if (thetaDiff < 0)
                    thetaDiff = M_PI / (2 * nPts) * -1;
                else
                    thetaDiff = M_PI / (2 * nPts);
            }

            for (int i = 1; i < nPts; i++)
            {
                currentTheta = currentTheta + thetaDiff;
                double rotAngle = currentTheta - M_PI / 2.0;

                double paramInStreamLine;
                if (getCurveParameter(paramInStreamLine, streamLineCurve, streamLineLength - (nPts - i) * streamStepLen))
                {
                    gp_Pnt pointInStream = streamLineCurve->Value(paramInStreamLine);
                    gp_Pnt pointInMean = pointInStream.Rotated(gp::OZ(), rotAngle);
                    pointsForTailCurve.push_back(pointInMean);
                }
            }

            currentTheta = currentTheta + thetaDiff;
            double rotAngle = currentTheta - M_PI / 2.0;
            gp_Pnt pointInMean = streamLineCurveLPoint.Rotated(gp::OZ(), rotAngle);
            pointsForTailCurve.push_back(pointInMean);
        }

        Handle_Geom_BSplineCurve bspCurve;
        if (makeGeomBSplineCurveFromPoints(bspCurve, pointsForTailCurve))
        {
            compCurveToBsp.Add(bspCurve, 1.0e-6, true);
#if DEBUG_AIFOIL
            BRepTools::Write(BRepBuilderAPI_MakeEdge(bspCurve).Edge(), qPrintable(filePath + "TailCurve.brep"));
#endif
        }
    }

    boundedMeanCurve = compCurveToBsp.BSplineCurve();
    if (!boundedMeanCurve.IsNull())
        meanLineCurve = boundedMeanCurve;
    else
        return false;

#if DEBUG_AIFOIL
    BRepTools::Write(BRepBuilderAPI_MakeEdge(meanLineCurve).Edge(), qPrintable(filePath + "MeanLineCurve-Ext.brep"));
#endif

    return true;
}
#define DEBUG_AIFOIL 0

// Create 2D RZCurve from a 3D curve
// Points in the generated 2D curve have gp_Pnt(0.0,R,Z) format. 
// Here conversion happens from gp_Pnt(X,Y,Z) to gp_Pnt(0.0,R,Z) --> R = sqrt(X*X +Y*Y)
// @param rzCurve - Returns the generated curve
// @param curve - 3D curve to be converted to RZ curve
// @return true if rzCurve created successfully
bool TGenCoolTools::createRZCurve(Handle(Geom_Curve)& rzCurve, const Handle(Geom_Curve)& curve)
{
    if (curve.IsNull())
        return false;

    int numOfPts = ceil(getCurveLength(curve)) * 10;
    if (numOfPts < 50)
        numOfPts = 50;

    std::vector<double> curvatures;
    double avgCurvature = 0.0;
    std::vector<gp_Pnt> pointsTemp;

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(curve).Edge();
    BRepAdaptor_Curve adaptor(edge);
    GCPnts_QuasiUniformAbscissa uniAbscissa(adaptor, numOfPts);
    if (uniAbscissa.IsDone())
    {
        for (int i = 1; i <= uniAbscissa.NbPoints(); i++)
        {
            gp_Pnt p;
            curve->D0(uniAbscissa.Parameter(i), p);
            pointsTemp.push_back(p);

            GeomLProp_CLProps props(curve, uniAbscissa.Parameter(i), 2, Precision::Confusion());
            double curvature = props.Curvature();
            curvatures.push_back(curvature);
            avgCurvature = avgCurvature + curvature;
        }
    }

    numOfPts = (int)curvatures.size();
    if (numOfPts < 2)
        return false;

    avgCurvature = avgCurvature / curvatures.size() * 2.0;

    std::vector<gp_Pnt> points;
    int counter = 9;
    for (int i = 0; i < pointsTemp.size(); i++)
    {
        gp_Pnt pnt = pointsTemp[i];
        gp_Pnt rzPnt = RZPOINT(pnt);

        if (avgCurvature < curvatures[i])
        {
            points.push_back(rzPnt);
            counter = 1;
        }
        else
        {
            counter++;
            if (counter == 10)
            {
                counter = 1;
                points.push_back(rzPnt);
            }
            else
            {
                if (i == pointsTemp.size() - 1)
                    points.push_back(rzPnt);
            }
        }
    }

    Handle(Geom_BSplineCurve) tempBsp;
    if (makeGeomBSplineCurveFromPoints(tempBsp, points))
    {
        rzCurve = tempBsp;
    }
    else
        return false;

    return true;
}

// Calculate theta values and related curve(U) parameters along the curve
// Here theta values are in radian and continuous. Not limited to 0 t0 2*PI
// @param thetaValues - Returns the calculated theta values
// @param uValues - Returns the curve parameters to of the theta values calculated points
// @param curve - Curve to be calculate the theta values
// @param numOfValuesNeeded - Number of theta values need to be calculated
// @return true if theta calculation is success
bool TGenCoolTools::calculateThetaValuesOfCurve(std::vector<double>& thetaValues, std::vector<double>& uValues, const Handle_Geom_Curve& curve, int numOfValuesNeeded)
{
    if (curve.IsNull())
        return false;

    if (numOfValuesNeeded < 2)
    {
        numOfValuesNeeded = ceil(getCurveLength(curve));
        if (numOfValuesNeeded < 10)
            numOfValuesNeeded = 10;
    }

    thetaValues.clear();
    uValues.clear();

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(curve).Edge();
    BRepAdaptor_Curve adaptor(edge);
    GCPnts_QuasiUniformAbscissa uniformAbs(adaptor, numOfValuesNeeded);

    for (int i = 1; i <= numOfValuesNeeded; i++)
    {
        double u = uniformAbs.Parameter(i);
        gp_Pnt pntInCurve;
        curve->D0(u, pntInCurve);

        double x = pntInCurve.X();
        double y = pntInCurve.Y();

        double theta = atan2(y, x);

        if (theta < 0)
            theta += 2 * M_PI;

        thetaValues.push_back(theta);
        uValues.push_back(u);
    }

    thetaValues = calculateContinuousThetaValues(thetaValues);
    return true;
}

// Calculate continuous theta values from 0 to 2*PI limited theta values
// @param thetaValues - 0 to 2*PI limited theta values of continuous points
// @return calculated continuous theta values of limited theta values
std::vector<double> TGenCoolTools::calculateContinuousThetaValues(const std::vector<double>& thetaValues)
{
    if (thetaValues.size() < 2)
        return thetaValues;

    std::vector<double> newThetaValues;

    double previousTheta = thetaValues[0];
    newThetaValues.push_back(previousTheta);

    for (int i = 1; i < thetaValues.size(); i++)
    {
        double currentTheta = thetaValues[i];
        double thetaDiff = currentTheta - previousTheta;

        if (thetaDiff < -M_PI)
            thetaDiff = 2 * M_PI + thetaDiff;
        else if (thetaDiff > M_PI)
            thetaDiff = thetaDiff - 2 * M_PI;

        double newTheta = newThetaValues[newThetaValues.size() - 1] + thetaDiff;
        newThetaValues.push_back(newTheta);

        previousTheta = currentTheta;
    }

    return newThetaValues;
}


