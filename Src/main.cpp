#include <QApplication>
#include <QtGui>
#include "MainWindow.h"
#include "NewTypes.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<int32>("int32");
    qRegisterMetaType<uint32>("uint32");

    qRegisterMetaType<int64>("int64");
    qRegisterMetaType<uint64>("uint64");

    qRegisterMetaType<byte_t>("byte_t");

    //iInitDbg();

    Bridge::initBridge();

    MainWindow w;
    w.show();

    return a.exec();
}


