#ifndef __DISASM_H__
#define __DISASM_H__

#include <_global.h>

typedef void (*CBCONTEXTMENU)(int,int);

struct DISASM_INIT
{
    CBCONTEXTMENU cbContextMenu;
    char** menu_items;
    int total_items;
};


#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT HWND GetConsoleHwnd(void);
void DLL_EXPORT DisasmInit(DISASM_INIT* initstruct);
uint* DLL_EXPORT DisasmDo(char* mem, uint mem_va, uint codebase, uint code_size, uint ip, uint real_eip);

#ifdef __cplusplus
}
#endif

#endif // __DISASM_H__
