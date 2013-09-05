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
        if(mbi.State==MEM_COMMIT and addr<newAddress and addr>=MyAddress)
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

bool memread(HANDLE hProcess, const void* lpBaseAddress, void* lpBuffer, DWORD nSize, SIZE_T* lpNumberOfBytesRead)
{
    uint size;
    uint base=memfindbaseaddr(hProcess, (uint)lpBaseAddress, &size);
    DWORD oldprotect;
    VirtualProtectEx(hProcess, (void*)base, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    bool ret=ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
    VirtualProtectEx(hProcess, (void*)base, size, oldprotect, &oldprotect);
    return ret;
}

void* memalloc(HANDLE hProcess, uint addr, DWORD size, DWORD fdProtect)
{
    return VirtualAllocEx(hProcess, (void*)addr, size, MEM_RESERVE|MEM_COMMIT, fdProtect);
}
