#ifndef AERODOCUMENT_H
#define AERODOCUMENT_H

//Aero
//OCCT
#include <TopoDS.hxx>
#include <TDocStd_Document.hxx>

//QT
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class AeroView;

class AeroDocument : public QWidget
{
    Q_OBJECT

public:
    AeroDocument(QWidget* parent = nullptr);

private:

    QDoubleSpinBox* spinBoxChordLength;
    QDoubleSpinBox* spinBoxSpanLength;
    QLabel* labelChordLength;
    QLabel* labelSpanLength;
    QLabel* labelFormat;
    QLabel* labelFilename;
    
    QPushButton* pushButtonBuild;
    QPushButton* pushButtonExport;

    QComboBox* comboBoxFormat;
    
    QTextEdit* textEditCoord;
    
    QLineEdit* lineEditFileName;
    

    void setupUi();


signals:
    void requestBuild(double& chord, double& span, int& index, QString& filename);


private slots:
    void onBuildButtonSlot();
    //void onExportButtonSlot();


public:
	bool AeroDocument::makeSphere(TopoDS_Shape& sphereShape, double radius);

};

#endif AERODOCUMENT_H

