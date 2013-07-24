#include "value.h"
#include "variable.h"
#include "debugger.h"
#include "console.h"
#include "math.h"

static bool dosignedcalc=false;

bool valuesignedcalc()
{
    return dosignedcalc;
}

void valuesetsignedcalc(bool a)
{
    dosignedcalc=a;
}

bool isflag(const char* string)
{
    if(scmp(string, "cf"))
        return true;
    if(scmp(string, "pf"))
        return true;
    if(scmp(string, "af"))
        return true;
    if(scmp(string, "zf"))
        return true;
    if(scmp(string, "sf"))
        return true;
    if(scmp(string, "tf"))
        return true;
    if(scmp(string, "if"))
        return true;
    if(scmp(string, "df"))
        return true;
    if(scmp(string, "of"))
        return true;
    if(scmp(string, "rf"))
        return true;
    if(scmp(string, "vm"))
        return true;
    if(scmp(string, "ac"))
        return true;
    if(scmp(string, "vif"))
        return true;
    if(scmp(string, "vip"))
        return true;
    if(scmp(string, "id"))
        return true;
    return false;
}

bool isregister(const char* string)
{
    if(scmp(string, "eax"))
        return true;
    if(scmp(string, "ebx"))
        return true;
    if(scmp(string, "ecx"))
        return true;
    if(scmp(string, "edx"))
        return true;
    if(scmp(string, "edi"))
        return true;
    if(scmp(string, "esi"))
        return true;
    if(scmp(string, "ebp"))
        return true;
    if(scmp(string, "esp"))
        return true;
    if(scmp(string, "eip"))
        return true;
    if(scmp(string, "eflags"))
        return true;

    if(scmp(string, "ax"))
        return true;
    if(scmp(string, "bx"))
        return true;
    if(scmp(string, "cx"))
        return true;
    if(scmp(string, "dx"))
        return true;
    if(scmp(string, "si"))
        return true;
    if(scmp(string, "di"))
        return true;
    if(scmp(string, "bp"))
        return true;
    if(scmp(string, "sp"))
        return true;
    if(scmp(string, "ip"))
        return true;

    if(scmp(string, "ah"))
        return true;
    if(scmp(string, "al"))
        return true;
    if(scmp(string, "bh"))
        return true;
    if(scmp(string, "bl"))
        return true;
    if(scmp(string, "ch"))
        return true;
    if(scmp(string, "cl"))
        return true;
    if(scmp(string, "dh"))
        return true;
    if(scmp(string, "dl"))
        return true;
    if(scmp(string, "sih"))
        return true;
    if(scmp(string, "sil"))
        return true;
    if(scmp(string, "dih"))
        return true;
    if(scmp(string, "dil"))
        return true;
    if(scmp(string, "bph"))
        return true;
    if(scmp(string, "bpl"))
        return true;
    if(scmp(string, "sph"))
        return true;
    if(scmp(string, "spl"))
        return true;

    if(scmp(string, "dr0"))
        return true;
    if(scmp(string, "dr1"))
        return true;
    if(scmp(string, "dr2"))
        return true;
    if(scmp(string, "dr3"))
        return true;
    if(scmp(string, "dr6") or scmp(string, "dr4"))
        return true;
    if(scmp(string, "dr7") or scmp(string, "dr5"))
        return true;

    if(scmp(string, "cip"))
        return true;
    if(scmp(string, "csp"))
        return true;
    if(scmp(string, "cflags"))
        return true;
#ifndef _WIN64
    return false;
#endif // _WIN64
    if(scmp(string, "rax"))
        return true;
    if(scmp(string, "rbx"))
        return true;
    if(scmp(string, "rcx"))
        return true;
    if(scmp(string, "rdx"))
        return true;
    if(scmp(string, "rdi"))
        return true;
    if(scmp(string, "rsi"))
        return true;
    if(scmp(string, "rbp"))
        return true;
    if(scmp(string, "rsp"))
        return true;
    if(scmp(string, "rip"))
        return true;
    if(scmp(string, "rflags"))
        return true;
    if(scmp(string, "r8"))
        return true;
    if(scmp(string, "r9"))
        return true;
    if(scmp(string, "r10"))
        return true;
    if(scmp(string, "r11"))
        return true;
    if(scmp(string, "r12"))
        return true;
    if(scmp(string, "r13"))
        return true;
    if(scmp(string, "r14"))
        return true;
    if(scmp(string, "r15"))
        return true;
    if(scmp(string, "r8d"))
        return true;
    if(scmp(string, "r9d"))
        return true;
    if(scmp(string, "r10d"))
        return true;
    if(scmp(string, "r11d"))
        return true;
    if(scmp(string, "r12d"))
        return true;
    if(scmp(string, "r13d"))
        return true;
    if(scmp(string, "r14d"))
        return true;
    if(scmp(string, "r15d"))
        return true;
    if(scmp(string, "r8w"))
        return true;
    if(scmp(string, "r9w"))
        return true;
    if(scmp(string, "r10w"))
        return true;
    if(scmp(string, "r11w"))
        return true;
    if(scmp(string, "r12w"))
        return true;
    if(scmp(string, "r13w"))
        return true;
    if(scmp(string, "r14w"))
        return true;
    if(scmp(string, "r15w"))
        return true;
    if(scmp(string, "r8b"))
        return true;
    if(scmp(string, "r9b"))
        return true;
    if(scmp(string, "r10b"))
        return true;
    if(scmp(string, "r11b"))
        return true;
    if(scmp(string, "r12b"))
        return true;
    if(scmp(string, "r13b"))
        return true;
    if(scmp(string, "r14b"))
        return true;
    if(scmp(string, "r15b"))
        return true;
    return false;
}

