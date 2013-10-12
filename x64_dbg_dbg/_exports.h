#ifndef _EXPORTS_H
#define _EXPORTS_H

#include "_global.h"
#include "..\x64_dbg_bridge\bridgemain.h"

#ifdef __cplusplus
extern "C"
{
#endif

duint DLL_EXPORT _dbg_memfindbaseaddr(duint addr, duint* size);
bool DLL_EXPORT _dbg_memread(duint addr, unsigned char* dest, duint size, duint* read);
extern "C" DLL_EXPORT bool _dbg_memmap(MEMMAP* memmap);

#ifdef __cplusplus
}
#endif

#endif // _EXPORTS_H
