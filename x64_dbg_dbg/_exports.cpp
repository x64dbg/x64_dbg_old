#include "_exports.h"
#include "memory.h"
#include "debugger.h"

extern "C" DLL_EXPORT duint _dbg_memfindbaseaddr(duint addr, duint* size)
{
    return memfindbaseaddr(fdProcessInfo->hProcess, addr, size);
}

extern "C" DLL_EXPORT bool _dbg_memread(duint addr, unsigned char* dest, duint size, duint* read)
{
    dbgdisablebpx();
    bool res=memread(fdProcessInfo->hProcess, (void*)addr, dest, size, read);
    dbgenablebpx();
    return res;
}

extern "C" DLL_EXPORT MEMPAGE* _dbg_memmap()
{
    static MEMPAGE mempage;
    memset(&mempage, 0, sizeof(MEMPAGE));
    return &mempage;
}
