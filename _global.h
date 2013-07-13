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
#include <stdarg.h>

#include "dbg.h"

//defines
#define deflen 1024

#ifdef _WIN64 //defined by default
#define fhex "%.16llX"
#define fext "ll"
#define uint unsigned long long
#define sint long long
#else
#define fhex "%.8X"
#define fext ""
#define uint unsigned long
#define sint long
#endif // _WIN64

//functions
bool arraycontains(const char* cmd_list, const char* cmd);
bool scmp(const char* a, const char* b);
void formathex(char* string);
void formatdec(char* string);
bool FileExists(const char* file);
bool DirExists(const char* dir);

#endif // _GLOBAL_H
