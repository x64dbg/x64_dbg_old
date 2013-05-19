#include "_global.h"
#include "argument.h"
#include "command.h"
#include "variable.h"
#include "instruction.h"

static const char* commands[]=
{
    "x\1exit", //1:quit debugger
    "strlen\1charcount\1ccount", //2:get strlen of arg1
    "var\1varnew", //3:make a variable
    "vardel", //4:delete a variable
    "mov\1set", //5:mov a variable
    "cls" //6:clear the screen
};

void CharacterCount(const char* arg1)
{
    printf("\"%s\"[%d]\n", arg1, strlen(arg1));
}

bool cbCommandLoop(const char* cmd)
{
    switch(cmdnum(cmd, commands, sizeof(commands))+1)
    {
    case 0: //variable/unknown command
        cbBadCmd(cmd);
    break;

    case 1: //x
        return false;
    break;

    case 2: //strlen
    {
        char arg1[deflen]="";
        if(!getarg(cmd, arg1, 0, false))
            break;
        CharacterCount(arg1);
    }
    break;

    case 3: //var
        cbInstrVar(cmd);
    break;

    case 4: //vardel
        cbInstrVarDel(cmd);
    break;

    case 5: //mov
        cbInstrMov(cmd);
    break;

    case 6: //cls
        system("cls");
    break;
    }
    return true;
}

int main()
{
    varinit();
    commandloop(cbCommandLoop);
    return 0;
}
