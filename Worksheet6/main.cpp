#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/icons/fileopen.png"));

    MainWindow w;
    w.show();
    return a.exec();
}
