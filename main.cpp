#include "_global.h"
#include "argument.h"
#include "command.h"
#include "variable.h"
#include "instruction.h"
#include "debugger.h"

static bool cbStrLen(const char* cmd)
{
    char arg1[deflen]="";
    if(argget(cmd, arg1, 0, false))
        printf("\"%s\"[%d]\n", arg1, strlen(arg1));
    return true;
}

static bool cbExit(const char* cmd)
{
    return false;
}

static bool cbCls(const char* cmd)
{
    system("cls");
    return true;
}

static COMMAND* command_list=0;

static void registercommands()
{
    COMMAND* cmd=command_list=cmdinit();
    cmdnew(cmd, "exit\1x", cbExit, false); //quit debugger
    cmdnew(cmd, "strlen\1charcount\1ccount", cbStrLen, false); //get strlen, arg1:string
    cmdnew(cmd, "varnew\1var", cbInstrVar, false); //make a variable arg1:name,[arg2:value]
    cmdnew(cmd, "vardel", cbInstrVarDel, false); //delete a variable, arg1:variable name
    cmdnew(cmd, "mov\1set", cbInstrMov, false); //mov a variable, arg1:dest,arg2:src
    cmdnew(cmd, "cls", cbCls, false); //clear the screen
    cmdnew(cmd, "varlist", cbInstrVarList, false); //list variables[arg1:type filter]
    cmdnew(cmd, "InitDebug\1init\1initdbg", cbDebugInit, false); //init debugger arg1:exefile,[arg2:commandline]
    cmdnew(cmd, "run\1r\1go\1g", cbDebugRun, true); //unlock WAITID_RUN
    cmdnew(cmd, "epbreak", cbDebugEpBreak, true); //test command
}

int main()
{
    dbg("main");
    varinit();
    registercommands();
    cmdloop(command_list, cbBadCmd);
    return 0;
}
