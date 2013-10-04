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

void DLL_IMPORT SaveWindowPos(const char* window_name, HWND hwndDlg);
void DLL_IMPORT LoadWindowPos(const char* window_name, HWND hwndDlg);
HWND DLL_IMPORT GetConsoleHwnd(void);
void DLL_IMPORT DisasmInit(DISASM_INIT* initstruct);
uint* DLL_IMPORT DisasmDo(char* mem, uint mem_va, uint codebase, uint code_size, uint ip, uint real_eip);

#ifdef __cplusplus
}
#endif

#endif // __DISASM_H__
