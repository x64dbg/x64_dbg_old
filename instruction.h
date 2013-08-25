#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "_global.h"

//functions
bool cbBadCmd(const char* cmd);
bool cbInstrVar(const char* cmd);
bool cbInstrVarDel(const char* cmd);
bool cbInstrMov(const char* cmd);
bool cbInstrVarList(const char* cmd);
bool cbInstrChd(const char* cmd);

#endif // _INSTRUCTIONS_H
