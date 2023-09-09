#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QMainWindow>
#include <QDir>
#include <QFileInfoList>
#include <QTreeWidgetItem>
#include <QIcon>

#include "OCCViewerWidget.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage("Aerofoil Tools by Pasan Jayasinghe Version 0.1");

    occViewer = new OCCViewerWidget(ui->openGLWidget);
    occViewer->setParent(ui->openGLWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_textEdit_textChanged() {
    statusBar()->showMessage("Data updated...");
    QString text = ui->textEdit->toPlainText();
    saveTextToFile(text);
}


void MainWindow::saveTextToFile(const QString &text) {
    QFile file("tempAerofoilData.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << text;
        file.close();
    } else {
        statusBar()->showMessage("tempAerofoilData.txt is unable to reach.");
    }
}


#include "wingClass.h"
#include <BRepPrimAPI_MakeBox.hxx>


void MainWindow::on_buildButton_clicked()
{
    statusBar()->showMessage("Build Started...");
    double chord = ui->chordDoubleSpinBox->value();
    double span  = ui->spanDoubleSpinBox_2->value();
    int index = ui->formatComboBox->currentIndex();
    const QString filename = ui->filename->text();
    wingClass thiswing("defaultRequest",chord, span, index, filename.toStdString());
    //occViewer->displayShape(thiswing.getShape());
    TopoDS_Shape simpleBox = BRepPrimAPI_MakeBox(10, 10, 10).Shape();
    occViewer->displayShape(simpleBox);
}


