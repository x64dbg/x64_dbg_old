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

//Debugger defines
#define MAX_LABEL_SIZE 256
#define MAX_COMMENT_SIZE 256

//Debugger enums
enum DBGSTATE
{
    initialized,
    paused,
    running,
    stopped
};

enum SEGMENTREG
{
    SEG_DEFAULT,
    SEG_ES,
    SEG_DS,
    SEG_FS,
    SEG_GS,
    SEG_CS,
    SEG_SS
};

enum ADDRINFOFLAGS
{
    module=1,
    label=2,
    comment=4
};

enum BPXTYPE
{
    bpnone,
    bpnormal,
    bphardware,
    bpmemory
};

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

struct ADDRINFO
{
    char module[16]; //module the address is in
    char label[MAX_LABEL_SIZE];
    char comment[MAX_COMMENT_SIZE];
    int flags; //ADDRINFOFLAGS
};

//Debugger functions
void DLL_IMPEXP DbgMemRead(duint va, unsigned char* dest, duint size);
duint DLL_IMPEXP DbgMemGetPageSize(duint base);
duint DLL_IMPEXP DbgMemFindBaseAddr(duint addr, duint* size);
bool DLL_IMPEXP DbgCmdExec(const char* cmd);
bool DLL_IMPEXP DbgMemMap(MEMMAP* memmap);
bool DLL_IMPEXP DbgIsValidExpression(const char* expression);
bool DLL_IMPEXP DbgIsDebugging();
bool DLL_IMPEXP DbgIsJumpGoingToExecute(duint addr);
bool DLL_IMPEXP DbgGetLabelAt(duint addr, SEGMENTREG segment, char* text);
bool DLL_IMPEXP DbgSetLabelAt(duint addr, const char* text);
bool DLL_IMPEXP DbgGetCommentAt(duint addr, char* text);
bool DLL_IMPEXP DbgSetCommentAt(duint addr, const char* text);
BPXTYPE DLL_IMPEXP DbgGetBpxTypeAt(duint addr);

//GUI functions
void DLL_IMPEXP GuiDisasmAt(duint addr, duint cip);
void DLL_IMPEXP GuiSetDebugState(DBGSTATE state);
void DLL_IMPEXP GuiAddLogMessage(const char* msg);
void DLL_IMPEXP GuiLogClear();

#ifdef __cplusplus
}
#endif

#endif // _BRIDGEMAIN_H_
