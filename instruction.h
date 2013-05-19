#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "_global.h"

//functions
void cbBadCmd(const char* cmd);
void cbInstrVar(const char* cmd);
void cbInstrVarDel(const char* cmd);
void cbInstrMov(const char* cmd);

#endif // _INSTRUCTIONS_H
