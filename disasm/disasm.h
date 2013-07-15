#ifndef __DISASM_H__
#define __DISASM_H__

#include <windows.h>

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

void DLL_EXPORT DisasmInit(DISASM_INIT* initstruct);
unsigned int* DLL_EXPORT DisasmDo(char* mem, unsigned int mem_va, unsigned int codebase, unsigned int code_size, int ip);

#ifdef __cplusplus
}
#endif

#endif // __DISASM_H__
