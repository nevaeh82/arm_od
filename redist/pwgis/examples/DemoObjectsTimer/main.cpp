#include "DemoObjectsTimer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DemoObjectsTimer w;
    w.show();

    return a.exec();
}
