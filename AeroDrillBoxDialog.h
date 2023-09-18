#ifndef AERODRILLBOXDIALOG_H
#define AERODRILLBOXDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <TopoDS_Face.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Geom_Plane.hxx>
#include <TopoDS_Edge.hxx>
#include <vector>
#include "AeroDocument.h"
#include "AeroView.h"

class AeroDrillBoxDialog : public QDialog
{
	Q_OBJECT
		//Constructor
public:
	AeroDrillBoxDialog(AeroDocument* drillBoxDoc, AeroView* drillBoxView, QWidget* drillBoxWidget = 0);
	//~AeroDrillBoxDialog();

private:
	AeroDocument* createDoc;
	AeroView* createView;

private:
	QLabel* labelHeight;
	QLabel* labelLength;
	QLabel* labelWidth;

	QDoubleSpinBox* dSpinBoxHeight;
	QDoubleSpinBox* dSpinBoxLength;
	QDoubleSpinBox* dSpinBoxWidth;

	QGridLayout* gridLayout;

	QPushButton* createButton;
	QPushButton* cancleButton;

	QAction* createButtonAction;
	QAction* cancleButtonAction;

private slots:
	void createButtonSLOT();
	//void cancleButtonSLOT();






};
#endif //AERODRILLBOXDIALOG_H
