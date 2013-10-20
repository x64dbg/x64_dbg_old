#ifndef _BRIDGEMAIN_H_
#define _BRIDGEMAIN_H_

#include <windows.h>

#ifdef _WIN64
typedef unsigned long long duint;
typedef long long dsint;
#else
typedef unsigned long duint;
typedef long dsint;
#endif //_WIN64

#ifndef DLL_IMPEXP
#ifdef BUILD_BRIDGE
#define DLL_IMPEXP __declspec(dllexport)
#else
#define DLL_IMPEXP __declspec(dllimport)
#endif //BUILD_BRIDGE
#endif //DLL_IMPEXP

#ifdef __cplusplus
extern "C"
{
#endif

//Bridge functions
const char* DLL_IMPEXP BridgeInit();
const char* DLL_IMPEXP BridgeStart();
void* DLL_IMPEXP BridgeAlloc(size_t size);
void DLL_IMPEXP BridgeFree(void* ptr);

//Debugger structs
struct MEMPAGE
{
    MEMORY_BASIC_INFORMATION mbi;
    char mod[16];
};

struct MEMMAP
{
    int count;
    MEMPAGE* page;
};

//Debugger enums
enum DBGSTATE
{
    initialized,
    paused,
    running,
    stopped
};

//Debugger functions
void DLL_IMPEXP DbgMemRead(duint va, unsigned char* dest, duint size);
duint DLL_IMPEXP DbgMemGetPageSize(duint base);
duint DLL_IMPEXP DbgMemFindBaseAddr(duint addr, duint* size);
bool DLL_IMPEXP DbgCmdExec(const char* cmd);
bool DLL_IMPEXP DbgMemMap(MEMMAP* memmap);

//GUI functions
void DLL_IMPEXP GuiDisasmAt(duint addr, duint cip);
void DLL_IMPEXP GuiSetDebugState(DBGSTATE state);
void DLL_IMPEXP GuiAddLogMessage(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // _BRIDGEMAIN_H_
