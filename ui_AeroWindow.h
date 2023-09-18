#ifndef UI_AEROWINDOW_H
#define UI_AEROWINDOW_H

//Aero
//OCCT
//QT
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AeroWindow
{
public:
    QAction* actionClose;
    QWidget* centralWidget;
    QMenuBar* menuBar;
    QToolBar* mainToolBar;
    QStatusBar* statusBar;

    void setupUi(QMainWindow* tGenWindow)
    {
        if (tGenWindow->objectName().isEmpty())
            tGenWindow->setObjectName(QString::fromUtf8("TGenWindow"));

        tGenWindow->resize(800, 600);
        actionClose = new QAction(tGenWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        centralWidget = new QWidget(tGenWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tGenWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(tGenWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        tGenWindow->setMenuBar(menuBar);
        //mainToolBar = new QToolBar(TGenWindow);
            //mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        //TGenWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(tGenWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        tGenWindow->setStatusBar(statusBar);

        retranslateUi(tGenWindow);

        QMetaObject::connectSlotsByName(tGenWindow);
    } // setupUi

    void retranslateUi(QMainWindow* tGenWindow)
    {
        tGenWindow->setWindowTitle(QCoreApplication::translate("TGenWindow", "TurboGenCAE", nullptr));
        actionClose->setText(QCoreApplication::translate("TGenWindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AeroWindow : public Ui_AeroWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AEROWINDOW_H
