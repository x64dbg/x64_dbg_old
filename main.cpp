#include "_global.h"
#include "argument.h"
#include "command.h"
#include "variable.h"
#include "instruction.h"
#include "debugger.h"
#include "data.h"
#include "resource.h"
#include "console.h"
#include "gui\disasm.h"

static bool cbStrLen(const char* cmd)
{
    char arg1[deflen]="";
    if(argget(cmd, arg1, 0, false))
        printf("\"%s\"[%d]\n", arg1, strlen(arg1));
    return true;
}

static bool cbExit(const char* cmd)
{
    return false;
}

static bool cbCls(const char* cmd)
{
    system("cls");
    return true;
}

static COMMAND* command_list=0;

static void registercommands()
{
    COMMAND* cmd=command_list=cmdinit();
    cmdnew(cmd, "exit\1x", cbExit, false); //quit debugger
    cmdnew(cmd, "strlen\1charcount\1ccount", cbStrLen, false); //get strlen, arg1:string
    cmdnew(cmd, "varnew\1var", cbInstrVar, false); //make a variable arg1:name,[arg2:value]
    cmdnew(cmd, "vardel", cbInstrVarDel, false); //delete a variable, arg1:variable name
    cmdnew(cmd, "mov\1set", cbInstrMov, false); //mov a variable, arg1:dest,arg2:src
    cmdnew(cmd, "cls", cbCls, false); //clear the screen
    cmdnew(cmd, "varlist", cbInstrVarList, false); //list variables[arg1:type filter]
    cmdnew(cmd, "InitDebug\1init\1initdbg", cbDebugInit, false); //init debugger arg1:exefile,[arg2:commandline]
    cmdnew(cmd, "StopDebug\1stop\1dbgstop", cbStopDebug, true); //stop debugger
    cmdnew(cmd, "run\1go\1r\1g", cbDebugRun, true); //unlock WAITID_RUN
    cmdnew(cmd, "SetBPXOptions\1bptype", cbDebugSetBPXOptions, false); //breakpoint type
    cmdnew(cmd, "SetBPX\1bp\1bpx", cbDebugSetBPX, true); //breakpoint
    cmdnew(cmd, "EnableBPX\1bpe\1be", cbDebugEnableBPX, true); //breakpoint enable
    cmdnew(cmd, "DisableBPX\1bpd\1bd", cbDebugDisableBPX, true); //breakpoint disable
    cmdnew(cmd, "ToggleBPX\1bpt\1bt", cbDebugToggleBPX, true); //breakpoint toggle
    cmdnew(cmd, "DeleteBPX\1bpc\1bc", cbDebugDeleteBPX, true); //breakpoint delete
    cmdnew(cmd, "bplist", cbDebugBplist, true); //breakpoint list
    cmdnew(cmd, "StepInto\1sti", cbDebugStepInto, true); //StepInto
    cmdnew(cmd, "StepOver\1step\1sto\1st", cbDebugStepOver, true); //StepOver
    cmdnew(cmd, "SingleStep\1sstep\1sst", cbDebugSingleStep, true); //SingleStep arg1:count
    cmdnew(cmd, "HideDebugger\1dbh\1hide", cbDebugHide, true); //HideDebugger
    cmdnew(cmd, "disasm\1dis\1d", cbDebugDisasm, true); //doDisasm
    cmdnew(cmd, "SetMemoryBPX\1membp\1bpm", cbDebugMemoryBpx, true); //SetMemoryBPX
    cmdnew(cmd, "chd", cbInstrChd, false); //Change directory
    cmdnew(cmd, "rtr", cbDebugRtr, true); //rtr
    cmdnew(cmd, "SetHardwareBreakpoint\1bph\1bphws", cbDebugSetHardwareBreakpoint, true); //hardware breakpoint
    cmdnew(cmd, "alloc", cbDebugAlloc, true); //allocate memory
    cmdnew(cmd, "free", cbDebugFree, true); //free memory
    cmdnew(cmd, "Fill\1memset", cbDebugMemset, true); //memset
}

static DWORD WINAPI focusThread(void* lpParam)
{
    unsigned int lastpress=0;
    HWND console=GetConsoleHwnd();
    HWND lastwindow=console;
    while(1)
    {
        if(GetAsyncKeyState(VK_RMENU))
        {
            if(GetTickCount()-lastpress<300)
                continue;
            if(GetForegroundWindow()==console)
                SetForegroundWindow(lastwindow);
            else
            {
                lastwindow=GetForegroundWindow();
                SetForegroundWindow(console);
            }
            lastpress=GetTickCount();
        }
        Sleep(100);
    }
    return 0;
}

int main()
{
    SetConsoleIcon(LoadIconA(0, MAKEINTRESOURCEA(IDI_ICON1)));
#ifndef _WIN64
    SetConsoleTitleA("x32_dbg");
#else
    SetConsoleTitleA("x64_dbg");
#endif
    DeleteFileA("DLLLoader.exe");
    char dir[deflen]="";
    GetModuleFileNameA(GetModuleHandleA(0), dir, deflen);
    int len=strlen(dir);
    while(dir[len]!='\\')
        len--;
    dir[len]=0;
    SetCurrentDirectoryA(dir);
    CreateThread(0, 0, focusThread, 0, 0, 0);

    varinit();
    registercommands();
    Sleep(200);
    SetForegroundWindow(GetConsoleHwnd());
    cmdloop(command_list, cbBadCmd);
    DeleteFileA("DLLLoader.exe");
    return 0;
}
