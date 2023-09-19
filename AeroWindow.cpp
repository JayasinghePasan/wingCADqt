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
	myView = new AeroView();
	myDocument = new AeroDocument();

	// Create a horizontal layout
	QHBoxLayout* layout = new QHBoxLayout(centralWidget());
	layout->addWidget(myView, 3);  // 1 is the stretch factor
	layout->addWidget(myDocument, 1);

	centralWidget()->setLayout(layout);  // Apply the layout to the central widget

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
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(fitAllAction);
	viewMenu->addAction(frontAction);
	viewMenu->addAction(backAction);
	viewMenu->addAction(topAction);
	viewMenu->addAction(bottomAction);
	viewMenu->addAction(leftAction);
	viewMenu->addAction(rightAction);


	primitiveMenu = menuBar()->addMenu(tr("&Examples"));
	primitiveMenu->addAction(makeSphereAction);
	primitiveMenu->addAction(wingAction);

}

void AeroWindow::createAction(void)
{
	//Exi Action
	exitAction = new QAction(tr("Exit"), this);
	exitAction->setStatusTip(tr("Eixt the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

	//View Actionss
	fitAllAction = new QAction(tr("FitAll"), this);
	fitAllAction->setStatusTip(tr("Fit All"));
	connect(fitAllAction, SIGNAL(triggered()), myView, SLOT(fitAll()));

	frontAction = new QAction(tr("Back"), this);
	frontAction->setStatusTip(tr("Back View"));
	connect(frontAction, SIGNAL(triggered()), myView, SLOT(front()));

	backAction = new QAction(tr("Front"), this);
	backAction->setStatusTip(tr("Front View"));
	connect(backAction, SIGNAL(triggered()), myView, SLOT(back()));

	topAction = new QAction(tr("Left"), this);
	topAction->setStatusTip(tr("Left View"));
	connect(topAction, SIGNAL(triggered()), myView, SLOT(top()));

	bottomAction = new QAction(tr("Right"), this);
	bottomAction->setStatusTip(tr("Right View"));
	connect(bottomAction, SIGNAL(triggered()), myView, SLOT(bottom()));

	leftAction = new QAction(tr("Top"), this);
	leftAction->setStatusTip(tr("Top View"));
	connect(leftAction, SIGNAL(triggered()), myView, SLOT(left()));

	rightAction = new QAction(tr("Bottom"), this);
	rightAction->setStatusTip(tr("Bottom View"));
	connect(rightAction, SIGNAL(triggered()), myView, SLOT(right()));



	//Primitive Actions
	makeSphereAction = new QAction(tr("Sphere"), this);
	makeSphereAction->setStatusTip(tr("Make a Sphere"));
	connect(makeSphereAction, SIGNAL(triggered()), this, SLOT(onMakeSphere()));

	wingAction = new QAction(tr("Sample Wing NACA0016"), this);
	wingAction->setStatusTip(tr("Make a Wing"));
	connect(wingAction, SIGNAL(triggered()), this, SLOT(onExampleWingSlot()));
}


// Slot Functions

// File->Exit - Close the program window
void AeroWindow::exit(void)
{
	this->close();
}


// creates a wingClass object and display it on the QOpenGlWidget
// chord length, span length, save file name will be taken from the UI
void AeroWindow::onExampleWingSlot()
{
	
	double chord = 1.0;
	double span = 2.0;
	wingClass thiswing(chord, span, "");
	TopoDS_Shape wingShape = thiswing.getShape();
	myView->clearView();
	myView->displayShape(wingShape, Quantity_NOC_YELLOW);
	myView->fitAll();

	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm:ss");  // format the time as hours:minutes:seconds
	statusBar()->showMessage(QString("Example Wing  NACA 0016   Chord = 1.0 mm  Span = 2.0 mm                              Created at : %1").arg(timeString));
	
	}


// creates a ball
void AeroWindow::onMakeSphere()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelRadius = new QLabel(tr("&Radius:"));
	QDoubleSpinBox* spinBoxRadius = new QDoubleSpinBox();
	spinBoxRadius->setRange(1.0e-7, DBL_MAX);
	spinBoxRadius->setValue(10.0);
	labelRadius->setBuddy(spinBoxRadius);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelRadius, 0, 0);
	gridLayout->addWidget(spinBoxRadius, 0, 1);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(inputDialog);
	//buttonBox->setBaseSize(QSize(200, 0));
	buttonBox->setLayoutDirection(Qt::LeftToRight);
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

	gridLayout->addWidget(buttonBox, 1, 1);
	inputDialog->setLayout(gridLayout);

	QObject::connect(buttonBox, SIGNAL(accepted()), inputDialog, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), inputDialog, SLOT(reject()));

	if (inputDialog->exec() == QDialog::Accepted)
	{
		double radius = spinBoxRadius->value();

		TopoDS_Shape shape;
		if (myDocument->makeSphere(shape, radius))
		{
			myView->clearView();
			myView->displayShape(shape, Quantity_NOC_YELLOW);
			myView->fitAll();
		}
	}
}



// handles the request from AeroDocument classBuild Button Slot 
// creates a wingClass object and display it on the QOpenGlWidget
// chord length, span length, save file name will be taken from the UI
void AeroWindow::handleBuildRequest(double& chord, double& span, QString& textCoords)
{
	wingClass* thisWing = new wingClass(chord, span, textCoords);
	currentWing = thisWing;
	TopoDS_Shape wingShape = thisWing->getShape();
	myView->clearView();
	myView->displayShape(wingShape, Quantity_NOC_RED);
	myView->fitAll();

	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm:ss");  // format the time as hours:minutes:seconds

	// Split the textCoords to get the first line
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

void AeroWindow::handleExportRequest(int& index, QString& filename)
{
	currentWing->ExportFile(filename.toStdString(), index);
}



