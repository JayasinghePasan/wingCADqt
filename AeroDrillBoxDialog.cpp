#pragma once

#include "AeroDrillBoxDialog.h"
#include "AeroWindow.h"

AeroDrillBoxDialog::AeroDrillBoxDialog(AeroDocument* drillBoxDoc, AeroView* drillBoxView, QWidget* drillBoxWidget) : QDialog(drillBoxWidget)
{
	this->createDoc = drillBoxDoc;
	this->createView = drillBoxView;

	//creating Height buddies
	labelHeight = new QLabel(tr("&Height"));
	dSpinBoxHeight = new QDoubleSpinBox();
	dSpinBoxHeight->setRange(1e-7, DBL_MAX);
	dSpinBoxHeight->setValue(10.0);
	labelHeight->setBuddy(dSpinBoxHeight);

	//creating Length buddies
	labelLength = new QLabel(tr("&Length"));
	dSpinBoxLength = new QDoubleSpinBox();
	dSpinBoxLength->setRange(1e-7, DBL_MAX);
	dSpinBoxLength->setValue(10.0);
	labelLength->setBuddy(dSpinBoxLength);

	//creating Width buddies
	labelWidth = new QLabel(tr("&Width"));
	dSpinBoxWidth = new QDoubleSpinBox();
	dSpinBoxWidth->setRange(1e-7, DBL_MAX);
	dSpinBoxWidth->setValue(10.0);
	labelWidth->setBuddy(dSpinBoxWidth);

	//assigning the gridlayout
	gridLayout = new QGridLayout();

	gridLayout->addWidget(labelHeight, 0, 0);
	gridLayout->addWidget(labelLength, 1, 0);
	gridLayout->addWidget(labelWidth, 2, 0);

	gridLayout->addWidget(dSpinBoxHeight, 0, 1);
	gridLayout->addWidget(dSpinBoxLength, 1, 1);
	gridLayout->addWidget(dSpinBoxWidth, 2, 1);

	//creating pushButtons
	createButton = new QPushButton(tr("&Create"), this);
	connect(createButton, SIGNAL(clicked()), this, SLOT(createButtonAction));
	gridLayout->addWidget(createButton, 0, 2);

	cancleButton = new QPushButton(tr("&Cancle"), this);
	connect(cancleButton, SIGNAL(clicked()), this, SLOT(cancleButtonAction));
	gridLayout->addWidget(cancleButton, 0, 2);



}

void AeroDrillBoxDialog::createButtonSLOT() {
	// method body here
}


