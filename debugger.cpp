#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "console.h"
#include "threading.h"
#include "value.h"
#include "breakpoint.h"
#include "instruction.h"
#include "disasm\disasm.h"
#include "memory.h"

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
        if(cur->type==BPNORMAL or cur->type==BPSINGLESHOOT)
            if(IsBPXEnabled(cur->addr) and DeleteBPX(cur->addr))
                cur->enabled=false;
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
        if(cur->type==BPNORMAL or cur->type==BPSINGLESHOOT)
            if(!IsBPXEnabled(cur->addr) and SetBPX(cur->addr, cur->type, (void*)cbUserBreakpoint))
                cur->enabled=true;
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
    uint basesize;
    uint base=memfindbaseaddr(fdProcessInfo->hProcess, addr, &basesize);
    uint start=addr-16*16;
    if(start<base)
        start=base;
    uint disasmsize=500*16;
    uint end=(uint)base+basesize;
    if((start+disasmsize)>end)
        disasmsize=end-start;
    dbgdisablebpx();
    char* mem=(char*)emalloc(500*16);
    memset(mem, 0xFF, 500*16);
    readmem(fdProcessInfo->hProcess, (void*)start, mem, disasmsize, 0);
    dbgenablebpx();
    memset(&dinit, 0, sizeof(DISASM_INIT));
    DisasmInit(&dinit);
    DisasmDo(mem, start, 0, disasmsize, addr-start, GetContextData(UE_CIP));
    efree(mem);
}

