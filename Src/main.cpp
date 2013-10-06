#include <QApplication>
#include <QtGui>
#include "MainWindow.h"
#include "NewTypes.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Register custom data types
    qRegisterMetaType<int32>("int32");
    qRegisterMetaType<uint32>("uint32");

    qRegisterMetaType<int64>("int64");
    qRegisterMetaType<uint64>("uint64");

    qRegisterMetaType<byte_t>("byte_t");

    // Init communication with debugger
    Bridge::initBridge();

    // Start GUI
    MainWindow w;
    w.show();

    return a.exec();
}


