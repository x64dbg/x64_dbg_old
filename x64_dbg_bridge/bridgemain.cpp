#include "_global.h"
#include "bridgemain.h"
#include <stdio.h>

static HINSTANCE hInst;

#ifdef _WIN64
#define dbg_lib "x64_dbg.dll"
#define gui_lib "x64_gui.dll"
#else
#define dbg_lib "x32_dbg.dll"
#define gui_lib "x32_gui.dll"
#endif // _WIN64

//#define NO_GUI //for debugger-only testing

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
    //_gui_disassembleat
    _gui_disassembleat=(GUIDISASSEMBLEAT)GetProcAddress(hInstGui, "_gui_disassembleat");
    if(!_gui_disassembleat)
        return "Export \"_gui_disassembleat\" could not be found!";
    //_gui_setdebugstate
    _gui_setdebugstate=(GUISETDEBUGSTATE)GetProcAddress(hInstGui, "_gui_setdebugstate");
    if(!_gui_setdebugstate)
        return "Export \"_gui_setdebugstate\" could not be found!";
    //_gui_addlogmessage
    _gui_addlogmessage=(GUIADDLOGMESSAGE)GetProcAddress(hInstGui, "_gui_addlogmessage");
    if(!_gui_addlogmessage)
        return "Export \"_gui_addlogmessage\" could not be found!";
    //_gui_logclear
    _gui_logclear=(GUILOGCLEAR)GetProcAddress(hInstGui, "_gui_logclear");
    if(!_gui_logclear)
        return "Export \"_gui_logclear\" could not be found!";
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
    //_dbg_dbgcmdexec
    _dbg_dbgcmdexec=(DBGDBGCMDEXEC)GetProcAddress(hInstDbg, "_dbg_dbgcmdexec");
    if(!_dbg_dbgcmdexec)
        return "Export \"_dbg_dbgcmdexec\" could not be found!";
    //_dbg_memmap
    _dbg_memmap=(DBGMEMMAP)GetProcAddress(hInstDbg, "_dbg_memmap");
    if(!_dbg_memmap)
        return "Export \"_dbg_memmap\" could not be found!";
    //_dbg_dbgexitsignal
    _dbg_dbgexitsignal=(DBGDBGEXITSIGNAL)GetProcAddress(hInstDbg, "_dbg_dbgexitsignal");
    if(!_dbg_dbgexitsignal)
        return "Export \"_dbg_dbgexitsignal\" could not be found!";
    //_dbg_valfromstring
    _dbg_valfromstring=(DBGVALFROMSTRING)GetProcAddress(hInstDbg, "_dbg_valfromstring");
    if(!_dbg_valfromstring)
        return "Export \"_dbg_valfromstring\" could not be found!";
    //_dbg_isdebugging
    _dbg_isdebugging=(DBGISDEBUGGING)GetProcAddress(hInstDbg, "_dbg_isdebugging");
    if(!_dbg_isdebugging)
        return "Export \"_dbg_isdebugging\" could not be found!";
    //_dbg_isjumpgoingtoexecute
    _dbg_isjumpgoingtoexecute=(DBGISJUMPGOINGTOEXECUTE)GetProcAddress(hInstDbg, "_dbg_isjumpgoingtoexecute");
    if(!_dbg_isjumpgoingtoexecute)
        return "Export \"_dbg_isjumpgoingtoexecute\" could not be found!";
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
    _dbg_dbgexitsignal(); //send exit signal to debugger
    return 0;
}

void* DLL_IMPEXP BridgeAlloc(size_t size)
{
    unsigned char* a=new unsigned char[size];
    if(!a)
    {
        MessageBoxA(0, "Could not allocate memory", "Error", MB_ICONERROR);
        ExitProcess(1);
    }
    memset(a, 0, size);
    return a;
}

void DLL_IMPEXP BridgeFree(void* ptr)
{
    delete[] (unsigned char*)ptr;
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

bool DLL_IMPEXP DbgCmdExec(const char* cmd)
{
    return _dbg_dbgcmdexec(cmd);
}

bool DLL_IMPEXP DbgMemMap(MEMMAP* memmap)
{
    return _dbg_memmap(memmap);
}

bool DLL_IMPEXP DbgIsValidExpression(const char* expression)
{
    duint value=0;
    return _dbg_valfromstring(expression, &value);
}

bool DLL_IMPEXP DbgIsDebugging()
{
    return _dbg_isdebugging();
}

bool DLL_IMPEXP DbgIsJumpGoingToExecute(duint addr)
{
    return _dbg_isjumpgoingtoexecute(addr);
}

//GUI
void DLL_IMPEXP GuiDisasmAt(duint addr, duint cip)
{
#ifndef NO_GUI
    _gui_disassembleat(addr, cip);
#endif
}

void DLL_IMPEXP GuiSetDebugState(DBGSTATE state)
{
#ifndef NO_GUI
    _gui_setdebugstate(state);
#endif // NO_GUI
}

void DLL_IMPEXP GuiAddLogMessage(const char* msg)
{
#ifndef NO_GUI
    _gui_addlogmessage(msg);
#endif // NO_GUI
}

void DLL_IMPEXP GuiLogClear()
{
#ifndef NO_GUI
    _gui_logclear();
#endif // NO_GUI
}

//Main
extern "C" DLL_IMPEXP BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    hInst=hinstDLL;
    return TRUE;
}