static int getflag(uint eflags, const char* string)
{
    if(scmp(string, "cf"))
        return (int)(eflags&0x1);
    if(scmp(string, "pf"))
        return (int)(eflags&0x4);
    if(scmp(string, "af"))
        return (int)(eflags&0x10);
    if(scmp(string, "zf"))
        return (int)(eflags&0x40);
    if(scmp(string, "sf"))
        return (int)(eflags&0x80);
    if(scmp(string, "tf"))
        return (int)(eflags&0x100);
    if(scmp(string, "if"))
        return (int)(eflags&0x200);
    if(scmp(string, "df"))
        return (int)(eflags&0x400);
    if(scmp(string, "of"))
        return (int)(eflags&0x800);
    if(scmp(string, "rf"))
        return (int)(eflags&0x10000);
    if(scmp(string, "vm"))
        return (int)(eflags&0x20000);
    if(scmp(string, "ac"))
        return (int)(eflags&0x40000);
    if(scmp(string, "vif"))
        return (int)(eflags&0x80000);
    if(scmp(string, "vip"))
        return (int)(eflags&0x100000);
    if(scmp(string, "id"))
        return (int)(eflags&0x200000);
    return 0;
}

static uint getregister(int* size, const char* string)
{
    if(size)
        *size=4;
    if(scmp(string, "eax"))
    {
        return GetContextData(UE_EAX);
    }
    if(scmp(string, "ebx"))
    {
        return GetContextData(UE_EBX);
    }
    if(scmp(string, "ecx"))
    {
        return GetContextData(UE_ECX);
    }
    if(scmp(string, "edx"))
    {
        return GetContextData(UE_EDX);
    }
    if(scmp(string, "edi"))
    {
        return GetContextData(UE_EDI);
    }
    if(scmp(string, "esi"))
    {
        return GetContextData(UE_ESI);
    }
    if(scmp(string, "ebp"))
    {
        return GetContextData(UE_EBP);
    }
    if(scmp(string, "esp"))
    {
        return GetContextData(UE_ESP);
    }
    if(scmp(string, "eip"))
    {
        return GetContextData(UE_EIP);
    }
    if(scmp(string, "eflags"))
    {
        return GetContextData(UE_EFLAGS);
    }

    if(size)
        *size=2;
    if(scmp(string, "ax"))
    {
        uint val=GetContextData(UE_EAX);
        return val&0xFFFF;
    }
    if(scmp(string, "bx"))
    {
        uint val=GetContextData(UE_EBX);
        return val&0xFFFF;
    }
    if(scmp(string, "cx"))
    {
        uint val=GetContextData(UE_ECX);
        return val&0xFFFF;
    }
    if(scmp(string, "dx"))
    {
        uint val=GetContextData(UE_EDX);
        return val&0xFFFF;
    }
    if(scmp(string, "si"))
    {
        uint val=GetContextData(UE_ESI);
        return val&0xFFFF;
    }
    if(scmp(string, "di"))
    {
        uint val=GetContextData(UE_EDI);
        return val&0xFFFF;
    }
    if(scmp(string, "bp"))
    {
        uint val=GetContextData(UE_EBP);
        return val&0xFFFF;
    }
    if(scmp(string, "sp"))
    {
        uint val=GetContextData(UE_ESP);
        return val&0xFFFF;
    }
    if(scmp(string, "ip"))
    {
        uint val=GetContextData(UE_EIP);
        return val&0xFFFF;
    }

    if(size)
        *size=1;
    if(scmp(string, "ah"))
    {
        uint val=GetContextData(UE_EAX);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "al"))
    {
        uint val=GetContextData(UE_EAX);
        return val&0xFF;
    }
    if(scmp(string, "bh"))
    {
        uint val=GetContextData(UE_EBX);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "bl"))
    {
        uint val=GetContextData(UE_EBX);
        return val&0xFF;
    }
    if(scmp(string, "ch"))
    {
        uint val=GetContextData(UE_ECX);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "cl"))
    {
        uint val=GetContextData(UE_ECX);
        return val&0xFF;
    }
    if(scmp(string, "dh"))
    {
        uint val=GetContextData(UE_EDX);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "dl"))
    {
        uint val=GetContextData(UE_EDX);
        return val&0xFF;
    }
    if(scmp(string, "sih"))
    {
        uint val=GetContextData(UE_ESI);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "sil"))
    {
        uint val=GetContextData(UE_ESI);
        return val&0xFF;
    }
    if(scmp(string, "dih"))
    {
        uint val=GetContextData(UE_EDI);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "dil"))
    {
        uint val=GetContextData(UE_EDI);
        return val&0xFF;
    }
    if(scmp(string, "bph"))
    {
        uint val=GetContextData(UE_EBP);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "bpl"))
    {
        uint val=GetContextData(UE_EBP);
        return val&0xFF;
    }
    if(scmp(string, "sph"))
    {
        uint val=GetContextData(UE_ESP);
        return (val>>8)&0xFF;
    }
    if(scmp(string, "spl"))
    {
        uint val=GetContextData(UE_ESP);
        return val&0xFF;
    }

    if(size)
        *size=sizeof(uint);
    if(scmp(string, "dr0"))
    {
        return GetContextData(UE_DR0);
    }
    if(scmp(string, "dr1"))
    {
        return GetContextData(UE_DR1);
    }
    if(scmp(string, "dr2"))
    {
        return GetContextData(UE_DR2);
    }
    if(scmp(string, "dr3"))
    {
        return GetContextData(UE_DR3);
    }
    if(scmp(string, "dr6") or scmp(string, "dr4"))
    {
        return GetContextData(UE_DR6);
    }
    if(scmp(string, "dr7") or scmp(string, "dr5"))
    {
        return GetContextData(UE_DR7);
    }

    if(scmp(string, "cip"))
    {
        return GetContextData(UE_CIP);
    }
    if(scmp(string, "csp"))
    {
        return GetContextData(UE_CSP);
    }
    if(scmp(string, "cflags"))
    {
        return GetContextData(UE_CFLAGS);
    }