static void cbUserBreakpoint()
{
    BREAKPOINT* cur=bpfind(bplist, 0, GetContextData(UE_CIP), 0, BPNORMAL);
    if(!cur)
        cinsert("breakpoint reached not in list!");
    else
    {
        const char* apiname=(const char*)ImporterGetAPINameFromDebugee(fdProcessInfo->hProcess, cur->addr);
        char log[deflen]="";
        if(apiname)
        {
            const char* dllname_=(const char*)ImporterGetDLLNameFromDebugee(fdProcessInfo->hProcess, cur->addr);
            char dllname[256]="";
            strcpy(dllname, dllname_);
            _strlwr(dllname);
            int len=strlen(dllname);
            for(int i=len-1; i!=0; i--)
                if(dllname[i]=='.')
                {
                    dllname[i]=0;
                    break;
                }
            if(cur->name)
                sprintf(log, "breakpoint \"%s\" at %s.%s ("fhex")!", cur->name, dllname, apiname, cur->addr);
            else
                sprintf(log, "breakpoint at %s.%s ("fhex")!", dllname, apiname, cur->addr);
        }
        else
        {
            if(cur->name)
                sprintf(log, "breakpoint \"%s\" at "fhex"!", cur->name, cur->addr);
            else
                sprintf(log, "breakpoint at "fhex"!", cur->addr);
        }
        cinsert(log);
        if(cur->type==BPSINGLESHOOT)
            bpdel(bplist, 0, cur->addr, BPNORMAL);
    }
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbHardwareBreakpoint(void* ExceptionAddress)
{
    uint cip=GetContextData(UE_CIP);
    BREAKPOINT* cur=bpfind(bplist, 0, (uint)ExceptionAddress, 0, BPHARDWARE);
    if(!cur)
        cinsert("hardware breakpoint reached not in list!");
    else
    {
        //TODO: type
        char log[50]="";
        if(cur->name)
            sprintf(log, "hardware breakpoint \"%s\" "fhex"!", cur->name, cur->addr);
        else
            sprintf(log, "hardware breakpoint "fhex"!", cur->addr);
        cinsert(log);
    }
    doDisasm(cip);
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbMemoryBreakpoint()
{
    uint cip=GetContextData(UE_CIP);
    uint base=memfindbaseaddr(fdProcessInfo->hProcess, cip, 0);
    BREAKPOINT* cur=bpfind(bplist, 0, base, 0, BPMEMORY);
    if(!cur)
        cinsert("breakpoint reached not in list!");
    else
    {
        char log[50]="";
        if(cur->name)
            sprintf(log, "memory breakpoint \"%s\" on "fhex"!", cur->name, cur->addr);
        else
            sprintf(log, "memory breakpoint on "fhex"!", cur->addr);
        cinsert(log);
    }
    if(!cur->oldbytes) //is auto-restoring?
        bpdel(bplist, 0, base, BPMEMORY);
    doDisasm(cip);
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

static int ecount=0;

static void cbAfterException(void* ExceptionData)
{
    EXCEPTION_DEBUG_INFO* edi=(EXCEPTION_DEBUG_INFO*)ExceptionData;
    uint addr=(uint)edi->ExceptionRecord.ExceptionAddress;
    SetNextDbgContinueStatus(DBG_CONTINUE);
    char msg[50]="";
    ecount++;
    sprintf(msg, "exception %d passed "fhex" (%.8X)", ecount, addr, edi->ExceptionRecord.ExceptionCode);
    cinsert(msg);
}

static void cbException(void* ExceptionData)
{
    EXCEPTION_DEBUG_INFO* edi=(EXCEPTION_DEBUG_INFO*)ExceptionData;
    uint addr=(uint)edi->ExceptionRecord.ExceptionAddress;
    if(edi->ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT)
        SetContextData(UE_CIP, (uint)edi->ExceptionRecord.ExceptionAddress);
    char msg[1024]="";
    sprintf(msg, "exception on "fhex" (%.8X)!", addr, edi->ExceptionRecord.ExceptionCode);
    cinsert(msg);
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbLoadDll(void* ExceptionData)
{
    puts("dll loaded!");
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static void cbSystemBreakpointStep()
{
    SetCustomHandler(UE_CH_UNHANDLEDEXCEPTION, (void*)cbException);
    SetCustomHandler(UE_CH_AFTERUNHANDLEDEXCEPTION, (void*)cbAfterException);
    //SetCustomHandler(UE_CH_PAGEGUARD, (void*)cbGuardPage);
    //SetCustomHandler(UE_CH_LOADDLL, (void*)cbLoadDll);
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
    cbSystemBreakpointStep();
    //StepInto((void*)cbSystemBreakpointStep);
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

static void cbRtrFinalStep()
{
    cinsert("returned!");
    doDisasm(GetContextData(UE_CIP));
    //lock
    lock(WAITID_RUN);
    wait(WAITID_RUN);
}

static unsigned char getCIPch()
{
    char ch;
    dbgdisablebpx();
    readmem(fdProcessInfo->hProcess, (void*)GetContextData(UE_CIP), &ch, 1, 0);
    dbgenablebpx();
    return ch;
}

static void cbRtrStep()
{
    unsigned int cipch=getCIPch();
    if(cipch==0xC3 or cipch==0xC2)
        cbRtrFinalStep();
    else
        StepOver((void*)cbRtrStep);
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
    ecount=0;
    bplist=bpinit();
    SetCustomHandler(UE_CH_SYSTEMBREAKPOINT, (void*)cbSystemBreakpoint);
    //SetEngineVariable(UE_ENGINE_PASS_ALL_EXCEPTIONS, true);
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

    char currentfolder[deflen]="";
    strcpy(currentfolder, arg1);
    int len=strlen(currentfolder);
    while(currentfolder[len]!='\\' and len!=0)
        len--;
    currentfolder[len]=0;
    if(DirExists(arg3))
        strcpy(currentfolder, arg3);
    INIT_STRUCT* init=(INIT_STRUCT*)emalloc(sizeof(INIT_STRUCT));
    memset(init, 0, sizeof(INIT_STRUCT));
    init->exe=arg1;
    init->commandline=commandline;
    if(*currentfolder)
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
    BREAKPOINT* found=bpfind(bplist, 0, addr, 0, BPNORMAL);
    if(IsBPXEnabled(addr) or !readmem(fdProcessInfo->hProcess, (void*)addr, &oldbytes, sizeof(short), 0) or found or !SetBPX(addr, type, (void*)cbUserBreakpoint))
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
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0, BPNORMAL);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0, BPNORMAL);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(bp->type!=BPNORMAL and bp->type!=BPSINGLESHOOT)
    {
        cputs("this breakpoint type cannot be enabled");
        return true;
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
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0, BPNORMAL);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0, BPNORMAL);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(bp->type!=BPNORMAL and bp->type!=BPSINGLESHOOT)
    {
        cputs("this breakpoint type cannot be disabled");
        return true;
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
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0, BPNORMAL);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return true;
        }
        bp=bpfind(bplist, 0, addr, 0, BPNORMAL);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return true;
        }
    }
    if(bp->type!=BPNORMAL and bp->type!=BPSINGLESHOOT)
    {
        cputs("this breakpoint type cannot be toggled");
        return true;
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
            if(cur->type==BPNORMAL or cur->type==BPSINGLESHOOT)
            {
                DeleteBPX(cur->addr);
                bpdel(bplist, 0, cur->addr, BPNORMAL);
            }
            cur=next;
            if(!cur)
                bNext=false;
        }
        //memset(bplist, 0, sizeof(BREAKPOINT));
        cputs("all breakpoints deleted!");
        return true;
    }
    BREAKPOINT* bp=bpfind(bplist, arg1, 0, 0, BPNORMAL);
    if(!bp)
    {
        uint addr=0;
        if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        {
            cprintf("invalid addr: \"%s\"\n", arg1);
            return false;
        }
        bp=bpfind(bplist, 0, addr, 0, BPNORMAL);
        if(!bp)
        {
            cprintf("no such breakpoint: \"%s\"\n", arg1);
            return false;
        }
    }
    if(!DeleteBPX(bp->addr))
    {
        cprintf("delete breakpoint failed: "fhex"\n", bp->addr);
        return false;
    }
    bpdel(bplist, 0, bp->addr, BPNORMAL);
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
    return cbDebugRun(cmd);
}

