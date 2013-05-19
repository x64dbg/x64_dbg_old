#include "instruction.h"
#include "argument.h"
#include "variable.h"

void cbBadCmd(const char* cmd)
{
    unsigned int value=0;
    if(getvaluefromstring(cmd, &value, 0, 0)) //dump variable/value/register/etc
    {
        if(!isxdigit(*cmd) and *cmd!='.' and *cmd!='x') //prevent stupid 0=0 stuff
        {
            if(value>15)
                printf("%s=%X (%ud)\n", cmd, value, value);
            else
                printf("%s=%X\n", cmd, value);
        }
        else
        {
            if(value>15)
                printf("%X (%ud)\n", value, value);
            else
                printf("%X\n", value);
        }
    }
    else //unknown command
    {
        char cmd_[deflen]="";
        strcpy(cmd_, cmd);
        int len=strlen(cmd_);
        for(int i=0; i<len; i++)
        {
            if(cmd_[i]==' ')
            {
                cmd_[i]=0;
                return;
            }
        }
        if(*cmd_=='$')
            printf("unknown variable: \"%s\"\n", cmd_);
        else
            printf("unknown command: \"%s\"\n", cmd_);
    }
}

void cbInstrVar(const char* cmd)
{
    char arg1[deflen]="";
    char arg2[deflen]="";
    if(!getarg(cmd, arg1, 0, false)) //var name
        return;
    getarg(cmd, arg2, 1, true); //var value (optional)
    unsigned int value=0;
    if(!getvaluefromstring(arg2, &value, 0, 0))
    {
        printf("invalid value \"%s\"\n", arg2);
        return;
    }
    if(!varnew(arg1, (void*)value, VAR_USER))
        printf("error creating variable \"%s\"\n", arg1);
    else
    {
        if(value>15)
            printf("%s=%X (%ud)\n", arg1, value, value);
        else
            printf("%s=%X\n", arg1, value);
    }
}

void cbInstrVarDel(const char* cmd)
{
    char arg1[deflen]="";
    if(!getarg(cmd, arg1, 0, false)) //var name
        return;
    if(!vardel(arg1))
        printf("error deleting variable \"%s\"\n", arg1);
    else
        printf("deleted variable \"%s\"\n", arg1);
}

void cbInstrMov(const char* cmd)
{
    char arg1[deflen]="";
    char arg2[deflen]="";
    if(!getarg(cmd, arg1, 0, false)) //dest name
        return;
    if(!getarg(cmd, arg2, 1, false)) //src name
        return;
    unsigned int set_value=0;
    if(!getvaluefromstring(arg2, &set_value, 0, 0))
    {
        printf("invalid src \"%s\"\n", arg2);
        return;
    }
    if(!varset(arg1, (void*)set_value))
    {
        if(*arg1!='$')
        {
            printf("invalid dest \"%s\"\n", arg1);
            return;
        }
        varnew(arg1, (void*)set_value, VAR_USER);
    }
    unsigned int value=0;
    varget(arg1, &value, 0);
    if(value>15)
        printf("%s=%X (%ud)\n", arg1, value, value);
    else
        printf("%s=%X\n", arg1, value);
}
