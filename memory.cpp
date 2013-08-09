#include "memory.h"

uint memfindbaseaddr(HANDLE hProcess, uint addr, uint* size)
{
#ifndef _WIN64
    MEMORY_BASIC_INFORMATION mbi;
#else
    MEMORY_BASIC_INFORMATION64 mbi;
#endif // _WIN64
    DWORD numBytes;
    uint MyAddress=0, newAddress=0;
    do
    {
        numBytes=VirtualQueryEx(hProcess, (LPCVOID)MyAddress, (MEMORY_BASIC_INFORMATION*)&mbi, sizeof(mbi));
        newAddress=(uint)mbi.BaseAddress+mbi.RegionSize;
        if(addr<newAddress and addr>=MyAddress)
        {
            if(size)
                *size=mbi.RegionSize;
            return (uint)mbi.BaseAddress;
        }
        if(newAddress<=MyAddress)
            numBytes=0;
        else
            MyAddress=newAddress;
    }
    while(numBytes);
    return 0;
}
