#include <QtGui/QApplication>
#include "qratzfest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qratzfest w;
    w.show();
    
    return a.exec();
}
