#ifndef _EXPORTS_H
#define _EXPORTS_H

#include "_global.h"
#include "..\x64_dbg_bridge\bridgemain.h"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT duint _dbg_memfindbaseaddr(duint addr, duint* size);
DLL_EXPORT bool _dbg_memread(duint addr, unsigned char* dest, duint size, duint* read);
DLL_EXPORT bool _dbg_memmap(MEMMAP* memmap);
DLL_EXPORT void _dbg_dbgexitsignal();

#ifdef __cplusplus
}
#endif

#endif // _EXPORTS_H
