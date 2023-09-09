#ifndef OCCVIEWERWIDGET_H
#define OCCVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class OCCViewerWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OCCViewerWidget(QWidget* parent = nullptr);
    ~OCCViewerWidget();


    void displayShape(const TopoDS_Shape& shape);
    Handle(AIS_InteractiveContext) context() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Handle(V3d_Viewer) myViewer;
    Handle(AIS_InteractiveContext) myContext;
    Handle(V3d_View) myView;
};

#endif // OCCVIEWERWIDGET_H
