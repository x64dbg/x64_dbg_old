#include "variable.h"

static VAR* vars;

static VAR* varfind(const char* name, VAR** link)
{
    dbg("varfind");
    VAR* cur=vars;
    if(!cur)
        return 0;
    VAR* prev=0;
    while(cur)
    {
        if(arraycontains(cur->name, name))
        {
            if(link)
                *link=prev;
            return cur;
        }
        prev=cur;
        cur=cur->next;
    }
    return 0;
}

void varinit()
{
    dbg("varinit");
    vars=(VAR*)malloc(sizeof(VAR));
    memset(vars, 0, sizeof(VAR));
    varnew("$res\1$result", 0, VAR_SYSTEM);
}

VAR* vargetptr()
{
    return vars;
}

bool varnew(const char* name_, void* value, VAR_TYPE type)
{
    dbg("varnew");
    if(!name_)
        return false;
    char* name=(char*)malloc(strlen(name_)+2);
    if(*name_!='$')
    {
        *name='$';
        strcpy(name+1, name_);
    }
    else
        strcpy(name, name_);
    if(!name[1])
        return false;
    if(varfind(name, 0))
    {
        free(name);
        return false;
    }
    VAR* var;
    bool nonext=false;
    if(!vars->name)
    {
        nonext=true;
        var=vars;
    }
    else
        var=(VAR*)malloc(sizeof(VAR));
    memset(var, 0, sizeof(VAR));
    var->name=name;
    var->type=type;
    var->value=value;
    if(!nonext)
    {
        VAR* cur=vars;
        while(cur->next)
            cur=cur->next;
        cur->next=var;
    }
    return true;
}

bool varget(const char* name, void* value, VAR_TYPE* type)
{
    dbg("varget");
    VAR* found=varfind(name, 0);
    if(!found)
        return false;
    if(!value)
        return false;
    if(*name!='$')
        return false;
    if(type)
        *type=found->type;
    void** val=(void**)value;
    *val=found->value;
    return true;
}

bool varset(const char* name, void* value, bool setreadonly)
{
    dbg("varset");
    VAR* found=varfind(name, 0);
    if(!found)
        return false;
    if(!setreadonly and found->type==VAR_READONLY)
        return false;
    found->value=value;
    return true;
}

bool vardel(const char* name_, bool delsystem)
{
    dbg("vardel");
    char* name=(char*)malloc(strlen(name_)+2);
    if(*name_!='$')
    {
        *name='$';
        strcpy(name+1, name_);
    }
    else
        strcpy(name, name_);
    VAR* prev=0;
    VAR* found=varfind(name, &prev);
    free(name);
    if(!found)
        return false;
    VAR_TYPE type=found->type;
    if(!delsystem and type!=VAR_USER)
        return false;
    free(found->name);
    if(found==vars)
    {
        VAR* next=vars->next;
        if(next)
        {
            memcpy(vars, vars->next, sizeof(VAR));
            vars->next=next->next;
            free(next);
        }
        else
            memset(vars, 0, sizeof(VAR));
    }
    else
    {
        prev->next=found->next;
        free(found);
    }
    return true;
}

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

bool getvaluefromstring(const char* string, void* value, int* value_size, VAR_TYPE* var_type, bool* isvar)
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
        return varget(string, value, var_type);
    }
    if(isregister(string)) //register
    {
        //TODO: getregister
        void** val=(void**)value;
        *val=(void*)0xFFFF;
        if(value_size)
            *value_size=sizeof(unsigned int);
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
        sscanf(temp, "%u", (unsigned int*)value);
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
    sscanf(temp, "%x", (unsigned int*)value);
    if(value_size)
        *value_size=sizeof(unsigned int);
    free(temp);
    return true;
}
