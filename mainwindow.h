#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include "OCCViewerWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_textEdit_textChanged();

    void on_buildButton_clicked();


private:
    Ui::MainWindow *ui;

    OCCViewerWidget* occViewer;

    void saveTextToFile(const QString &text);

};
#endif // MAINWINDOW_H
