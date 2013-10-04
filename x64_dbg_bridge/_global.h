#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <windows.h>

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif //DLL_IMPORT
#ifndef DLL_EXPORT
#define DLL_IMPORT __declspec(dllimport)
#endif //DLL_IMPORT

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

//hInst GUI/DBG
extern HINSTANCE hInstGui;
extern HINSTANCE hInstDbg;

//GUI typedefs
typedef int (*GUIGUIINIT)(int, char**);

//GUI functions
extern GUIGUIINIT _gui_guiinit;

//DBG typedefs
typedef const char* (*DBGDBGINIT)(void);

//DBG functions
extern DBGDBGINIT _dbg_dbginit;

#endif // _GLOBAL_H
