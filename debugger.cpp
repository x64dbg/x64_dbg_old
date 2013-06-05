#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"
#include "threading.h"
#include "value.h"
#include "breakpoint.h"

static PROCESS_INFORMATION* fdProcessInfo;
static char szFileName[deflen]="";
bool bFileIsDll;
BREAKPOINT* bplist;

static void cbUserBreakpoint()
{
    BREAKPOINT* cur=bpfind(bplist, 0, GetContextData(UE_CIP), 0);
    if(!cur)
        cinsert("breakpoint reached not in list!");
    else
    {
        char log[deflen]="";
        if(cur->name)
            sprintf(log, "breakpoint \"%s\" at "fhex"!", cur->name, cur->addr);
        else
            sprintf(log, "breakpoint at "fhex"!", cur->addr);
        cinsert(log);
    }
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

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
    SetCustomHandler(UE_CH_CREATEPROCESS, 0);
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
    varset("$hp", (uint)fdProcessInfo->hProcess, true);
    varset("$pid", fdProcessInfo->dwProcessId, true);
    bplist=bpinit();
    SetCustomHandler(UE_CH_CREATEPROCESS, (void*)cbSystemBreakpoint);
    //run debug loop (returns when process debugging is stopped)
    DebugLoop();
    DeleteFileA("DLLLoader.exe");
    //message the user/do final stuff
    cinsert("debugging stopped!");
    varset("$hp", 0, true);
    varset("$pid", 0, true);
    waitclear();
    return 0;
}

bool cbDebugInit(const char* cmd)
{
    //dbg("cbInitDebug");
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

bool cbDebugSetBPX(const char* cmd) //bp addr [,name [,type]]
{
    dbg("cbDebugSetBPX");
    char argaddr[deflen]="";
    if(!argget(cmd, argaddr, 0, false))
        return true;
    char argname[deflen]="";
    argget(cmd, argname, 1, true);
    char argtype[deflen]="";
    argget(cmd, argtype, 2, true);
    _strlwr(argtype);
    uint addr=0;
    if(!valfromstring(argaddr, &addr, 0, 0))
    {
        cprintf("invalid addr: \"%s\"\n", argaddr);
        return true;
    }
    uint type=0;
    BP_TYPE list_type;
    if(strstr(argtype, "ss"))
    {
        type|=UE_SINGLESHOOT;
        list_type=BPSINGLESHOOT;
    }
    else
    {
        type|=UE_BREAKPOINT;
        list_type=BPNORMAL;
    }
    if(strstr(argtype, "long"))
        type|=UE_BREAKPOINT_LONG_INT3;
    else if(strstr(argtype, "ud2"))
        type|=UE_BREAKPOINT_TYPE_UD2;
    else
        type|=UE_BREAKPOINT_TYPE_INT3;
    if(IsBPXEnabled(addr) or !SetBPX(addr, type, (void*)cbUserBreakpoint))
    {
        cprintf("error setting breakpoint at "fhex"!\n", addr);
        return true;
    }
    bpnew(bplist, argname, addr, list_type);
    cprintf("breakpoint at "fhex" set!\n", addr);
    return true;
}
