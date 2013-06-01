#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"
#include "threading.h"

static PROCESS_INFORMATION* fdProcessInfo;
static char szFileName[deflen]="";
bool bFileIsDll;

static void cbEntryBreakpoint()
{
    cinsert("entry point reached!");
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbSystemBreakpoint(void* ExceptionData)
{
    //handle stuff (TLS, main entry, etc)
    cputs("system breakpoint reached!");
    //unlock
    unlock(WAITID_SYSBREAK);
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static DWORD WINAPI threadDebugLoop(void* lpParameter)
{
    //initialize
    INIT_STRUCT* init=(INIT_STRUCT*)lpParameter;
    bFileIsDll=IsFileDLL(init->exe, 0);
    if(bFileIsDll)
        fdProcessInfo=(PROCESS_INFORMATION*)InitDLLDebug(init->exe, false, init->commandline, init->currentfolder, (void*)cbEntryBreakpoint);
    else
        fdProcessInfo=(PROCESS_INFORMATION*)InitDebugEx(init->exe, init->commandline, init->currentfolder, (void*)cbEntryBreakpoint);
    if(!fdProcessInfo)
    {
        cputs("error starting process (invalid pe?)!");
        unlock(WAITID_SYSBREAK);
        return 0;
    }
    strcpy(szFileName, init->exe);
    varset("$hp", (void*)fdProcessInfo->hProcess, true);
    varset("$pid", (void*)fdProcessInfo->dwProcessId, true);
    SetCustomHandler(UE_CH_CREATEPROCESS, (void*)cbSystemBreakpoint);
    //run debug loop (returns when process debugging is stopped)
    DebugLoop();
    //message the user/do final stuff
    cinsert("debugging stopped!");
    varset("$hp", (void*)0, true);
    varset("$pid", (void*)0, true);
    waitclear();
    return 0;
}

bool cbDebugInit(const char* cmd)
{
    dbg("cbInitDebug");
    if(IsFileBeingDebugged())
    {
        cputs("already debugging!");
        return true;
    }

    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, false))
        return true;
    if(!FileExists(arg1))
    {
        cputs("file does not exsist!");
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

    //initialize
    waitclear(); //clear waiting flags
    lock(WAITID_SYSBREAK);
    if(!CreateThread(0, 0, threadDebugLoop, init, 0, 0))
    {
        cputs("failed creating debug thread!");
        return true;
    }
    wait(WAITID_SYSBREAK);
    return true;
}

bool cbDebugRun(const char* cmd)
{
    if(!waitislocked(WAITID_RUN))
    {
        cputs("program is already running");
        return true;
    }
    unlock(WAITID_RUN);
    return true;
}

bool cbDebugEpBreak(const char* cmd)
{
    dbg("cbDebugEpBreak");

    uint imagebase;
    if(bFileIsDll)
        imagebase=GetDebuggedDLLBaseAddress();
    else
        imagebase=GetDebuggedFileBaseAddress();
    uint ep=GetPE32Data(szFileName, 0, UE_OEP);
    if(!SetBPX(imagebase+ep, UE_SINGLESHOOT, (void*)cbEntryBreakpoint))
        cputs("failed to set breakpoint!");
    else
        cputs("breakpoint set!");
    return true;
}
