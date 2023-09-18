#ifndef AEROVIEW_H
#define AEROVIEW_H

//Aero
//OCCT
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_Viewer.hxx>

//QT
#include <QOpenGLWidget>

class QMenu;
class QRubberBand;

class AeroView : public QOpenGLWidget
{
    Q_OBJECT

public:
    AeroView();
    ~AeroView();

public:
    Handle_AIS_InteractiveContext getContext();

    // Display interactive object with face boundary line aspects
    // @param interactiveObject - An AIS_InteractiveObject is going to be displayed in 3D View
    // @param objectColor - Color of the shape
    // @param updateTheViewer -true ->  Update the 3D view immediately after function call
    // @param showFaceBoundary - true -> Show face boundary false - Hide face boundary
    // @param boundaryColor - Color of the boundary
    // @param boundaryWidth - Width of boundary if the boundary lines
    // @param typeOfLine - Type of line as defined in Aspect_TypeOfLine enum 
    void displayShape(Handle_AIS_InteractiveObject interactiveObject, Quantity_Color objectColor = Quantity_Color(Quantity_NameOfColor::Quantity_NOC_ROSYBROWN), bool updateTheViewer = true, bool showFaceBoundary = true, Quantity_Color boundaryColor = Quantity_Color(Quantity_NameOfColor::Quantity_NOC_BLACK), double boundaryWidth = 1.2, Aspect_TypeOfLine typeOfLine = Aspect_TypeOfLine::Aspect_TOL_SOLID);

    // Display shape with given color 
    // @param shape - Shape to display
    // @param color - Color of the shape
    void displayShape(const TopoDS_Shape& shape, Quantity_NameOfColor color);

    void clearView();

protected:
    virtual void paintEvent(QPaintEvent* e);
    virtual void resizeEvent(QResizeEvent* e);

    // mouse events
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);

    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Key events
    //virtual void keyPressEvent(QKeyEvent* e);
    //virtual void keyReleaseEvent(QKeyEvent* e);

    void inputEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void panByMiddleButton(const QPoint& thepoint);
    void popup(const int x, const int y);

private:
    void init(void);
    void createSelectionLevelMenu();

public slots:
    void fitAll(void);
    void front(void);
    void back(void);
    void top(void);
    void bottom(void);
    void left(void);
    void right(void);
    void zoom(void);
    void pan(void);
    void rotation(void);

    void onCompound();
    void onVertex();
    void onEdge();
    void onWire();
    void onFace();

signals:
    void rightButtonClicked(QPoint point);

public:
    // the OCCT context.
    Handle(AIS_InteractiveContext) myContext;

public:
    // the OCCT viewer.
    Handle(V3d_Viewer) myViewer;

    // the OCCT view.
    Handle(V3d_View) myView;

    // the current selection mode
    TopAbs_ShapeEnum selectionMode;

    // save the mouse position.
    Standard_Integer myXMin;
    Standard_Integer myYMin;
    Standard_Integer myXMax;
    Standard_Integer myYMax;

    // mouse current mode.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };
    CurrentAction3d myCurrentMode;

    QRubberBand* myRectBand;

    QMenu* myContextMenu;
    QMenu* mySelectionLevelMenu;
};

#endif AEROVIEW_H