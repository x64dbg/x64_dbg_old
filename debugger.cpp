#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"
#include "threading.h"
#include "value.h"
#include "breakpoint.h"
#include "instruction.h"
#include "disasm\disasm.h"

PROCESS_INFORMATION* fdProcessInfo;
static char szFileName[deflen]="";
bool bFileIsDll;
BREAKPOINT* bplist;
static bool isStepping=false;

DISASM_INIT dinit;

//static functions
static void cbStep();
static void cbSystemBreakpoint(void* ExceptionData);
static void cbEntryBreakpoint();
static void cbUserBreakpoint();

void dbgdisablebpx()
{
    BREAKPOINT* cur=bplist;
    if(!cur or !cur->addr)
        return;
    bool bNext=true;
    while(bNext)
    {
        if(DeleteBPX(cur->addr))
            cur->enabled=false;
        else
            cputs("error: DeleteBPX");
        cur=cur->next;
        if(!cur)
            bNext=false;
    }
}

void dbgenablebpx()
{
    BREAKPOINT* cur=bplist;
    if(!cur or !cur->addr)
        return;
    bool bNext=true;
    while(bNext)
    {
        if(SetBPX(cur->addr, cur->type, (void*)cbUserBreakpoint))
            cur->enabled=true;
        else
            cputs("error: SetBPX");
        cur=cur->next;
        if(!cur)
            bNext=false;
    }
}

bool dbgisrunning()
{
    if(!waitislocked(WAITID_RUN))
        return true;
    return false;
}

