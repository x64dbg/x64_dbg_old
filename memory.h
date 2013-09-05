#ifndef _MEMORY_H
#define _MEMORY_H

#include "_global.h"

uint memfindbaseaddr(HANDLE hProcess, uint addr, uint* size);
bool memread(HANDLE hProcess, const void* lpBaseAddress, void* lpBuffer, DWORD nSize, SIZE_T* lpNumberOfBytesRead);
void* memalloc(HANDLE hProcess, uint addr, DWORD size, DWORD fdProtect);

#endif // _MEMORY_H
