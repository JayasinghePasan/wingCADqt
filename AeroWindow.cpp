//Aero
#include "ui_AeroWindow.h"
#include "AeroView.h"
#include "AeroDocument.h"
#include "AeroCoolTools.h"
#include "AeroWindow.h"
#include "AeroDrillBoxDialog.h"

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

AeroWindow::AeroWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::AeroWindow)
{
	ui->setupUi(this);
	myView = new AeroView();
	myDocument = new AeroDocument();
	setCentralWidget(myView);
	createAction();
	createMenu();
}

AeroWindow::~AeroWindow()
{
	delete ui;
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

	frontAction = new QAction(tr("Front"), this);
	frontAction->setStatusTip(tr("Front View"));
	connect(frontAction, SIGNAL(triggered()), myView, SLOT(front()));

	backAction = new QAction(tr("Back"), this);
	backAction->setStatusTip(tr("Back View"));
	connect(backAction, SIGNAL(triggered()), myView, SLOT(back()));

	topAction = new QAction(tr("Top"), this);
	topAction->setStatusTip(tr("Top View"));
	connect(topAction, SIGNAL(triggered()), myView, SLOT(top()));

	bottomAction = new QAction(tr("Bottom"), this);
	bottomAction->setStatusTip(tr("Bottom View"));
	connect(bottomAction, SIGNAL(triggered()), myView, SLOT(bottom()));

	leftAction = new QAction(tr("Left"), this);
	leftAction->setStatusTip(tr("Left View"));
	connect(leftAction, SIGNAL(triggered()), myView, SLOT(left()));

	rightAction = new QAction(tr("Right"), this);
	rightAction->setStatusTip(tr("Right View"));
	connect(rightAction, SIGNAL(triggered()), myView, SLOT(right()));

	zoomAction = new QAction(tr("Zoom"), this);
	zoomAction->setStatusTip(tr("Zooming"));
	connect(zoomAction, SIGNAL(triggered()), myView, SLOT(zoom()));

	panAction = new QAction(tr("Pan"), this);
	panAction->setStatusTip(tr("Pan Objects"));
	connect(panAction, SIGNAL(triggered()), myView, SLOT(pan()));

	rotateAction = new QAction(tr("Rotation"), this);
	rotateAction->setStatusTip(tr("Rotate Object"));
	connect(rotateAction, SIGNAL(triggered()), myView, SLOT(rotation()));

	//Primitive Actions
	makeSphereAction = new QAction(tr("Sphere"), this);
	makeSphereAction->setStatusTip(tr("Make a Sphere"));
	connect(makeSphereAction, SIGNAL(triggered()), this, SLOT(onMakeSphere()));

	//Sum Action
	sumAction = new QAction(tr("Recursive Summation"), this);
	sumAction->setToolTip(tr("Add trangle numbers"));
	connect(sumAction, SIGNAL(triggered()), this, SLOT(onSumSlot()));

	//listMenuBoundAction
	listMenuBoundAction = new QAction(tr("5.Find Boundries"), this);
	listMenuBoundAction->setToolTip(tr("Show upper and lower bound"));
	connect(listMenuBoundAction, SIGNAL(triggered()), this, SLOT(onListMenuBoundSlot()));

	//listMenuOccuranceAction
	listMenuOccuranceAction = new QAction(tr("6.Occurance Cal"), this);
	listMenuOccuranceAction->setToolTip(tr("Show upper and lower bound"));
	connect(listMenuOccuranceAction, SIGNAL(triggered()), this, SLOT(onListMenuOccuranceSlot()));

	//listMenuBubbleSortAction
	listMenuBubbleSortAction = new QAction(tr("7.Bubble Sort"), this);
	listMenuBubbleSortAction->setToolTip(tr("Sort a list of numbers seperated by spaces"));
	connect(listMenuBubbleSortAction, SIGNAL(triggered()), this, SLOT(onListMenuBubbleSortSlot()));

	//listMenuQuickSortAction
	listMenuQuickSortAction = new QAction(tr("8.Quick Sort"), this);
	listMenuQuickSortAction->setToolTip(tr("Sort a list of numbers seperated by spaces"));
	connect(listMenuQuickSortAction, SIGNAL(triggered()), this, SLOT(onListMenuQuickSortSlot()));
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
	viewMenu->addAction(zoomAction);
	viewMenu->addAction(panAction);
	viewMenu->addAction(rotateAction);

	primitiveMenu = menuBar()->addMenu(tr("&Primitive"));
	primitiveMenu->addAction(makeSphereAction);

	sumMenu = menuBar()->addMenu(tr("&Summation"));
	sumMenu->addAction(sumAction);

	listMenu = menuBar()->addMenu(tr("&List Op"));
	listMenu->addAction(listMenuBoundAction);
	listMenu->addAction(listMenuOccuranceAction);
	listMenu->addAction(listMenuBubbleSortAction);
	listMenu->addAction(listMenuQuickSortAction);

	BottleMenuAction = menuBar()->addAction(tr("&Bottle"));
	connect(BottleMenuAction, SIGNAL(triggered()), this, SLOT(onMakeBottleSlot()));

	DrillBoxAction = menuBar()->addAction(tr("&Drill Box"));
	connect(DrillBoxAction, SIGNAL(triggered()), this, SLOT(onMakeDrillBoxSlot()));
}

