#ifndef _GLOBAL_H
#define _GLOBAL_H

#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#define _WIN32_IE 0x0500

#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <shlwapi.h>

//enums
enum WAIT_ID
{
    WAITID_SYSBREAK=0,
    WAITID_RUN=1
};

//defines
#define deflen 1024

//functions
bool arraycontains(const char* cmd_list, const char* cmd);
bool scmp(const char* a, const char* b);
void dbg(const char* a);
void formathex(char* string);
void formatdec(char* string);

void waitclear();
void wait(WAIT_ID id);
void lock(WAIT_ID id);
void unlock(WAIT_ID id);

#endif // _GLOBAL_H
