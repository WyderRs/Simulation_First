#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");


    w.show();
    return a.exec();
}
