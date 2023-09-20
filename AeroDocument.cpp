//Aero
#include "AeroDocument.h"
#include "AeroWindow.h"
#include "wingClass.h"

//OCCT
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

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

AeroDocument::AeroDocument(QWidget* parent) : QWidget(parent)
{
    setupUi();
}

void AeroDocument::setupUi()
{
    spinBoxChordLength = new QDoubleSpinBox(this);
    spinBoxChordLength->setSuffix(" mm");
    spinBoxChordLength->setValue(1.00);

    spinBoxSpanLength  = new QDoubleSpinBox(this);
    spinBoxSpanLength->setSuffix(" mm");
    spinBoxSpanLength->setValue(2.00);
    
    labelChordLength = new QLabel(this);
    labelSpanLength  = new QLabel(this);
    labelFormat      = new QLabel(this);
    labelFilename    = new QLabel(this);

    labelChordLength->setText("Chord Length");
    labelSpanLength ->setText("Span Length");
    labelFormat     ->setText("Format");
    labelFilename   ->setText("Filename");
    
    pushButtonBuild  = new QPushButton("Build", this);
    pushButtonExport = new QPushButton("Export", this);

    comboBoxFormat = new QComboBox(this);
    comboBoxFormat->addItem(".step");
    comboBoxFormat->addItem(".iges");
    comboBoxFormat->addItem(".brep");
    comboBoxFormat->setItemText(0, ".step");
    comboBoxFormat->setItemText(1, ".iges");
    comboBoxFormat->setItemText(2, ".brep");
    comboBoxFormat->setCurrentIndex(0); // Selects the first item by default


    textEditCoord = new QTextEdit(this);
    textEditCoord->setPlaceholderText("Paste Coordinates here. \nFirst line will be skipped.");

    lineEditFileName = new QLineEdit(this);
    lineEditFileName->setPlaceholderText("Type filename here");
    


    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout;
    
    layout->addWidget(textEditCoord);
    
    QHBoxLayout* hLayoutExportChord = new QHBoxLayout;
    hLayoutExportChord->addWidget(labelChordLength);
    hLayoutExportChord->addWidget(spinBoxChordLength);
    layout->addLayout(hLayoutExportChord);

    QHBoxLayout* hLayoutExportSpan = new QHBoxLayout;
    hLayoutExportSpan->addWidget(labelSpanLength);
    hLayoutExportSpan->addWidget(spinBoxSpanLength);
    layout->addLayout(hLayoutExportSpan);

    layout->addWidget(pushButtonBuild);
    
    QHBoxLayout* hLayoutExportLabels = new QHBoxLayout;
    hLayoutExportLabels->addWidget(labelFilename,3);
    hLayoutExportLabels->addWidget(labelFormat);
    layout->addLayout(hLayoutExportLabels);

    QHBoxLayout* hLayoutExportTabs = new QHBoxLayout;
    hLayoutExportTabs->addWidget(lineEditFileName);
    hLayoutExportTabs->addWidget(comboBoxFormat);
    layout->addLayout(hLayoutExportTabs);
    
    layout->addWidget(pushButtonExport);

    setLayout(layout);

    // Connect buttons to slots
    connect(pushButtonBuild,  &QPushButton::clicked, this, &AeroDocument::onBuildButtonSlot);
    connect(pushButtonExport, &QPushButton::clicked, this, &AeroDocument::onExportButtonSlot);

}

// creates a wingClass object and display it on the QOpenGlWidget
// chord length, span length, save file name will be taken from the UI
void AeroDocument::onBuildButtonSlot()
{
    double chord = spinBoxChordLength->value();
    double span  = spinBoxSpanLength->value();
    QString textCoords = textEditCoord->toPlainText();
    emit requestBuild(chord, span, textCoords);
}


void AeroDocument::onExportButtonSlot()
{
    int index = comboBoxFormat->currentIndex();
    QString filename = lineEditFileName->text();
    emit requestExport(index, filename);
}

// Make a sphere with given radius
// @param sphereShape - Return generated box shape
// @param radius - Radius of the sphere
// @return true if sphere generation is success
bool AeroDocument::makeSphere(TopoDS_Shape& sphereShape, double radius)
{
    if (radius <= 0.0)
        return false;

    try
    {
        OCC_CATCH_SIGNALS
            sphereShape = BRepPrimAPI_MakeSphere(radius).Shape();
    }
    catch (Standard_Failure)
    {
        return false;
    }

    return true;
}
