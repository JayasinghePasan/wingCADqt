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
    void onSumSlot();
    void onListMenuBoundSlot();
    void onListMenuOccuranceSlot();
    void onListMenuBubbleSortSlot();
    void onListMenuQuickSortSlot();
    void onMakeBottleSlot();
    void onMakeDrillBoxSlot();
    
    //////////////////////////////////////////////
    //void on_textEdit_textChanged();
    //void on_buildButton_clicked();
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
    QAction* zoomAction;
    QAction* panAction;
    QAction* rotateAction;

    QAction* makeSphereAction;
    QAction* sumAction;
    QAction* listMenuBoundAction;
    QAction* listMenuOccuranceAction;
    QAction* listMenuBubbleSortAction;
    QAction* listMenuQuickSortAction;
    QAction* BottleMenuAction;
    QAction* DrillBoxAction;

    //Menu bar
    QMenu* fileMenu;
    QMenu* primitiveMenu;
    QMenu* viewMenu;
    QMenu* sumMenu;
    QMenu* listMenu;



};
#endif // AEROWINDOW_H
