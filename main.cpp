#include "centercontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CenterControl::instance().show();
    return a.exec();
}
