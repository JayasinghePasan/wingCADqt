#include "AeroView.h"

// OCCT
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>
#include <AIS_Shape.hxx>
#include <Quantity_Color.hxx>

// QT
#include <QRubberBand>
#include <QMouseEvent>
#include <QMenu>

// Only include for Windows
#include <WNT_Window.hxx>

#define GTTS_BK_COLOR_TOP 0.133, 0.424, 0.667
#define GTTS_BK_COLOR_BOTTOM 0.801, 0.91, 1


static Handle(OpenGl_GraphicDriver)& GetGraphicDriver()
{
    static Handle(OpenGl_GraphicDriver) bGraphicDriver;
    return bGraphicDriver;
}

AeroView::AeroView() :
    myContextMenu(0),
    mySelectionLevelMenu(0),
    myXMin(0),
    myYMin(0),
    myXMax(0),
    myYMax(0),
    myCurrentMode(CurAction3d_Nothing)
{
    // No Background
    setBackgroundRole(QPalette::NoRole);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}

AeroView::~AeroView()
{
}

void AeroView::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection;

    // Get graphic driver if it exists, otherwise initialise it
    Handle(OpenGl_GraphicDriver) aGraphicDriver;
    if (aGraphicDriver.IsNull())
    {
        aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    Aspect_Handle window_handle = (Aspect_Handle)winId();

    // Create appropriate window for platform
    Handle(WNT_Window) wind = new WNT_Window(window_handle);

    // Create V3dViewer and V3d_View
    myViewer = new V3d_Viewer(aGraphicDriver);

    myView = myViewer->CreateView();

    try
    {
        OCC_CATCH_SIGNALS
            myView->SetWindow(wind);
    }
    catch (Standard_Failure)
    {
        std::cout << "Setting windows failed";
    }

    if (!wind->IsMapped())
        wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    selectionMode = TopAbs_COMPOUND;

    // Set up lights etc
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myView->SetBgGradientColors(
        Quantity_Color(GTTS_BK_COLOR_TOP, Quantity_TOC_RGB),
        Quantity_Color(GTTS_BK_COLOR_BOTTOM, Quantity_TOC_RGB),
        Aspect_GFM_VER);

    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    myContext->SetDisplayMode(AIS_Shaded, true);

    const Handle(Prs3d_Drawer) selectionStyle = new Prs3d_Drawer();
    selectionStyle->SetDisplayMode(1);
    selectionStyle->SetColor(Quantity_NOC_DODGERBLUE2);
    myContext->SetSelectionStyle(selectionStyle);

    myContext->DefaultDrawer()->SetFaceBoundaryDraw(true);
    }


Handle_AIS_InteractiveContext AeroView::getContext()
{
    return myContext;
}

// Display interactive object with face boundary line aspects
// @param interactiveObject - An AIS_InteractiveObject is going to be displayed in 3D View
// @param objectColor - Color of the shape
// @param updateTheViewer -true ->  Update the 3D view immediately after function call
// @param showFaceBoundary - true -> Show face boundary false - Hide face boundary
// @param boundaryColor - Color of the boundary
// @param boundaryWidth - Width of boundary if the boundary lines
// @param typeOfLine - Type of line as defined in Aspect_TypeOfLine enum 
void AeroView::displayShape(Handle_AIS_InteractiveObject interactiveObject, Quantity_Color objectColor, bool updateTheViewer, bool showFaceBoundary, Quantity_Color boundaryColor, double boundaryWidth, Aspect_TypeOfLine typeOfLine)
{
    if (myContext.IsNull() || interactiveObject.IsNull())
        return;

    interactiveObject->SetColor(objectColor);

    interactiveObject->Attributes()->SetFaceBoundaryDraw(showFaceBoundary);
    interactiveObject->Attributes()->FaceBoundaryAspect()->SetColor(boundaryColor);
    interactiveObject->Attributes()->FaceBoundaryAspect()->SetWidth(boundaryWidth);
    interactiveObject->Attributes()->FaceBoundaryAspect()->SetTypeOfLine(typeOfLine);

    myContext->Display(interactiveObject, updateTheViewer);
}

