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

#ifdef _WIN64
typedef unsigned long long duint;
typedef long long dsint;
#else
typedef unsigned long duint;
typedef long dsint;
#endif //_WIN64

//hInst GUI/DBG
extern HINSTANCE hInstGui;
extern HINSTANCE hInstDbg;

//GUI typedefs
typedef int (*GUIGUIINIT)(int, char**);
typedef void (*GUICHANGECIP)(duint va, duint cip);

//GUI functions
extern GUIGUIINIT _gui_guiinit;
extern GUICHANGECIP _gui_disassembleAt;

//DBG typedefs
typedef const char* (*DBGDBGINIT)();
typedef duint (*DBGMEMFINDBASEADDR)(duint addr, duint* size);
typedef bool (*DBGMEMREAD)(duint addr, unsigned char* dest, duint size, duint* read);
typedef bool (*DBGDBGCMDEXEC)(const char* cmd);

//DBG functions
extern DBGDBGINIT _dbg_dbginit;
extern DBGMEMFINDBASEADDR _dbg_memfindbaseaddr;
extern DBGMEMREAD _dbg_memread;
extern DBGDBGCMDEXEC _dbg_dbgcmdexec;

#endif // _GLOBAL_H
