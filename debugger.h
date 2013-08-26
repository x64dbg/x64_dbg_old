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
void dbgdisablebpx();
void dbgenablebpx();
bool dbgisrunning();
void doDisasm(uint addr);
//callbacks
bool cbDebugInit(const char* cmd);
bool cbStopDebug(const char* cmd);
bool cbDebugRun(const char* cmd);
bool cbDebugSetBPXOptions(const char* cmd);
bool cbDebugSetBPX(const char* cmd);
bool cbDebugEnableBPX(const char* cmd);
bool cbDebugDisableBPX(const char* cmd);
bool cbDebugToggleBPX(const char* cmd);
bool cbDebugDeleteBPX(const char* cmd);
bool cbDebugBplist(const char* cmd);
bool cbDebugStepInto(const char* cmd);
bool cbDebugStepOver(const char* cmd);
bool cbDebugSingleStep(const char* cmd);
bool cbDebugHide(const char* cmd);
bool cbDebugDisasm(const char* cmd);
bool cbDebugMemoryBpx(const char* cmd);
bool cbDebugRtr(const char* cmd);
bool cbDebugSetHardwareBreakpoint(const char* cmd);

//variables
extern PROCESS_INFORMATION* fdProcessInfo;

#endif // _DEBUGGER_H
