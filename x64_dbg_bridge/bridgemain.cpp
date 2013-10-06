#include "_global.h"
#include "bridgemain.h"
#include <stdio.h>

static HINSTANCE hInst;

#ifdef _WIN64
#define dbg_lib "x64_dbg.dll"
#define gui_lib "DebuggerX64.dll"
#else
#define dbg_lib "x32_dbg.dll"
#define gui_lib "x32_gui.dll"
#endif // _WIN64

#define NO_GUI //for debugger-only testinw

//Bridge
const char* DLL_IMPEXP BridgeInit()
{
#ifndef NO_GUI
    //GUI Load
    hInstGui=LoadLibraryA(gui_lib); //Sigma
    if(!hInstGui)
        return "Error loading GUI library ("gui_lib")!";
    //_gui_guiinit
    _gui_guiinit=(GUIGUIINIT)GetProcAddress(hInstGui, "_gui_guiinit");
    if(!_gui_guiinit)
        return "Export \"_gui_guiinit\" could not be found!";
    _gui_changecip=(GUICHANGECIP)GetProcAddress(hInstGui, "_gui_changecip");
    if(!_gui_changecip)
        return "Export \"_gui_changecip\" could not be found!";
#endif
    //DBG Load
    hInstDbg=LoadLibraryA(dbg_lib); //Mr. eXoDia
    if(!hInstDbg)
        return "Error loading debugger library ("dbg_lib")!";
    //_dbg_dbginit
    _dbg_dbginit=(DBGDBGINIT)GetProcAddress(hInstDbg, "_dbg_dbginit");
    if(!_dbg_dbginit)
        return "Export \"_dbg_dbginit\" could not be found!";
    //_dbg_memfindbaseaddr
    _dbg_memfindbaseaddr=(DBGMEMFINDBASEADDR)GetProcAddress(hInstDbg, "_dbg_memfindbaseaddr");
    if(!_dbg_memfindbaseaddr)
        return "Export \"_dbg_memfindbaseaddr\" could not be found!";
    //_dbg_memfindbaseaddr
    _dbg_memread=(DBGMEMREAD)GetProcAddress(hInstDbg, "_dbg_memread");
    if(!_dbg_memread)
        return "Export \"_dbg_memread\" could not be found!";
    return 0;
}

const char* DLL_IMPEXP BridgeStart()
{
#ifndef NO_GUI
    if(!_dbg_dbginit or !_gui_guiinit)
#else
    if(!_dbg_dbginit)
#endif
        return "\"_dbg_dbginit\" or \"_gui_guiinit\" was not loaded yet, call BridgeInit!";
    const char* errormsg=_dbg_dbginit();
    if(errormsg)
        return errormsg;
#ifndef NO_GUI
    _gui_guiinit(0, 0); //remove arguments
#else
    Sleep(-1);
#endif
    return 0;
}

//Debugger
void DLL_IMPEXP DbgMemRead(duint va, unsigned char* dest, duint size)
{
    if(!_dbg_memread(va, dest, size, 0))
        memset(dest, 0x90, size);
}

duint DLL_IMPEXP DbgMemGetPageSize(duint base)
{
    duint size=0;
    _dbg_memfindbaseaddr(base, &size);
    return size;
}

duint DLL_IMPEXP DbgMemFindBaseAddr(duint addr, duint* size)
{
    return _dbg_memfindbaseaddr(addr, size);
}

//GUI
void DLL_IMPEXP GuiChangeCIP(duint cip)
{
#ifndef NO_GUI
    _gui_changecip(cip);
#endif
}

//Main
extern "C" DLL_IMPEXP BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    hInst=hinstDLL;
    return TRUE;
}

