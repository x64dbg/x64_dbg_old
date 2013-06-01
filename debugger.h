#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include "_global.h"
#include "TitanEngine.h"

//structures
struct INIT_STRUCT
{
    char* exe;
    char* commandline;
    char* currentfolder;
};

//functions
bool cbDebugInit(const char* cmd);
bool cbDebugRun(const char* cmd);
bool cbDebugSetBPX(const char* cmd);

#endif // _DEBUGGER_H
