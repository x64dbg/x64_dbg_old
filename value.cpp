#include "value.h"
#include "variable.h"

static bool isregister(const char* string)
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

    if(scmp(string, "dr0"))
        return true;
    if(scmp(string, "dr1"))
        return true;
    if(scmp(string, "dr2"))
        return true;
    if(scmp(string, "dr3"))
        return true;
    if(scmp(string, "dr6"))
        return true;
    if(scmp(string, "dr7"))
        return true;

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

    if(scmp(string, "cip"))
        return true;
    if(scmp(string, "csp"))
        return true;

    return false;
}

bool valfromstring(const char* string, uint* value, int* value_size, bool* isvar)
{
    if(!value)
        return false;
    if(!*string)
    {
        *value=0;
        return true;
    }
    if(*string=='$') //variable
    {
        if(isvar)
            *isvar=true;
        return varget(string, value, value_size, 0);
    }
    if(isregister(string)) //register
    {
        //TODO: getregister
        *value=0xFFFF;
        if(value_size)
            *value_size=sizeof(uint);
        if(isvar)
            *isvar=true;
        return true;
    }
    if(*string=='!') //flag
    {
        //TODO: getflag
        *value=1;
        if(value_size)
            *value_size=0;
        if(isvar)
            *isvar=true;
        return true;
    }
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
    if(value_size)
        *value_size=sizeof(uint);
    return true;
}
