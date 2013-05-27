#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

//defines
#define deflen 1024

//functions
bool arraycontains(const char* cmd_list, const char* cmd);
bool scmp(const char* a, const char* b);
void dbg(const char* a);
void formathex(char* string);
void formatdec(char* string);

#endif // _GLOBAL_H
