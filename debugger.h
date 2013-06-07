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
bool cbDebugSetBPXOptions(const char* cmd);
bool cbDebugSetBPX(const char* cmd);
bool cbDebugEnableBPX(const char* cmd);
bool cbDebugDisableBPX(const char* cmd);
bool cbDebugToggleBPX(const char* cmd);
bool cbDebugDeleteBPX(const char* cmd);
bool cbDebugBplist(const char* cmd);

#endif // _DEBUGGER_H
