/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_4;
    QTextEdit *textEdit;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_3;
    QComboBox *formatComboBox;
    QPushButton *pushButton_3;
    QDoubleSpinBox *chordDoubleSpinBox;
    QDoubleSpinBox *spanDoubleSpinBox_2;
    QLabel *label_3;
    QLineEdit *filename;
    QPushButton *buildButton;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QOpenGLWidget *openGLWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        textEdit = new QTextEdit(frame_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout->addWidget(textEdit, 0, 0, 1, 2);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(frame_2);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));

        gridLayout->addWidget(doubleSpinBox_3, 7, 1, 1, 1);

        formatComboBox = new QComboBox(frame_2);
        formatComboBox->addItem(QString());
        formatComboBox->addItem(QString());
        formatComboBox->addItem(QString());
        formatComboBox->setObjectName(QString::fromUtf8("formatComboBox"));

        gridLayout->addWidget(formatComboBox, 3, 1, 1, 1);

        pushButton_3 = new QPushButton(frame_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 8, 0, 1, 2);

        chordDoubleSpinBox = new QDoubleSpinBox(frame_2);
        chordDoubleSpinBox->setObjectName(QString::fromUtf8("chordDoubleSpinBox"));
        chordDoubleSpinBox->setMaximum(999999.000000000000000);
        chordDoubleSpinBox->setValue(1.000000000000000);

        gridLayout->addWidget(chordDoubleSpinBox, 1, 1, 1, 1);

        spanDoubleSpinBox_2 = new QDoubleSpinBox(frame_2);
        spanDoubleSpinBox_2->setObjectName(QString::fromUtf8("spanDoubleSpinBox_2"));
        spanDoubleSpinBox_2->setMaximum(999999.000000000000000);
        spanDoubleSpinBox_2->setValue(1.000000000000000);

        gridLayout->addWidget(spanDoubleSpinBox_2, 2, 1, 1, 1);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 7, 0, 1, 1);

        filename = new QLineEdit(frame_2);
        filename->setObjectName(QString::fromUtf8("filename"));

        gridLayout->addWidget(filename, 4, 0, 1, 2);

        buildButton = new QPushButton(frame_2);
        buildButton->setObjectName(QString::fromUtf8("buildButton"));

        gridLayout->addWidget(buildButton, 6, 0, 1, 2);


        gridLayout_4->addWidget(frame_2, 0, 1, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        openGLWidget = new QOpenGLWidget(frame);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));

        verticalLayout->addWidget(openGLWidget);


        gridLayout_4->addWidget(frame, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Chord Length", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Export Format", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Paste your Coordinates here. First line will be neglected.", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Span Length", nullptr));
        formatComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "STEP (.step)", nullptr));
        formatComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "BREP (.brep)", nullptr));
        formatComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "IGES (.iges)", nullptr));

        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Run FEA", nullptr));
        chordDoubleSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " cm", nullptr));
        spanDoubleSpinBox_2->setSuffix(QCoreApplication::translate("MainWindow", " cm", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Wind Speed", nullptr));
        filename->setPlaceholderText(QCoreApplication::translate("MainWindow", "Name your expoort file here.", nullptr));
        buildButton->setText(QCoreApplication::translate("MainWindow", "Build", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
