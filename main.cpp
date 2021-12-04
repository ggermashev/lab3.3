#include "lab33qt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lab33qt w;
    w.setFixedHeight(1100);
    w.setFixedWidth(1400);
    w.show();
    return a.exec();
}
