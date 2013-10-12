#include "_global.h"

//hInst GUI/DBG
HINSTANCE hInstGui;
HINSTANCE hInstDbg;

//GUI functions
GUIGUIINIT _gui_guiinit;
GUIDISASSEMBLEAT _gui_disassembleat;

//DBG functions
DBGDBGINIT _dbg_dbginit;
DBGMEMFINDBASEADDR _dbg_memfindbaseaddr;
DBGMEMREAD _dbg_memread;
DBGDBGCMDEXEC _dbg_dbgcmdexec;
DBGMEMMAP _dbg_memmap;