void doDisasm(uint addr)
{
    MEMORY_BASIC_INFORMATION mbi= {0};
    VirtualQueryEx(fdProcessInfo->hProcess, (void*)addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
    uint base=(uint)mbi.BaseAddress;
    uint start=addr-16*16;
    if(start<base)
        start=base;
    /*uint end=addr+16*16;
    if(end>(uint)(mbi.BaseAddress)+mbi.RegionSize)
        end=(uint)(mbi.BaseAddress)+mbi.RegionSize;*/
    char* mem=(char*)emalloc(300*16);
    memset(mem, 0x90, 300*16);
    dbgdisablebpx();
    uint size=300*16;
    uint end=(uint)(mbi.BaseAddress)+mbi.RegionSize;
    if((start+size)>end)
        size=end-start;
    ReadProcessMemory(fdProcessInfo->hProcess, (void*)start, mem, size, 0);
    dbgenablebpx();
    memset(&dinit, 0, sizeof(DISASM_INIT));
    DisasmInit(&dinit);
    DisasmDo(mem, start, 0, size, addr-start);
    efree(mem);
}

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
        if(cur->type==BPSINGLESHOOT)
            bpdel(bplist, 0, cur->addr);
    }
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbEntryBreakpoint()
{
    cinsert("entry point reached!");
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbAfterException(void* ExceptionData)
{
    SetNextDbgContinueStatus(DBG_CONTINUE);
}

static void cbException(void* ExceptionData)
{
    EXCEPTION_DEBUG_INFO* edi=(EXCEPTION_DEBUG_INFO*)ExceptionData;
    uint addr=(uint)edi->ExceptionRecord.ExceptionAddress;
    if(edi->ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT and IsBPXEnabled(addr))
        return;
    else if(edi->ExceptionRecord.ExceptionCode==EXCEPTION_SINGLE_STEP and isStepping)
        return;

    char msg[1024]="";
    sprintf(msg, "exception on %.8X!", addr);
    cinsert(msg);
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbSystemBreakpointStep()
{
    //SetCustomHandler(UE_CH_EVERYTHINGELSE, (void*)cbException);
    SetCustomHandler(UE_CH_AFTEREXCEPTIONPROCESSING, (void*)cbAfterException);
    cputs("system breakpoint reached!");
    doDisasm(GetContextData(UE_CIP));
    //unlock
    unlock(WAITID_SYSBREAK);
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbSystemBreakpoint(void* ExceptionData)
{
    //handle stuff (TLS, main entry, etc)
    SetCustomHandler(UE_CH_CREATEPROCESS, 0);
    StepInto((void*)cbSystemBreakpointStep);
}

static void cbStep()
{
    cinsert("stepped!");
    isStepping=false;
    doDisasm(GetContextData(UE_CIP));
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
    SetEngineVariable(UE_ENGINE_PASS_ALL_EXCEPTIONS, true);
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
    INIT_STRUCT* init=(INIT_STRUCT*)emalloc(sizeof(INIT_STRUCT));
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

bool cbStopDebug(const char* cmd)
{
    StopDebug();
    unlock(WAITID_RUN);
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

bool cbDebugSetBPXOptions(const char* cmd)
{
    char argtype[deflen]="";
    uint type=0;
    if(!argget(cmd, argtype, 0, false))
        return true;
    const char* a=0;
    if(strstr(argtype, "long"))
    {
        a="TYPE_LONG_INT3";
        type=UE_BREAKPOINT_TYPE_LONG_INT3;
    }
    else if(strstr(argtype, "ud2"))
    {
        a="TYPE_UD2";
        type=UE_BREAKPOINT_TYPE_UD2;
    }
    else if(strstr(argtype, "short"))
    {
        a="TYPE_INT3";
        type=UE_BREAKPOINT_TYPE_INT3;
    }
    else
    {
        cputs("invalid type specified!");
        return true;
    }
    SetBPXOptions(type);
    cprintf("default breakpoint type set to: %s\n", a);
    return true;
}

bool cbDebugSetBPX(const char* cmd) //bp addr [,name [,type]]
{
    char argaddr[deflen]="";
    if(!argget(cmd, argaddr, 0, true))
        if(!strncasecmp(cmd, "bp", 2))
            return cbBadCmd(cmd);
    if(!argget(cmd, argaddr, 0, false))
        return true;
    char argname[deflen]="";
    argget(cmd, argname, 1, true);
    char argtype[deflen]="";
    bool has_arg2=argget(cmd, argtype, 2, true);
    if(!has_arg2 and (scmp(argname, "ss") or scmp(argname, "long") or scmp(argname, "ud2")))
    {
        strcpy(argtype, argname);
        *argname=0;
    }

    _strlwr(argtype);
    uint addr=0;
    if(!valfromstring(argaddr, &addr, 0, 0, false, 0))
    {
        cprintf("invalid addr: \"%s\"\n", argaddr);
        return true;
    }
    if(addr==(uint)(GetPE32Data(szFileName, 0, UE_OEP)+GetPE32Data(szFileName, 0, UE_IMAGEBASE)))
    {
        cputs("entry breakpoint will be set automatically");
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
        type|=UE_BREAKPOINT_TYPE_LONG_INT3;
    else if(strstr(argtype, "ud2"))
        type|=UE_BREAKPOINT_TYPE_UD2;
    else if(strstr(argtype, "short"))
        type|=UE_BREAKPOINT_TYPE_INT3;
    short oldbytes;
    if(IsBPXEnabled(addr) or !ReadProcessMemory(fdProcessInfo->hProcess, (void*)addr, &oldbytes, sizeof(short), 0) or bpfind(bplist, 0, addr, 0) or !SetBPX(addr, type, (void*)cbUserBreakpoint))
    {
        cprintf("error setting breakpoint at "fhex"!\n", addr);
        return true;
    }
    if(bpnew(bplist, argname, addr, oldbytes, list_type))
        cprintf("breakpoint at "fhex" set!\n", addr);
    else
        cputs("problem setting breakpoint!");
    return true;
}

bool cbDebugEnableBPX(const char* cmd)
{
    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, true)) //enable all breakpoints
    {
        BREAKPOINT* cur=bplist;
        if(!cur or !cur->addr)
        {
            cputs("no breakpoints!");
            return true;
        }
        bool bNext=true;
        while(bNext)
        {
            if(!SetBPX(cur->addr, cur->type, (void*)cbUserBreakpoint))
                cprintf("could not enable %.8X\n", cur->addr);
            else
                cur->enabled=true;
            cur=cur->next;
            if(!cur)
                bNext=false;
        }
        cputs("all breakpoints enabled!");
        return true;
    }
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(bp->enabled)
    {
        cputs("breakpoint already enabled!");
        return true;
    }
    if(!SetBPX(bp->addr, bp->type, (void*)cbUserBreakpoint))
        cputs("could not enable breakpoint");
    else
        bp->enabled=true;
    return true;
}

bool cbDebugDisableBPX(const char* cmd)
{
    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, true)) //disable all breakpoints
    {
        BREAKPOINT* cur=bplist;
        if(!cur or !cur->addr)
        {
            cputs("no breakpoints!");
            return true;
        }
        bool bNext=true;
        while(bNext)
        {
            if(!DeleteBPX(cur->addr))
                cprintf("could not disable %.8X\n", cur->addr);
            else
                cur->enabled=false;
            cur=cur->next;
            if(!cur)
                bNext=false;
        }
        cputs("all breakpoints disabled!");
        return true;
    }
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(!bp->enabled)
    {
        cputs("breakpoint already disabled!");
        return true;
    }
    if(!DeleteBPX(bp->addr))
        cputs("could not disable breakpoint");
    else
        bp->enabled=false;
    return true;
}

bool cbDebugToggleBPX(const char* cmd)
{
    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, false))
        return true;
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    bool disable=bp->enabled;
    if(disable)
    {
        if(!DeleteBPX(bp->addr))
            cputs("could not disable breakpoint");
        else
        {
            bp->enabled=false;
            cputs("breakpoint disabled!");
        }
    }
    else
    {
        if(!SetBPX(bp->addr, bp->type, (void*)cbUserBreakpoint))
            cputs("could not disable breakpoint");
        else
        {
            bp->enabled=true;
            cputs("breakpoint enabled!");
        }
    }
    varset("$res", (uint)disable, false);
    return true;
}

