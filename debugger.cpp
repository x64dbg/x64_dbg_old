#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"

static PROCESS_INFORMATION* fdProcessInfo;

static DWORD WINAPI threadDebugLoop(void* lpParameter)
{
    //initialize

    //run debug loop (returns when process debugging is stopped)
    DebugLoop();
    //message the user/do final stuff
    consoleinsert("debugging stopped!");
    return 0;
}

static void cbSystemBreakpoint(void* ExceptionData)
{
    //handle stuff (TLS, main entry, etc)
    puts("system breakpoint reached!");
    //unlock
    unlock(WAITID_SYSBREAK);
}

bool cbInitDebug(const char* cmd)
{
    dbg("cbInitDebug");
    if(IsFileBeingDebugged())
    {
        puts("already debugging!");
        return true;
    }
    char arg1[deflen]="";
    char arg2[deflen]="";
    char arg3[deflen]="";
    if(!argget(cmd, arg1, 0, false))
        return true;
    argget(cmd, arg2, 1, true);
    argget(cmd, arg3, 2, true);
    char full_path[deflen]="";
    if(arg1[1]!=':')
    {
        GetCurrentDirectoryA(deflen, full_path);
        PathAppendA(full_path, arg1);
    }
    else
        strcpy(full_path, arg1);
    printf("[DEBUG] InitDebug(\"%s\", \"%s\", \"%s\");\n", full_path, arg2, arg3);
    if(IsFileDLL(full_path, 0))
        fdProcessInfo=(PROCESS_INFORMATION*)InitDLLDebug(full_path, false, arg2, arg3, 0);
    else
        fdProcessInfo=(PROCESS_INFORMATION*)InitDebug(full_path, arg2, arg3);
    if(!fdProcessInfo)
    {
        puts("error starting process (invalid pe?)!");
        return true;
    }
    //initialize
    waitclear(); //clear waiting flags
    varset("$hp", (void*)fdProcessInfo->hProcess, true);
    varset("$pid", (void*)fdProcessInfo->dwProcessId, true);
    SetCustomHandler(UE_CH_CREATEPROCESS, (void*)cbSystemBreakpoint);
    //start debugging
    lock(WAITID_SYSBREAK);
    if(!CreateThread(0, 0, threadDebugLoop, 0, 0, 0))
    {
        puts("failed creating debug thread!");
        return true;
    }
    wait(WAITID_SYSBREAK);
    return true;
}