// Display shape with given color 
// @param shape - Shape to display
// @param color - Color of the shape
void AeroView::displayShape(const TopoDS_Shape& shape, Quantity_NameOfColor color)
{
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    displayShape(aisShape, color);
    fitAll();
}

void AeroView::clearView()
{
    myContext->RemoveAll(true);
}

void AeroView::paintEvent(QPaintEvent* e)
{
    // eliminate the warning C4100: 'e' : unreferenced formal parameter
    Q_UNUSED(e);

    if (myContext.IsNull())
    {
        init();
    }

    myView->Redraw();
}

void AeroView::resizeEvent(QResizeEvent* e)
{
    Q_UNUSED(e);

    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}

void AeroView::fitAll(void) {
    if (!myView.IsNull()) {
        myView->FitAll();
        myView->ZFitAll();
        myView->Redraw();
    }
}

void AeroView::front(void) {
    myView->SetProj(V3d_Xpos);
}

void AeroView::back(void) {
    myView->SetProj(V3d_Xneg);
}

void AeroView::top(void) {
    myView->SetProj(V3d_Zpos);
}

void AeroView::bottom(void) {
    myView->SetProj(V3d_Zneg);
}

void AeroView::left(void) {
    myView->SetProj(V3d_Ypos);
}

void AeroView::right(void) {
    myView->SetProj(V3d_Yneg);
}

void AeroView::zoom(void)
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void AeroView::pan(void) {
    myCurrentMode = CurAction3d_DynamicPanning;
}

void AeroView::rotation(void)
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void AeroView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
}

void AeroView::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
}

void AeroView::mouseMoveEvent(QMouseEvent* e)
{
    onMouseMove(e->buttons(), e->pos());
}

void AeroView::wheelEvent(QWheelEvent* e)
{
    onMouseWheel(e->buttons(), e->delta(), e->pos());
}

void AeroView::onLButtonDown(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    // Save the current mouse coordinate in min.
    myXMin = thePoint.x();
    myYMin = thePoint.y();
    myXMax = thePoint.x();
    myYMax = thePoint.y();

    /*myView->Pan(thePoint.x() - myXMax, myYMax - thePoint.y());
    myXMax = thePoint.x();
    myYMax = thePoint.y()*/;

    if (myCurrentMode == CurAction3d_Nothing)
        myView->StartRotation(thePoint.x(), thePoint.y());

}

void AeroView::onMButtonDown(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    // Save the current mouse coordinate in min.
    myXMin = thePoint.x();
    myYMin = thePoint.y();
    myXMax = thePoint.x();
    myYMax = thePoint.y();

    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void AeroView::onRButtonDown(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);
    emit rightButtonClicked(thePoint);
}

void AeroView::onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void AeroView::onLButtonUp(const int theFlags, const QPoint thePoint)
{
    // Hide the QRubberBand
    /*if (myRectBand)
    {
      myRectBand->hide();
    }*/

    // Ctrl for multi selection.
    if (thePoint.x() == myXMin && thePoint.y() == myYMin)
    {
        myXMax = thePoint.x();
        myYMax = thePoint.y();
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }
    else
    {
        myXMax = thePoint.x();
        myYMax = thePoint.y();
    }

}

void AeroView::onMButtonUp(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    if (thePoint.x() == myXMin && thePoint.y() == myYMin)
    {
        panByMiddleButton(thePoint);
    }
}

void AeroView::onRButtonUp(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    popup(thePoint.x(), thePoint.y());
}

void AeroView::onMouseMove(const int theFlags, const QPoint thePoint)
{

    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    if (theFlags & Qt::MidButton)
    {
        switch (myCurrentMode)
        {
        case CurAction3d_Nothing:
            myView->Pan(thePoint.x() - myXMax, myYMax - thePoint.y());
            myXMax = thePoint.x();
            myYMax = thePoint.y();
            break;
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            myView->Redraw();
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXMin, myYMin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXMax, myYMax - thePoint.y());
            myXMax = thePoint.x();
            myYMax = thePoint.y();
            break;


        default:
            break;
        }
    }
    if (theFlags & Qt::LeftButton) {

        switch (myCurrentMode)
        {
        case CurAction3d_Nothing:
            myView->Rotation(thePoint.x(), thePoint.y());
            myView->Redraw();
            break;
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            myView->Redraw();
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXMin, myYMin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXMax, myYMax - thePoint.y());
            myXMax = thePoint.x();
            myYMax = thePoint.y();
            break;

        default:
            break;
        }
    }
}

