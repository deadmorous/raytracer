#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (a.arguments().size() > 1)
        w.openScene(a.arguments()[1]);

    return a.exec();
}
