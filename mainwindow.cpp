#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QMainWindow>
#include <QDir>
#include <QFileInfoList>
#include <QTreeWidgetItem>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage("Aerofoil Tools by Pasan Jayasinghe Version 0.1");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_textEdit_textChanged() {
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
        // You can add error handling here, e.g., show a message box.
    }
}



#include "wingClass.h"
void MainWindow::on_buildButton_clicked()
{
    statusBar()->showMessage("Build Started...");
    double chord = ui->chordDoubleSpinBox->value();
    double span  = ui->spanDoubleSpinBox_2->value();
    wingClass thiswing("defaultRequest",chord, span);
}

