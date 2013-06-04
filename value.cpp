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

bool valfromstring(const char* string, void* value, int* value_size, bool* isvar)
{
    if(!*string)
    {
        void** val=(void**)value;
        *val=0;
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
        void** val=(void**)value;
        *val=(void*)0xFFFF;
        if(value_size)
            *value_size=sizeof(uint);
        if(isvar)
            *isvar=true;
        return true;
    }
    if(*string=='!') //flag
    {
        //TODO: getflag
        void** val=(void**)value;
        *val=(void*)1;
        if(value_size)
            *value_size=0;
        if(isvar)
            *isvar=true;
        return true;
    }
    char* temp=(char*)malloc(strlen(string)+1);
    strcpy(temp, string);
    if(*string=='.') //decimal value
    {
        temp++;
        int len=strlen(temp);
        for(int i=0; i<len; i++)
            if(!isdigit(temp[i]))
                return false;
        if(!*temp)
            return false;
        sscanf(temp, "%u", (uint*)value);
        free(temp);
        return true;
    }
    //hexadecimal value
    if(*string=='x')
        temp++;
    int len=strlen(temp);
    for(int i=0; i<len; i++)
        if(!isxdigit(temp[i]))
            return false;
    if(!*temp)
        return false;
    sscanf(temp, "%x", (uint*)value);
    if(value_size)
        *value_size=sizeof(uint);
    free(temp);
    return true;
}
