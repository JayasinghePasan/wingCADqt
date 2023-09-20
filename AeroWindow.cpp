//Aero
#include "ui_AeroWindow.h"
#include "AeroView.h"
#include "AeroDocument.h"
#include "AeroWindow.h"

//OCCT
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>
#include <ApproxInt_KnotTools.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <gp_Pnt2d.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <gp_Circ.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <Standard_DefineHandle.hxx>
#include <Geom_BSplineCurve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Geom_Curve.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <Geom_Ellipse.hxx>
#include <gp_Elips.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <GC_MakeEllipse.hxx>
#include <TNaming_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_Color.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_ChildIterator.hxx>

//QT
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QVector>
#include <QLineEdit>
#include <QApplication>
#include <QProcess>
#include <qvector.h>
#include <qwidget.h>
#include <QTime>

AeroWindow::AeroWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::AeroWindow)
{
	ui->setupUi(this);
	myView = new AeroView();           //For CAD window 
	myDocument = new AeroDocument();   //For Inputs 

	// Creates a horizontal layout
	//  ___________________________________________
	// |                       |                   |
	// |   AeroView::          |  AeroDocument::   |
	// |    myView             |   myDomucent      |
	// |    CAD                |   Inputs          |
	// |_______________________|___________________|
	//  <----------3----------><---------1--------->
	
	QHBoxLayout* layout = new QHBoxLayout(centralWidget());
	layout->addWidget(myView, 3);       // 3 is the stretch factor
	layout->addWidget(myDocument, 1);

	centralWidget()->setLayout(layout);  // Apply the layout to the central widget

	// Connecting Signals given by myDocument pushButtons with SLOTS of myView to trigger CAD display area
	connect(myDocument, &AeroDocument::requestBuild, this, &AeroWindow::handleBuildRequest);
	connect(myDocument, &AeroDocument::requestExport, this, &AeroWindow::handleExportRequest);

	createAction();
	createMenu();
	
}


AeroWindow::~AeroWindow()
{
    delete ui;
}



void AeroWindow::createMenu(void)
{
	// menubar menu 1 -> File
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);   // File->Exit

	// menubar menu 2 -> View 
	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(fitAllAction); // View->Fit to All
	viewMenu->addAction(frontAction);  // View->Front
	viewMenu->addAction(backAction);   // View->Back
	viewMenu->addAction(topAction);    // View->Top
	viewMenu->addAction(bottomAction); // View->Bottom
	viewMenu->addAction(leftAction);   // View->Left
	viewMenu->addAction(rightAction);  // View->Right

	// menubar menu 3 -> Examples
	exampleMenu = menuBar()->addMenu(tr("&Examples"));
	exampleMenu->addAction(wingAction);        //NACA 0016

}

void AeroWindow::createAction(void)
{
	// Exi Action
	exitAction = new QAction(tr("Exit"), this);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

	// View Actions
	// Name and the Slot name is differently connected to get the Std view of a wing
	fitAllAction = new QAction(tr("FitAll"), this);
	connect(fitAllAction, SIGNAL(triggered()), myView, SLOT(fitAll()));

	frontAction = new QAction(tr("Back"), this);
	connect(frontAction, SIGNAL(triggered()), myView, SLOT(front()));

	backAction = new QAction(tr("Front"), this);
	connect(backAction, SIGNAL(triggered()), myView, SLOT(back()));

	topAction = new QAction(tr("Left"), this);
	connect(topAction, SIGNAL(triggered()), myView, SLOT(top()));

	bottomAction = new QAction(tr("Right"), this);
	connect(bottomAction, SIGNAL(triggered()), myView, SLOT(bottom()));

	leftAction = new QAction(tr("Top"), this);
	connect(leftAction, SIGNAL(triggered()), myView, SLOT(left()));

	rightAction = new QAction(tr("Bottom"), this);
	connect(rightAction, SIGNAL(triggered()), myView, SLOT(right()));



	//Example Actions
	wingAction = new QAction(tr("NACA 0016"), this);
	connect(wingAction, SIGNAL(triggered()), this, SLOT(onExampleWingSlot()));
}


// Slot Functions

// File->Exit
void AeroWindow::exit(void)
{
	this->close();
}


// creates a wingClass object and display it on the QOpenGlWidget
// default chord length and span length
void AeroWindow::onExampleWingSlot()
{
	double chord = 1.0;
	double span = 2.0;
	QString text = "";
	handleBuildRequest(chord, span, text);
}



// handles the request of AeroDocument::requestBuild() 
// creates a wingClass object and display it on the QOpenGlWidget
// @para chord = wing chord length
// @para span  = wing span length 
// @para textCoords = Coordinates pasted on the QTextEdit as a QString
// chord, span & textCoords will be given from AeroDocument
void AeroWindow::handleBuildRequest(double& chord, double& span, QString& textCoords)
{
	wingClass* thisWing = new wingClass(chord, span, textCoords);
	currentWing = thisWing;
	TopoDS_Shape wingShape = thisWing->getShape();
	myView->clearView();
	myView->displayShape(wingShape, Quantity_NOC_RED);
	myView->fitAll();

	// For status bar update
	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm:ss");

	// Split the textCoords to get the first line
	// Firstline usually contains the name of the aerofoil
	QStringList lines = textCoords.split("\n");
	QString firstLine = lines.isEmpty() ? "" : lines.first();

	// Create the status message
	QString statusMessage = QString("%1       Chord = %2 mm  Span = %3 mm                              Created at : %4")
		.arg(firstLine)
		.arg(chord)
		.arg(span)
		.arg(timeString);

	statusBar()->showMessage(statusMessage);
}



// handles the request of AeroDocument::requestExport() 
// Exports the displayed wing to Exports directory in project folder    // *** Currently nly work in debug mode *** //
// @para index = CAD format (0-.stp , 1-.brep , 2-.iges)
// @para filename  = output file name 
// index & filename will be given from AeroDocument
void AeroWindow::handleExportRequest(int& index, QString& filename)
{
	currentWing->ExportFile(filename.toStdString(), index);
}



