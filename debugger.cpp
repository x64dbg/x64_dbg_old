#include "debugger.h"
#include "argument.h"
#include "variable.h"
#include "TitanEngine.h"

bool cbInitDebug(const char* cmd)
{
    char arg1[deflen]="";
    char arg2[deflen]="";
    char arg3[deflen]="";
    if(!argget(cmd, arg1, 0, false))
        return true;
    argget(cmd, arg2, 1, true);
    argget(cmd, arg3, 2, true);
    printf("InitDebug(\"%s\", \"%s\", \"%s\");\n", arg1, arg2, arg3);
    return true;
}
