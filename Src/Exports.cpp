#include "Exports.h"


#ifdef BUILD_LIB

__declspec(dllexport) int launchGUI(int argc, char *argv[])
{
    QApplication wApp(argc, argv);
    Bridge wBridge;

    MainWindow wMainWin;
    wMainWin.show();

    return wApp.exec();
}

__declspec(dllexport) int GuiChangeEIP(unsigned long long eip)
{

}


#endif


