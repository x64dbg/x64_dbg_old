#include "_global.h"
#include "argument.h"
#include "command.h"
#include "variable.h"
#include "instruction.h"

static const char* commands[]=
{
    "x\1exit", //1:quit debugger
    "strlen\1charcount\1ccount", //2:get strlen, arg1:string
    "var\1varnew", //3:make a variable arg1:name,[arg2:value]
    "vardel", //4:delete a variable, arg1:variable name
    "mov\1set", //5:mov a variable, arg1:dest,arg2:src
    "cls", //6:clear the screen
    "varlist", //7:list variables[arg1:type filter]
    "InitDebugger\1init\1initdbg", //8:init debugger arg1:exefile,[arg2:commandline]
};

static void CharacterCount(const char* arg1)
{
    printf("\"%s\"[%d]\n", arg1, strlen(arg1));
}

static bool cbMainCommandLoop(const char* cmd, COMMAND_LIST* cmd_list)
{
    //COMMAND_LIST cmd_list={sizeof(commands), commands};
    switch(cmdnum(cmd, cmd_list))
    {
    case 7: //varlist
        cbInstrVarList(cmd);
        break;
    case 6: //cls
        system("cls");
        break;
    case 5: //mov
        cbInstrMov(cmd);
        break;
    case 4: //vardel
        cbInstrVarDel(cmd);
        break;
    case 3: //var
        cbInstrVar(cmd);
        break;
    case 2: //strlen
    {
        char arg1[deflen]="";
        if(!argget(cmd, arg1, 0, false))
            break;
        CharacterCount(arg1);
    }
    break;
    case 1: //x
        return false;
        break;
    case 0: //variable/unknown command
        cbBadCmd(cmd);
        break;
    }
    return true;
}

int main()
{
    dbg("main");
    varinit();
    COMMAND_LIST cmd_list={sizeof(commands), commands};
    cmdloop(cbMainCommandLoop, &cmd_list);
    return 0;
}