bool cbDebugDeleteBPX(const char* cmd)
{
    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, true)) //delete all breakpoints
    {
        BREAKPOINT* cur=bplist;
        if(!cur or !cur->addr)
        {
            cputs("no breakpoints!");
            return true;
        }
        bool bNext=true;
        while(bNext)
        {
            BREAKPOINT* next=cur->next;
            DeleteBPX(cur->addr);
            if(cur->name)
                efree(cur->name);
            efree(cur);
            cur=next;
            if(!cur)
                bNext=false;
        }
        memset(bplist, 0, sizeof(BREAKPOINT));
        cputs("all breakpoints deleted!");
        return true;
    }
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(!DeleteBPX(bp->addr))
    {
        cprintf("delete breakpoint failed: "fhex"\n", bp->addr);
        return true;
    }
    bpdel(bplist, 0, bp->addr);
    return true;
}

bool cbDebugBplist(const char* cmd)
{
    BREAKPOINT* cur=bplist;
    dbg("cbDebugBplist");
    if(!cur or !cur->addr)
    {
        cputs("no breakpoints!");
        return true;
    }
    bool bNext=true;
    while(bNext)
    {
        const char* type=0;
        if(cur->type==BPNORMAL)
            type="BP";
        if(cur->type==BPSINGLESHOOT)
            type="SS";
        if(cur->type==BPHARDWARE)
            type="HW";
        if(cur->type==BPMEMORY)
            type="GP";
        bool enabled=cur->enabled;
        if(cur->name)
            cprintf("%d:%s:"fhex":\"%s\"\n", enabled, type, cur->addr, cur->name);
        else
            cprintf("%d:%s:"fhex"\n", enabled, type, cur->addr);
        cur=cur->next;
        if(!cur)
            bNext=false;
    }
    return true;
}

bool cbDebugStepInto(const char* cmd)
{
    StepInto((void*)cbStep);
    isStepping=true;
    if(!waitislocked(WAITID_RUN))
    {
        cputs("program is already running");
        return true;
    }
    unlock(WAITID_RUN);
    return true;
}

bool cbDebugStepOver(const char* cmd)
{
    StepOver((void*)cbStep);
    isStepping=true;
    if(!waitislocked(WAITID_RUN))
    {
        cputs("program is already running");
        return true;
    }
    unlock(WAITID_RUN);
    return true;
}

bool cbDebugSingleStep(const char* cmd)
{
    char arg1[deflen]="";
    uint stepcount=1;
    if(argget(cmd, arg1, 0, true))
    {
        if(!valfromstring(arg1, &stepcount, 0, 0, true, 0))
            stepcount=1;
    }

    SingleStep((DWORD)stepcount, (void*)cbStep);
    isStepping=true;
    if(!waitislocked(WAITID_RUN))
    {
        cputs("program is already running");
        return true;
    }
    unlock(WAITID_RUN);
    return true;
}

bool cbDebugHide(const char* cmd)
{
    if(HideDebugger(fdProcessInfo->hProcess, UE_HIDE_BASIC))
        cputs("debugger hidden");
    else
        cputs("something went wrong");
    return true;
}
