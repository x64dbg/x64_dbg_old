#ifndef _ARGUMENT_H
#define _ARGUMENT_H

#include "_global.h"

//functions
bool getarg(const char* cmd, char* arg, int arg_num, bool optional);
int getargcount(const char* cmd);
void formatarg(char* cmd);

#endif
