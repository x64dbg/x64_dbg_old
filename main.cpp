#include "_global.h"
#include "argument.h"
#include "command.h"
#include "variable.h"

static const char* commands[]=
{
    "x\1exit", //1:quit debugger
    "charcount\1ccount", //2:get strlen of arg1
    "var\1varnew", //3:make a variable
    "vardel", //4:delete a variable
    "mov\1set", //5:mov a variable
    "cls" //6:clear the screen
};

void CharacterCount(const char* arg1)
{
    printf("\"%s\"[%d]\n", arg1, strlen(arg1));
}

bool cbMainLoop(const char* cmd)
{
    switch(cmdnum(cmd, commands, sizeof(commands))+1)
    {
    case 0: //unknown command
    {
        unsigned int value=0;
        if(getvaluefromstring(cmd, &value)) //dump variable/value/register/etc
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
        else
        {
            char cmd_[deflen]="";
            strcpy(cmd_, cmd);
            int len=strlen(cmd_);
            for(int i=0; i<len; i++)
            {
                if(cmd_[i]==' ')
                {
                    cmd_[i]=0;
                    break;
                }
            }
            printf("unknown command: \"%s\"\n", cmd_);
        }
    }
    break;

    case 1: //x
    {
        return false;
    }
    break;

    case 2: //charcount
    {
        char arg1[deflen]="";
        if(!getarg(cmd, arg1, 0, false))
            break;
        CharacterCount(arg1);
    }
    break;

    case 3: //var
    {
        char arg1[deflen]="";
        char arg2[deflen]="";
        if(!getarg(cmd, arg1, 0, false)) //var name
            break;
        getarg(cmd, arg2, 1, true); //var value (optional)
        unsigned int value=0;
        if(!getvaluefromstring(arg2, &value))
        {
            printf("invalid argument \"%s\"\n", arg2);
            break;
        }
        if(!varnew(arg1, (void*)value, VAR_USER))
            printf("error creating variable \"%s\"\n", arg1);
        else
        {
            if(value>15)
                printf("created variable \"%s\" with value %X (%ud)\n", arg1, value, value);
            else
                printf("created variable \"%s\" with value %X\n", arg1, value);
        }

    }
    break;

    case 4: //vardel
    {
        char arg1[deflen]="";
        if(!getarg(cmd, arg1, 0, false)) //var name
            break;
        if(!vardel(arg1))
            printf("error deleting variable \"%s\"\n", arg1);
        else
            printf("deleted variable \"%s\"\n", arg1);
    }
    break;

    case 5: //mov
    {
        char arg1[deflen]="";
        char arg2[deflen]="";
        if(!getarg(cmd, arg1, 0, false)) //dest name
            break;
        if(!getarg(cmd, arg2, 1, false)) //src name
            break;
        unsigned int set_value=0;
        if(!getvaluefromstring(arg2, &set_value))
        {
            printf("invalid src \"%s\"\n", arg2);
            break;
        }
        if(!varset(arg1, (void*)set_value))
            printf("error setting dest \"%s\"\n", arg1);
        else
        {
            unsigned int value=0;
            varget(arg1, &value, 0);
            if(value>15)
                printf("%s=%X (%ud)\n", arg1, value, value);
            else
                printf("%s=%X\n", arg1, value);
        }
    }
    break;

    case 6: //cls
    {
        system("cls");
    }
    break;
    }
    return true;
}

int main()
{
    varinit();
    commandloop(cbMainLoop);
    return 0;
}
