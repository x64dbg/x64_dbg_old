#ifndef _MEMORY_H
#define _MEMORY_H

#include "_global.h"

uint memfindbaseaddr(HANDLE hProcess, uint addr, uint* size);
bool readmem(HANDLE hProcess, const void* lpBaseAddress, void* lpBuffer, DWORD nSize, SIZE_T* lpNumberOfBytesRead);

#endif // _MEMORY_H