void AeroWindow::exit(void)
{
	this->close();
}


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

void AeroWindow::onSumSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelNumber = new QLabel(tr("&Number:"));
	QSpinBox* spinBoxNumber = new QSpinBox();
	spinBoxNumber->setRange(1, INT_MAX);
	spinBoxNumber->setValue(10.0);
	labelNumber->setBuddy(spinBoxNumber);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelNumber, 0, 0);
	gridLayout->addWidget(spinBoxNumber, 0, 1);

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
		int number = spinBoxNumber->value();
		double total = 0;

		for (int i = 1; i <= number; i++)
		{
			total += (i / 2.0 * (i + 1));
		}
		QString outputText = "Sum =" + QString::number(total);
		QMessageBox outputMsgBox;
		outputMsgBox.setText(outputText);
		outputMsgBox.exec();
	}
}

void AeroWindow::onListMenuBoundSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelText = new QLabel(tr("&Number List:"));
	QLineEdit* lineEditText = new QLineEdit();
	labelText->setBuddy(lineEditText);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelText, 0, 0);
	gridLayout->addWidget(lineEditText, 0, 1);

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
		QString inputText = lineEditText->text();
		QString seperator = " ";
		QStringList listedNumbers = inputText.split(" ");
		QVector<double> numberVector;


		for (int i = 0; i < listedNumbers.size(); i++)
		{
			numberVector.push_back(listedNumbers[i].toDouble());
		}

		double max = numberVector[0];
		double min = numberVector[0];

		for (int i = 0; i < numberVector.size(); i++)
		{
			if (numberVector[i] > max)
			{
				max = numberVector[i];
			}
			if (numberVector[i] < min)
			{
				min = numberVector[i];
			}
		}

		QString outputText = "Max=" + QString::number(max) + "  and  " + "Min=" + QString::number(min);
		QMessageBox outputMsgBox;
		outputMsgBox.setText(outputText);
		outputMsgBox.exec();
	}
}

