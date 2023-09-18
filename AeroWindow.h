#ifndef AEROWINDOW_H
#define AEROWINDOW_H

//Aero
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
    void onMakeSphere();
    void onBuildButtonSlot();
    
    //////////////////////////////////////////////
    //void on_textEdit_textChanged();
    //////////////////////////////////////////////

private:
    Ui::AeroWindow *ui;
    AeroView* myView;
    AeroDocument* myDocument;

    /////////////////////////////////////////////
    //void saveTextToFile(const QString &text);
    /////////////////////////////////////////////

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
    QAction* makeSphereAction;
    QAction* wingAction;


    //Menu bar
    QMenu* fileMenu;
    QMenu* primitiveMenu;
    QMenu* viewMenu;
    QMenu* listMenu;



};
#endif // AEROWINDOW_H
