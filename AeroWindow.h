#ifndef AEROWINDOW_H
#define AEROWINDOW_H

//Aero
#include "wingClass.h";
//OCCT
//QT
#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTimer>

class AeroView;
class AeroDocument;


namespace Ui 
{ 
    class AeroWindow; 
}


class AeroWindow : public QMainWindow
{
    Q_OBJECT

public:
    AeroWindow(QWidget* parent = 0);
    ~AeroWindow();

protected:
    void createAction(void);
    void createMenu(void);

private slots:
    void exit(void);
    

public slots:
    void onExampleWingSlot();
    void handleBuildRequest(double& chord, double& span, QString& textCoords);
    void handleExportRequest(int& index, QString& filename);

private:
    Ui::AeroWindow *ui;
    AeroView* myView;
    AeroDocument* myDocument;

    wingClass* currentWing;


private:
    QAction* exitAction;

    //view actions
    QAction* fitAllAction;
    QAction* frontAction;
    QAction* backAction;
    QAction* topAction;
    QAction* bottomAction;
    QAction* leftAction;
    QAction* rightAction;


    //primitive actions
    QAction* wingAction;


    //Menu bar
    QMenu* fileMenu;
    QMenu* exampleMenu;
    QMenu* viewMenu;



};
#endif // AEROWINDOW_H