bool cbDebugStepOver(const char* cmd)
{
    StepOver((void*)cbStep);
    isStepping=true;
    return cbDebugRun(cmd);
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
    return cbDebugRun(cmd);
}

bool cbDebugHide(const char* cmd)
{
    if(HideDebugger(fdProcessInfo->hProcess, UE_HIDE_BASIC))
        cputs("debugger hidden");
    else
        cputs("something went wrong");
    return true;
}

bool cbDebugDisasm(const char* cmd)
{
    char arg1[deflen]="";
    uint addr=GetContextData(UE_EIP);
    if(argget(cmd, arg1, 0, true))
        if(!valfromstring(arg1, &addr, 0, 0, true, 0))
            addr=GetContextData(UE_CIP);
    doDisasm(addr);
    return true;
}

bool cbDebugMemoryBpx(const char* cmd)
{
    char arg1[deflen]=""; //addr
    if(!argget(cmd, arg1, 0, false))
        return true;
    uint addr;
    if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        return true;
    bool restore=false;
    char arg2[deflen]=""; //restore
    if(argget(cmd, arg2, 1, true))
        restore=true;
    uint type=UE_MEMORY;
    char arg3[deflen]=""; //type
    if(argget(cmd, arg3, 2, true))
    {
        switch(*arg3)
        {
        case 'r':
            type=UE_MEMORY_READ;
            break;
        case 'w':
            type=UE_MEMORY_WRITE;
            break;
        default:
            cputs("invalid type (argument ignored)");
            break;
        }
    }
    uint size=0;
    uint base=memfindbaseaddr(fdProcessInfo->hProcess, addr, &size);
    BREAKPOINT* found=bpfind(bplist, 0, base, 0, BPMEMORY);
    if(found or !SetMemoryBPXEx(base, size, type, restore, (void*)cbMemoryBreakpoint))
    {
        cputs("error setting memory breakpoint!");
        return true;
    }
    if(bpnew(bplist, 0, addr, restore, BPMEMORY))
        cprintf("memory breakpoint at "fhex" set!\n", addr);
    else
        cputs("problem setting breakpoint (report please)!");
    return true;
}

bool cbDebugRtr(const char* cmd)
{
    StepOver((void*)cbRtrStep);
    cbDebugRun(cmd);
    return true;
}

bool cbDebugSetHardwareBreakpoint(const char* cmd)
{
    char arg1[deflen]="";
    if(!argget(cmd, arg1, 0, false))
        return true;
    uint addr;
    if(!valfromstring(arg1, &addr, 0, 0, false, 0))
        return true;
    uint type=UE_HARDWARE_EXECUTE;
    char arg2[deflen]="";
    if(argget(cmd, arg2, 1, true))
    {
        if(strstr(arg2, "rw") or strstr(arg2, "readwrite"))
            type=UE_HARDWARE_READWRITE;
        else if(strstr(arg2, "w") or strstr(arg2, "write"))
            type=UE_HARDWARE_WRITE;
    }
    char arg3[deflen]="";
    uint size=UE_HARDWARE_SIZE_1;
    if(argget(cmd, arg3, 2, true))
    {
        if(!valfromstring(arg3, &size, 0, 0, false, 0))
            return true;
        switch(size)
        {
        case 2:
            size=UE_HARDWARE_SIZE_2;
            break;
        case 4:
            size=UE_HARDWARE_SIZE_4;
            break;
#ifdef _WIN64
        case 8:
            size=UE_HARDWARE_SIZE_8;
            break;
#endif // _WIN64
        default:
            puts("invalid size, using 1");
            break;
        }
        if(addr%size)
        {
            cprintf("address not aligned to %d\n", size);
            return true;
        }
    }
    DWORD drx=0;
    if(!GetUnusedHardwareBreakPointRegister(&drx))
    {
        puts("no free debug register");
        return true;
    }
    BREAKPOINT* found=bpfind(bplist, 0, addr, 0, BPHARDWARE);
    if(found or !SetHardwareBreakPoint(addr, drx, type, size, (void*)cbHardwareBreakpoint))
    {
        cputs("error setting hardware breakpoint!");
        return true;
    }
    if(bpnew(bplist, 0, addr, (drx<<8)|(type<<4)|size, BPHARDWARE))
        cprintf("hardware breakpoint at "fhex" set!\n", addr);
    else
        cputs("problem setting breakpoint (report please)!");
    return true;
}