#ifndef _WIN64
    if(size)
        *size=0;
    return 0;
#endif // _WIN64

    if(size)
        *size=8;
    if(scmp(string, "rax"))
    {
        return GetContextData(UE_RAX);
    }
    if(scmp(string, "rbx"))
    {
        return GetContextData(UE_RBX);
    }
    if(scmp(string, "rcx"))
    {
        return GetContextData(UE_RCX);
    }
    if(scmp(string, "rdx"))
    {
        return GetContextData(UE_RDX);
    }
    if(scmp(string, "rdi"))
    {
        return GetContextData(UE_RDI);
    }
    if(scmp(string, "rsi"))
    {
        return GetContextData(UE_RSI);
    }
    if(scmp(string, "rbp"))
    {
        return GetContextData(UE_RBP);
    }
    if(scmp(string, "rsp"))
    {
        return GetContextData(UE_RSP);
    }
    if(scmp(string, "rip"))
    {
        return GetContextData(UE_RIP);
    }
    if(scmp(string, "rflags"))
    {
        return GetContextData(UE_RFLAGS);
    }
    if(scmp(string, "r8"))
    {
        return GetContextData(UE_R8);
    }
    if(scmp(string, "r9"))
    {
        return GetContextData(UE_R9);
    }
    if(scmp(string, "r10"))
    {
        return GetContextData(UE_R10);
    }
    if(scmp(string, "r11"))
    {
        return GetContextData(UE_R11);
    }
    if(scmp(string, "r12"))
    {
        return GetContextData(UE_R12);
    }
    if(scmp(string, "r13"))
    {
        return GetContextData(UE_R13);
    }
    if(scmp(string, "r14"))
    {
        return GetContextData(UE_R14);
    }
    if(scmp(string, "r15"))
    {
        return GetContextData(UE_R15);
    }

    if(size)
        *size=4;
    if(scmp(string, "r8d"))
    {
        return GetContextData(UE_R8)&0xFFFFFFFF;
    }
    if(scmp(string, "r9d"))
    {
        return GetContextData(UE_R9)&0xFFFFFFFF;
    }
    if(scmp(string, "r10d"))
    {
        return GetContextData(UE_R10)&0xFFFFFFFF;
    }
    if(scmp(string, "r11d"))
    {
        return GetContextData(UE_R11)&0xFFFFFFFF;
    }
    if(scmp(string, "r12d"))
    {
        return GetContextData(UE_R12)&0xFFFFFFFF;
    }
    if(scmp(string, "r13d"))
    {
        return GetContextData(UE_R13)&0xFFFFFFFF;
    }
    if(scmp(string, "r14d"))
    {
        return GetContextData(UE_R14)&0xFFFFFFFF;
    }
    if(scmp(string, "r15d"))
    {
        return GetContextData(UE_R15)&0xFFFFFFFF;
    }

    if(size)
        *size=2;
    if(scmp(string, "r8w"))
    {
        return GetContextData(UE_R8)&0xFFFF;
    }
    if(scmp(string, "r9w"))
    {
        return GetContextData(UE_R9)&0xFFFF;
    }
    if(scmp(string, "r10w"))
    {
        return GetContextData(UE_R10)&0xFFFF;
    }
    if(scmp(string, "r11w"))
    {
        return GetContextData(UE_R11)&0xFFFF;
    }
    if(scmp(string, "r12w"))
    {
        return GetContextData(UE_R12)&0xFFFF;
    }
    if(scmp(string, "r13w"))
    {
        return GetContextData(UE_R13)&0xFFFF;
    }
    if(scmp(string, "r14w"))
    {
        return GetContextData(UE_R14)&0xFFFF;
    }
    if(scmp(string, "r15w"))
    {
        return GetContextData(UE_R15)&0xFFFF;
    }

    if(size)
        *size=1;
    if(scmp(string, "r8b"))
    {
        return GetContextData(UE_R8)&0xFF;
    }
    if(scmp(string, "r9b"))
    {
        return GetContextData(UE_R9)&0xFF;
    }
    if(scmp(string, "r10b"))
    {
        return GetContextData(UE_R10)&0xFF;
    }
    if(scmp(string, "r11b"))
    {
        return GetContextData(UE_R11)&0xFF;
    }
    if(scmp(string, "r12b"))
    {
        return GetContextData(UE_R12)&0xFF;
    }
    if(scmp(string, "r13b"))
    {
        return GetContextData(UE_R13)&0xFF;
    }
    if(scmp(string, "r14b"))
    {
        return GetContextData(UE_R14)&0xFF;
    }
    if(scmp(string, "r15b"))
    {
        return GetContextData(UE_R15)&0xFF;
    }

    if(size)
        *size=0;
    return 0;
}

