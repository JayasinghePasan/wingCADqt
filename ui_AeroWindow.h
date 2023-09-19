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

    void setupUi(QMainWindow* aeroWindow)
    {
        if (aeroWindow->objectName().isEmpty())
            aeroWindow->setObjectName(QString::fromUtf8("AeroWindow"));

        aeroWindow->resize(800, 600);
        actionClose = new QAction(aeroWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        centralWidget = new QWidget(aeroWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        aeroWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(aeroWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        aeroWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(aeroWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        aeroWindow->setStatusBar(statusBar);

        retranslateUi(aeroWindow);

        QMetaObject::connectSlotsByName(aeroWindow);
    } // setupUi

    void retranslateUi(QMainWindow* aeroWindow)
    {
        aeroWindow->setWindowTitle(QCoreApplication::translate("AeroWindow", "AeroTool 3D by PasanJ", nullptr));
        actionClose->setText(QCoreApplication::translate("AeroWindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AeroWindow : public Ui_AeroWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AEROWINDOW_H
