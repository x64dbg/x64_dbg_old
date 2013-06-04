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
    //General variables
    varnew("$res\1$result", 0, VAR_SYSTEM);
    varnew("$res1\1$result1", 0, VAR_SYSTEM);
    varnew("$res2\1$result2", 0, VAR_SYSTEM);
    varnew("$res3\1$result3", 0, VAR_SYSTEM);
    varnew("$res4\1$result4", 0, VAR_SYSTEM);
    //InitDebug variables
    varnew("$hp\1$hProcess", 0, VAR_READONLY);
    varnew("$pid", 0, VAR_READONLY);
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
    var->value.value=value;
    if(!nonext)
    {
        VAR* cur=vars;
        while(cur->next)
            cur=cur->next;
        cur->next=var;
    }
    return true;
}

bool varget(const char* name, void* value, int* size, VAR_TYPE* type)
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
    *val=found->value.value;
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
    found->value.value=value;
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
