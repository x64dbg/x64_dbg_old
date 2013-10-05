#include "_global.h"

//hInst GUI/DBG
HINSTANCE hInstGui;
HINSTANCE hInstDbg;

//GUI functions
GUIGUIINIT _gui_guiinit;
GUICHANGECIP _gui_changecip;

//DBG functions
DBGDBGINIT _dbg_dbginit;
DBGMEMFINDBASEADDR _dbg_memfindbaseaddr;
DBGMEMREAD _dbg_memread;