void AeroWindow::onListMenuOccuranceSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelText = new QLabel(tr("&Number List:"));
	QLineEdit* lineEditText = new QLineEdit();
	labelText->setBuddy(lineEditText);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelText, 0, 0);
	gridLayout->addWidget(lineEditText, 0, 1);

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
		QString inputText = lineEditText->text();
		QString seperator = " ";
		QStringList listedNumbers = inputText.split(" ");
		QVector<double> numberVector;


		for (int i = 0; i < listedNumbers.size(); i++)
		{
			numberVector.push_back(listedNumbers[i].toDouble());
		}
		int counter[10] = { 0 };

		for (int i = 0; i < numberVector.size(); i++)
		{
			if (numberVector[i] == 0.0)
			{
				counter[0] += 1;
			}
			else if (numberVector[i] == 1.0)
			{
				counter[1] += 1;
			}
			else if (numberVector[i] == 2.0)
			{
				counter[2] += 1;
			}
			else if (numberVector[i] == 3.0)
			{
				counter[3] += 1;
			}
			else if (numberVector[i] == 4.0)
			{
				counter[4] += 1;
			}
			else if (numberVector[i] == 5.0)
			{
				counter[5] += 1;
			}
			else if (numberVector[i] == 6.0)
			{
				counter[6] += 1;
			}
			else if (numberVector[i] == 7.0)
			{
				counter[7] += 1;
			}
			else if (numberVector[i] == 8.0)
			{
				counter[8] += 1;
			}
			else if (numberVector[i] == 9.0)
			{
				counter[9] += 1;
			}

		}
		int indexmax = 0;
		int indexmin = 0;
		double max = numberVector[0];
		double min = numberVector[0];

		for (int i = 0; i < 10; i++)
		{
			if (counter[i] > max)
			{
				max = counter[i];
				indexmax = i;
			}
			if (counter[i] < min)
			{
				min = counter[i];
				indexmin = i;
			}
		}


		QString outputText = "Most=" + QString::number(indexmax) + "  and  " + "Least=" + QString::number(indexmin);
		QMessageBox outputMsgBox;
		outputMsgBox.setText(outputText);
		outputMsgBox.exec();

	}
}

void AeroWindow::onListMenuBubbleSortSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelText = new QLabel(tr("&Number List:"));
	QLineEdit* lineEditText = new QLineEdit();
	labelText->setBuddy(lineEditText);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelText, 0, 0);
	gridLayout->addWidget(lineEditText, 0, 1);

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
		QString inputText = lineEditText->text();
		QString seperator = " ";
		QStringList listedNumbers = inputText.split(" ");
		QVector<double> numberVector;


		for (int i = 0; i < listedNumbers.size(); i++)
		{
			numberVector.push_back(listedNumbers[i].toDouble());
		}


		for (int i = 0; i < numberVector.size(); i++)
		{
			for (int i = 0; i < numberVector.size() - 1; i++)
			{
				if (numberVector[i] > numberVector[i + 1])
				{
					double tempHolder = numberVector[i + 1];
					numberVector[i + 1] = numberVector[i];
					numberVector[i] = tempHolder;
				}
			}
		}

		QString outputTextAscending;
		QString	outputTextDscending;

		for (int i = 0; i < numberVector.size(); i++)
		{
			outputTextAscending += QString::number(numberVector[i]) + " ";
			outputTextDscending += QString::number(numberVector[(numberVector.size() - 1) - i]) + " ";
		}
		QString  outputText = "Ascending : " + outputTextAscending + "\n" + "Dscending : " + outputTextDscending;
		QMessageBox outputMsgBox;
		outputMsgBox.setText(outputText);
		outputMsgBox.exec();

	}
}

void AeroWindow::onListMenuQuickSortSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelText = new QLabel(tr("&Number List:"));
	QLineEdit* lineEditText = new QLineEdit();
	labelText->setBuddy(lineEditText);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelText, 0, 0);
	gridLayout->addWidget(lineEditText, 0, 1);

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
		QString inputText = lineEditText->text();
		QString seperator = " ";
		QStringList listedNumbers = inputText.split(" ");
		QVector<double> numberVector;


		for (int i = 0; i < listedNumbers.size(); i++)
		{
			numberVector.push_back(listedNumbers[i].toDouble());
		}


		double pivot = numberVector[0];


		QString outputTextAscending;
		QString	outputTextDscending;

		for (int i = 0; i < numberVector.size(); i++)
		{
			outputTextAscending += QString::number(numberVector[i]) + " ";
			outputTextDscending += QString::number(numberVector[(numberVector.size() - 1) - i]) + " ";
		}
		QString  outputText = "Ascending : " + outputTextAscending + "\n" + "Dscending : " + outputTextDscending;
		QMessageBox outputMsgBox;
		outputMsgBox.setText(outputText);
		outputMsgBox.exec();

	}
}

