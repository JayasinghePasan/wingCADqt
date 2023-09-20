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

    QPalette palette = w.palette();
    palette.setColor(QPalette::Background, Qt::white);
    w.setPalette(palette);
    
    w.show();
    return a.exec();
}
