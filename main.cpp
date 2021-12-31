#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName("Client Server Example");
    MainWindow w;
    w.show();
    return a.exec();
}
