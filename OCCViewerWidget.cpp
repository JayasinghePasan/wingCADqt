// OCCViewerWidget.cpp
#include "OCCViewerWidget.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_Shape.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

OCCViewerWidget::OCCViewerWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);
    myViewer = new V3d_Viewer(graphicDriver);
    myContext = new AIS_InteractiveContext(myViewer);
}

OCCViewerWidget::~OCCViewerWidget()
{
}

void OCCViewerWidget::initializeGL()
{
    myView = myViewer->CreateView();
}

void OCCViewerWidget::resizeGL(int width, int height)
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}

void OCCViewerWidget::paintGL()
{
    if (!myView.IsNull())
        myView->Redraw();
}

void OCCViewerWidget::displayShape(const TopoDS_Shape& shape)
{
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    myContext->Display(aisShape, Standard_True);
    myView->FitAll();
}

Handle(AIS_InteractiveContext) OCCViewerWidget::context() const
{
    return myContext;
}
