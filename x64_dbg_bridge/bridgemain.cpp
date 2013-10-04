#include "_global.h"
#include "bridgemain.h"

static HINSTANCE hInst;

#ifdef _WIN64
#define dbg_lib "x64_dbg.dll"
#define gui_lib "x64_gui.dll"
#else
#define dbg_lib "x32_dbg.dll"
#define gui_lib "x32_gui.dll"
#endif // _WIN64

//Bridge
const char* DLL_EXPORT BridgeInit()
{
    hInstDbg=LoadLibraryA(dbg_lib); //Mr. eXoDia
    if(!hInstDbg)
        return "Error loading debugger library ("dbg_lib")!";
    hInstGui=LoadLibraryA(gui_lib); //Sigma
    if(!hInstGui)
        return "Error loading GUI library ("gui_lib")!";
    //DBG GPA
    _dbg_dbginit=(DBGDBGINIT)GetProcAddress(hInstDbg, "_dbg_dbginit");
    //DBG Check
    if(!_dbg_dbginit)
        return "Export \"_dbg_dbginit\" could not be found!";
    //GUI GPA
    _gui_guiinit=(GUIGUIINIT)GetProcAddress(hInstGui, "_gui_guiinit");
    //GUI Check
    if(!_gui_guiinit)
        return "Export \"_gui_guiinit\" could not be found!";
    return 0;
}

const char* DLL_EXPORT BridgeStart()
{
    if(!_dbg_dbginit or !_gui_guiinit)
        return "\"_dbg_dbginit\" or \"_gui_guiinit\" was not loaded yet, call BridgeInit!";
    const char* errormsg=_dbg_dbginit();
    if(errormsg)
        return errormsg;
    _gui_guiinit(0, 0); //remove arguments
    return 0;
}

//Debugger
void DLL_EXPORT DbgMemRead(unsigned char* dest, unsigned long long va, unsigned long long size)
{
    //TODO: call debugger
    memset(dest, 0x90, size);
}

unsigned long long DLL_EXPORT DbgMemGetPageSize(unsigned long long base)
{
    //TODO: call debugger
    return 0x1000;
}

//GUI
void DLL_EXPORT GuiChangeCIP(unsigned long long cip)
{
    //TODO: implement this
}

//Main
extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    hInst=hinstDLL;
    return TRUE;
}

