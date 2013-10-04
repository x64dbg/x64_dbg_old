#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "_global.h"

//functions
void cinsert(const char* text);
int consolegetlasty();
int consolesetlasty();
int cputs(const char* text);
int cprintf(const char* format, ...);

#endif // _CONSOLE_H