void AeroWindow::onMakeBottleSlot()
{
	//Create a dialog box to get input from users
	QDialog* inputDialog = new QDialog();

	QLabel* labelHeight = new QLabel(tr("&Height:"));
	QDoubleSpinBox* heightInput = new QDoubleSpinBox();
	heightInput->setValue(600.0);
	heightInput->setRange(1.0e-5, DBL_MAX);
	labelHeight->setBuddy(heightInput);

	QLabel* labelWidth = new QLabel(tr("&Width:"));
	QDoubleSpinBox* widthInput = new QDoubleSpinBox();
	widthInput->setValue(30.0);
	widthInput->setRange(1.0e-5, DBL_MAX);
	labelWidth->setBuddy(widthInput);

	QLabel* labelThickness = new QLabel(tr("&Thickness:"));
	QDoubleSpinBox* thicknessInput = new QDoubleSpinBox();
	thicknessInput->setValue(20.0);
	thicknessInput->setRange(1.0e-5, DBL_MAX);
	labelThickness->setBuddy(thicknessInput);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(labelHeight, 0, 0);
	gridLayout->addWidget(heightInput, 0, 1);

	gridLayout->addWidget(labelWidth, 1, 0);
	gridLayout->addWidget(widthInput, 1, 1);

	gridLayout->addWidget(labelThickness, 2, 0);
	gridLayout->addWidget(thicknessInput, 2, 1);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(inputDialog);
	//buttonBox->setBaseSize(QSize(200, 0));
	buttonBox->setLayoutDirection(Qt::LeftToRight);
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

	gridLayout->addWidget(buttonBox, 3, 1);
	inputDialog->setLayout(gridLayout);

	QObject::connect(buttonBox, SIGNAL(accepted()), inputDialog, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), inputDialog, SLOT(reject()));

	if (inputDialog->exec() == QDialog::Accepted)
	{
		double bottleHeight = heightInput->value();
		double bottleWidth = widthInput->value();
		double bottleThickness = thicknessInput->value();

		/*
		profile point map
					   |
					   |								 Thickness
		---1-----------+-----------2-----					^
		---5-----------|-----------3-----					|
		---------------4-----------------				   -|---> Width
		*/

		gp_Pnt profilePnt1(-0.5 * bottleWidth, 0, 0);
		gp_Pnt profilePnt2(+0.5 * bottleWidth, 0, 0);
		gp_Pnt profilePnt3(+0.5 * bottleWidth, -0.25 * bottleThickness, 0);
		gp_Pnt profilePnt4(0, -0.5 * bottleThickness, 0);
		gp_Pnt profilePnt5(-0.5 * bottleWidth, -0.25 * bottleThickness, 0);

		//creating the segment at X negative side
		Handle(Geom_TrimmedCurve) segmentAtXnegative;
		GC_MakeSegment tempSegmentAtXnegative = GC_MakeSegment(profilePnt2, profilePnt3);
		if (tempSegmentAtXnegative.IsDone())
		{
			segmentAtXnegative = tempSegmentAtXnegative.Value();
		}
		else
		{
			std::cout << "Segment At X negative Failed" << endl;
		}

		//creating the segment at X positive side
		Handle(Geom_TrimmedCurve) segmentAtXpositive;
		GC_MakeSegment tempSegmentAtXpositive = GC_MakeSegment(profilePnt1, profilePnt5);
		if (tempSegmentAtXpositive.IsDone())
		{
			segmentAtXpositive = tempSegmentAtXpositive.Value();
		}
		else
		{
			std::cout << "Segment At X positive Failed" << endl;
		}

		//creating the ARC
		Handle(Geom_TrimmedCurve) segmentArc;
		GC_MakeArcOfCircle tempSegmentArc = GC_MakeArcOfCircle(profilePnt5, profilePnt4, profilePnt3);
		if (tempSegmentArc.IsDone())
		{
			segmentArc = tempSegmentArc.Value();
		}
		else
		{
			std::cout << "Circular Arc Failed" << endl;
		}

		//making edges out of segments
		TopoDS_Edge edgeAtXnegative = BRepBuilderAPI_MakeEdge(segmentAtXnegative);
		TopoDS_Edge edgeAtXpositive = BRepBuilderAPI_MakeEdge(segmentAtXpositive);
		TopoDS_Edge edgeArc = BRepBuilderAPI_MakeEdge(segmentArc);

		//making a wire out of edges
		TopoDS_Wire halfProfileWire = BRepBuilderAPI_MakeWire(edgeAtXnegative, edgeArc, edgeAtXpositive);

		//defining the symmetric axis
		gp_Pnt originPnt(0, 0, 0);
		gp_Dir xDirection(1, 0, 0);
		gp_Ax1 profileSymmetricAxis(originPnt, xDirection);

		//seting up mirror axis
		gp_Trsf mirrorAction;
		mirrorAction.SetMirror(profileSymmetricAxis);

		//mirror transformation
		BRepBuilderAPI_Transform mirrorTransform(halfProfileWire, mirrorAction);

		//making the mirror shape
		TopoDS_Shape mirrorShape = mirrorTransform.Shape();

		//making mirror wire out of shape
		TopoDS_Wire mirroredWire = TopoDS::Wire(mirrorShape);

		//creating a complete single wire
		BRepBuilderAPI_MakeWire completeProfileWireBuilder;
		completeProfileWireBuilder.Add(halfProfileWire);
		completeProfileWireBuilder.Add(mirroredWire);
		TopoDS_Wire completeProfileWire = completeProfileWireBuilder.Wire();

		//making the bottom face
		TopoDS_Face bottomFace = BRepBuilderAPI_MakeFace(completeProfileWire);

		//making the body using "Prism"
		gp_Vec bottleBodyPrismVector(0, 0, bottleHeight);
		TopoDS_Shape bottleBodyPrism = BRepPrimAPI_MakePrism(bottomFace, bottleBodyPrismVector);

		//adding fillet
		BRepFilletAPI_MakeFillet makeEdgeFillet(bottleBodyPrism);
		TopExp_Explorer edgeExplorer(bottleBodyPrism, TopAbs_EDGE);
		while (edgeExplorer.More())
		{
			TopoDS_Edge tempEdge = TopoDS::Edge(edgeExplorer.Current());
			makeEdgeFillet.Add(bottleThickness / 12, tempEdge);
			edgeExplorer.Next();
		}
		bottleBodyPrism = makeEdgeFillet.Shape();

		//creating the neck 

		//defining neck axis system
		gp_Pnt neckBottomPnt(0, 0, bottleHeight);
		gp_Dir neckAxisDirection = gp::DZ();
		gp_Ax2 neckAxisSystem(neckBottomPnt, neckAxisDirection);

		//creating the cylinder
		double neckRadius = Min(bottleThickness, bottleWidth) / 4.0;
		double neckHeight = bottleHeight / 10.0;
		BRepPrimAPI_MakeCylinder neckCylinderBuilder(neckAxisSystem, neckRadius, neckHeight);
		TopoDS_Shape neckCylinder = neckCylinderBuilder.Shape();
		bottleBodyPrism = BRepAlgoAPI_Fuse(bottleBodyPrism, neckCylinder);

		//creating the hollow bottle
		TopTools_ListOfShape topFace;
		TopoDS_Face faceToRemove;

		for (TopExp_Explorer faceExplorer(bottleBodyPrism, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
		{
			TopoDS_Face tempFace = TopoDS::Face(faceExplorer.Current());
			Handle(Geom_Surface) tempSurface = BRep_Tool::Surface(tempFace);
			if (tempSurface->DynamicType() == STANDARD_TYPE(Geom_Plane))
			{
				Handle(Geom_Plane) tempPlane = Handle(Geom_Plane)::DownCast(tempSurface);
				double zFilter = -1.0;
				if (tempPlane->Location().Z() > zFilter)
				{
					zFilter = tempPlane->Location().Z();
					faceToRemove = tempFace;
				}
			}
		}

		topFace.Append(faceToRemove);
		BRepOffsetAPI_MakeThickSolid hollowBuilder;
		hollowBuilder.MakeThickSolidByJoin(bottleBodyPrism, topFace, -bottleThickness / 50, 1e-7);
		bottleBodyPrism = hollowBuilder.Shape();

		//Creating the thread 
		//creating 2 cylindrical surfaces
		Handle(Geom_CylindricalSurface) outerThreadSurface = new Geom_CylindricalSurface(neckAxisSystem, neckRadius * 1.05);
		Handle(Geom_CylindricalSurface) innerThreadSurface = new Geom_CylindricalSurface(neckAxisSystem, neckRadius * 0.99);

		gp_Pnt2d elypsePnt(2.0 * M_PI, neckHeight / 2.0);
		gp_Dir2d elypseDir(2.0 * M_PI, neckHeight / 8.0);
		gp_Ax2d elypseAxis(elypsePnt, elypseDir);

		Standard_Real elypseMinRadius = neckHeight / 10;
		Standard_Real elypseMaxRadius = 6 * M_PI;
		Handle(Geom2d_Ellipse) elipse1 = new Geom2d_Ellipse(elypseAxis, elypseMaxRadius, elypseMinRadius);
		Handle(Geom2d_Ellipse) elipse2 = new Geom2d_Ellipse(elypseAxis, elypseMaxRadius, elypseMinRadius / 2);

		Handle(Geom2d_TrimmedCurve) elypse1TrimArc = new Geom2d_TrimmedCurve(elipse1, 0, M_PI);
		Handle(Geom2d_TrimmedCurve) elypse2TrimArc = new Geom2d_TrimmedCurve(elipse2, 0, M_PI);

		gp_Pnt2d elipsePnt1 = elipse1->Value(0);
		gp_Pnt2d elipsePnt2 = elipse1->Value(M_PI);

		Handle(Geom2d_TrimmedCurve) elipseSegment = GCE2d_MakeSegment(elipsePnt1, elipsePnt2);

		// Threading : Build Edges and Wires
		TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(elypse1TrimArc, innerThreadSurface);
		TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(elipseSegment, innerThreadSurface);
		TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(elypse2TrimArc, outerThreadSurface);
		TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(elipseSegment, outerThreadSurface);
		TopoDS_Wire threadWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
		TopoDS_Wire threadWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
		BRepLib::BuildCurves3d(threadWire1);
		BRepLib::BuildCurves3d(threadWire2);

		// Create Threading 
		BRepOffsetAPI_ThruSections ThreadTool(Standard_True);
		ThreadTool.AddWire(threadWire1);
		ThreadTool.AddWire(threadWire2);
		ThreadTool.CheckCompatibility(Standard_False);
		TopoDS_Shape threadShape = ThreadTool.Shape();

		// Building the Resulting Compound 
		TopoDS_Compound bottleCompound;
		BRep_Builder fullBottleBuilder;
		fullBottleBuilder.MakeCompound(bottleCompound);
		fullBottleBuilder.Add(bottleCompound, bottleBodyPrism);
		fullBottleBuilder.Add(bottleCompound, threadShape);




		myView->clearView();
		myView->displayShape(bottleCompound, Quantity_NOC_YELLOW);
		myView->fitAll();

	}
}

void AeroWindow::onMakeDrillBoxSlot()
{



}
