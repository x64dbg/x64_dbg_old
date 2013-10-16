#include "_exports.h"
#include <vector>
#include "memory.h"
#include "debugger.h"
#include <psapi.h>
#include "dbghelp\dbghelp.h"


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

extern "C" DLL_EXPORT bool _dbg_memmap(MEMMAP* memmap)
{
    memset(memmap, 0, sizeof(MEMMAP));

    MEMORY_BASIC_INFORMATION mbi;
    DWORD numBytes;
    uint MyAddress=0, newAddress=0, curmod=0, curmodsize=0;
    char curmodname[20]="";
    SymInitialize(fdProcessInfo->hProcess, 0, true);
    std::vector<MEMPAGE> pageVector;
    do
    {
        numBytes=VirtualQueryEx(fdProcessInfo->hProcess, (LPCVOID)MyAddress, &mbi, sizeof(mbi));
        if(mbi.State==MEM_COMMIT)
        {
            IMAGEHLP_MODULE64 nfo;
            nfo.SizeOfStruct=sizeof(IMAGEHLP_MODULE64);
            if(SymGetModuleInfo64(fdProcessInfo->hProcess, MyAddress, &nfo))
            {
                curmod=MyAddress;
                curmodsize=nfo.ImageSize;
                char szBaseName[256]="";
                int len=GetModuleBaseName(fdProcessInfo->hProcess, (HMODULE)MyAddress, szBaseName, 256);
                if(len)
                {
                    len--;
                    while(szBaseName[len]!='.')
                        len--;
                    if(len)
                        szBaseName[len]=0;
                    szBaseName[16]=0;
                    _strlwr(szBaseName);
                    memset(curmodname, ' ', 16);
                    memcpy(curmodname, szBaseName, strlen(szBaseName));
                }
            }
            char mod[20]="                ";
            if(mbi.Type==MEM_IMAGE) //image
            {
                if(MyAddress>=curmod and MyAddress<(curmod+curmodsize))
                    memcpy(mod, curmodname, 16);
            }
            MEMPAGE curPage;
            memcpy(&curPage.mbi, &mbi, sizeof(mbi));
            memcpy(&curPage.mod, mod, 16);
            pageVector.push_back(curPage);
            memmap->count++;
        }
        newAddress=(uint)mbi.BaseAddress+mbi.RegionSize;
        if(newAddress<=MyAddress)
            numBytes=0;
        else
            MyAddress=newAddress;
    }
    while(numBytes);

    //process vector
    int pagecount=memmap->count;
    memmap->page=(MEMPAGE*)BridgeAlloc(sizeof(MEMPAGE)*pagecount);
    memset(memmap->page, 0, sizeof(MEMPAGE)*pagecount);
    for(int i=0; i<pagecount; i++)
        memcpy(&memmap->page[i], &pageVector.at(i), sizeof(MEMPAGE));

    return true;
}

extern "C" DLL_EXPORT void _dbg_dbgexitsignal()
{
    //TODO: handle exit signal
    DeleteFileA("DLLLoader.exe");
    StopDebug();
    ForceClose();
}