void AeroView::popup(const int x, const int y)
{
    if (!myContextMenu)
        myContextMenu = new QMenu(this);
    else
        myContextMenu->clear();

    createSelectionLevelMenu();
    myContextMenu->addMenu(mySelectionLevelMenu);

    myContextMenu->exec(QCursor::pos());

}

void AeroView::inputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(true);

    //emit selectionChanged();
}

void AeroView::multiInputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(true);

    //emit selectionChanged();
}

void AeroView::moveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, true);
}

void AeroView::multiMoveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, true);
}

void AeroView::panByMiddleButton(const QPoint& thePoint)
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void AeroView::createSelectionLevelMenu()
{
    if (!mySelectionLevelMenu)
        mySelectionLevelMenu = new QMenu(this);
    else
        mySelectionLevelMenu->clear();

    mySelectionLevelMenu->setTitle(tr("&Selection Level"));

    QAction* action = new QAction(QObject::tr("Compound"), this);
    action->setToolTip(QObject::tr("Compound"));
    action->setStatusTip(QObject::tr("Compound"));
    connect(action, SIGNAL(triggered()), this, SLOT(onCompound()));
    mySelectionLevelMenu->addAction(action);
    if (selectionMode == TopAbs_COMPOUND)
        action->setDisabled(true);

    action = new QAction(QObject::tr("Vertex"), this);
    action->setToolTip(QObject::tr("Vertex"));
    action->setStatusTip(QObject::tr("Vertex"));
    connect(action, SIGNAL(triggered()), this, SLOT(onVertex()));
    mySelectionLevelMenu->addAction(action);
    if (selectionMode == TopAbs_VERTEX)
        action->setDisabled(true);

    action = new QAction(QObject::tr("Edge"), this);
    action->setToolTip(QObject::tr("Edge"));
    action->setStatusTip(QObject::tr("Edge"));
    connect(action, SIGNAL(triggered()), this, SLOT(onEdge()));
    mySelectionLevelMenu->addAction(action);
    if (selectionMode == TopAbs_EDGE)
        action->setDisabled(true);

    action = new QAction(QObject::tr("Wire"), this);
    action->setToolTip(QObject::tr("Wire"));
    action->setStatusTip(QObject::tr("Wire"));
    connect(action, SIGNAL(triggered()), this, SLOT(onWire()));
    mySelectionLevelMenu->addAction(action);
    if (selectionMode == TopAbs_WIRE)
        action->setDisabled(true);

    action = new QAction(QObject::tr("Face"), this);
    action->setToolTip(QObject::tr("Face"));
    action->setStatusTip(QObject::tr("Face"));
    connect(action, SIGNAL(triggered()), this, SLOT(onFace()));
    mySelectionLevelMenu->addAction(action);
    if (selectionMode == TopAbs_FACE)
        action->setDisabled(true);
}

void AeroView::onCompound()
{
    myContext->Deactivate();
    myContext->Activate(AIS_Shape::SelectionMode(TopAbs_COMPOUND));
    selectionMode = TopAbs_COMPOUND;
}

void AeroView::onVertex()
{
    myContext->Deactivate();
    myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
    selectionMode = TopAbs_VERTEX;
}

void AeroView::onEdge()
{
    myContext->Deactivate();
    myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
    selectionMode = TopAbs_EDGE;
}

void AeroView::onWire()
{
    myContext->Deactivate();
    myContext->Activate(AIS_Shape::SelectionMode(TopAbs_WIRE));
    selectionMode = TopAbs_WIRE;
}

void AeroView::onFace()
{
    myContext->Deactivate();
    myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
    selectionMode = TopAbs_FACE;
}



