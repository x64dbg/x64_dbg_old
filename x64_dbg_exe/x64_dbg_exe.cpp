#include <stdio.h>
#include <windows.h>
#include "resource.h"
#include "..\x64_dbg_bridge\bridgemain.h"

static void SetConsoleIcon(HICON hIcon)
{
    HMODULE hMod=GetModuleHandleA("kernel32");
    if(!hMod)
        return;
    typedef DWORD (WINAPI *SCI)(HICON);
    SCI sci=(SCI)GetProcAddress(hMod, "SetConsoleIcon");
    if(!sci)
        return;
    sci(hIcon);
}

int main()
{
    SetConsoleIcon(LoadIconA(0, MAKEINTRESOURCEA(IDI_ICON1)));
#ifndef _WIN64
    SetConsoleTitleA("x32_dbg");
#else
    SetConsoleTitleA("x64_dbg");
#endif
    const char* errormsg=BridgeInit();
    if(errormsg)
    {
        MessageBoxA(0, errormsg, "BridgeInit Error", MB_ICONERROR|MB_SYSTEMMODAL);
        return 1;
    }
    errormsg=BridgeStart();
    if(errormsg)
    {
        MessageBoxA(0, errormsg, "BridgeStart Error", MB_ICONERROR|MB_SYSTEMMODAL);
        return 1;
    }
    return 0;
}