bool valfromstring(const char* string, uint* value, int* value_size, bool* isvar, bool silent)
{
    if(!value)
        return false;
    else if(!*string)
    {
        *value=0;
        return true;
    }
    else if(mathcontains(string)) //handle math
    {
        int len=strlen(string);
        char* string_=(char*)malloc(len+256);
        strcpy(string_, string);
        int add=0;
        while(mathisoperator(string_[add])>2)
            add++;
        if(!mathhandlebrackets(string_+add))
        {
            free(string_);
            return false;
        }
        bool ret=mathfromstring(string_+add, value);
        free(string_);
        return ret;
    }
    else if(*string=='@')
    {
        if(!IsFileBeingDebugged())
        {
            if(!silent)
                cputs("not debugging");
            *value=0;
            if(value_size)
                *value_size=0;
            if(isvar)
                *isvar=true;
            return true;
        }
        int read_size=sizeof(uint);
        int add=1;
        if(string[2]==':' and isdigit((string[1])))
        {
            add+=2;
            int new_size=string[1]-0x30;
            if(new_size<read_size)
                read_size=new_size;
        }
        if(!valfromstring(string+add, value, 0, 0, false))
            return false;
        uint addr=*value;
        *value=0;
        bool isrunning=dbgisrunning();
        if(!isrunning)
            dbgdisablebpx();
        bool rpm=ReadProcessMemory(fdProcessInfo->hProcess, (void*)addr, value, read_size, 0);
        if(!isrunning)
            dbgenablebpx();
        if(!rpm)
        {
            if(!silent)
                cputs("failed to read memory");
            return false;
        }
        if(value_size)
            *value_size=read_size;
        if(isvar)
            *isvar=true;
        return true;
    }
    else if(isregister(string)) //register
    {
        if(!IsFileBeingDebugged())
        {
            if(!silent)
                cputs("not debugging!");
            *value=0;
            if(value_size)
                *value_size=0;
            if(isvar)
                *isvar=true;
            return true;
        }
        *value=getregister(value_size, string);
        if(isvar)
            *isvar=true;
        return true;
    }
    else if(*string=='!' and isflag(string+1)) //flag
    {
        if(!IsFileBeingDebugged())
        {
            if(!silent)
                cputs("not debugging");
            *value=0;
            if(value_size)
                *value_size=0;
            if(isvar)
                *isvar=true;
            return true;
        }
        uint eflags=GetContextData(UE_CFLAGS);
        if(getflag(eflags, string+1))
            *value=1;
        else
            *value=0;
        if(value_size)
            *value_size=0;
        if(isvar)
            *isvar=true;
        return true;
    }
    else if(varget(string, value, value_size, 0)) //variable?
    {
        if(isvar)
            *isvar=true;
        return true;
    }

    if(value_size)
        *value_size=0;
    if(isvar)
        *isvar=false;
    if(*string=='.') //decimal value
    {
        int len=strlen(string+1);
        if(!string[1])
            return false;
        for(int i=0; i<len; i++)
            if(!isdigit(string[i+1]))
                return false;
        sscanf(string+1, "%"fext"u", value);
        return true;
    }
    //hexadecimal value
    int inc=0;
    if(*string=='x')
        inc=1;
    int len=strlen(string+inc);
    if(!string[inc])
        return false;
    for(int i=0; i<len; i++)
        if(!isxdigit(string[i+inc]))
            return false;
    sscanf(string+inc, "%"fext"x", value);
    return true;
}
