//AERO
#include "AeroWindow.h"

//QT
#include <QApplication>
#include <QProcess>

//OCCT

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AeroWindow w;
    w.show();
    return a.exec();
}
