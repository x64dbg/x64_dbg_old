#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"

static PROCESS_INFORMATION* fdProcessInfo;

static void cbSystemBreakpoint(void* ExceptionData)
{
    //handle stuff (TLS, main entry, etc)
    puts("system breakpoint reached!");
    //unlock
    unlock(WAITID_SYSBREAK);
}

static DWORD WINAPI threadDebugLoop(void* lpParameter)
{
    //initialize
    INIT_STRUCT* init=(INIT_STRUCT*)lpParameter;
    if(IsFileDLL(init->exe, 0))
        fdProcessInfo=(PROCESS_INFORMATION*)InitDLLDebug(init->exe, false, init->commandline, init->currentfolder, 0);
    else
        fdProcessInfo=(PROCESS_INFORMATION*)InitDebug(init->exe, init->commandline, init->currentfolder);
    if(!fdProcessInfo)
    {
        puts("error starting process (invalid pe?)!");
        return true;
    }
    varset("$hp", (void*)fdProcessInfo->hProcess, true);
    varset("$pid", (void*)fdProcessInfo->dwProcessId, true);
    SetCustomHandler(UE_CH_CREATEPROCESS, (void*)cbSystemBreakpoint);
    //run debug loop (returns when process debugging is stopped)
    DebugLoop();
    //message the user/do final stuff
    consoleinsert("debugging stopped!");
    waitclear();
    return 0;
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
    if(!argget(cmd, arg1, 0, false))
        return true;
    if(!FileExists(arg1))
    {
        puts("file does not exsist!");
        return true;
    }

    char arg2[deflen]="";
    argget(cmd, arg2, 1, true);
    char* commandline=0;
    if(strlen(arg2))
        commandline=arg2;

    char arg3[deflen]="";
    argget(cmd, arg3, 2, true);
    char* currentfolder=0;
    if(DirExists(currentfolder))
        currentfolder=arg3;
    INIT_STRUCT* init=(INIT_STRUCT*)malloc(sizeof(INIT_STRUCT));
    memset(init, 0, sizeof(INIT_STRUCT));
    init->exe=arg1;
    init->commandline=commandline;
    init->currentfolder=currentfolder;

    //printf("[DEBUG] InitDebug(\"%s\", \"%s\", \"%s\");\n", arg1, commandline, currentfolder);


    //initialize
    waitclear(); //clear waiting flags
    lock(WAITID_SYSBREAK);
    if(!CreateThread(0, 0, threadDebugLoop, init, 0, 0))
    {
        puts("failed creating debug thread!");
        return true;
    }
    wait(WAITID_SYSBREAK);
    return true;
}
