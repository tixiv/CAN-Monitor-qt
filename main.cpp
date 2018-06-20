#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("CAN Monitor 3000");
    QCoreApplication::setApplicationName("CAN Monitor 3000");

    MainWindow w;
    w.show();

    return a.exec();
}
