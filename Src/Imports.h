#ifndef IMPORTS_H
#define IMPORTS_H

#ifdef BUILD_LIB
    extern "C" bool __declspec(dllimport) BridgeInit();
    extern "C" void __declspec(dllimport) DbgMemRead(unsigned char* dest, unsigned long long va, unsigned long long size);
    extern "C" unsigned long long __declspec(dllimport) DbgMemGetPageSize(unsigned long long base);
    extern "C" unsigned long long __declspec(dllimport) DbgMemFindBaseAddr(unsigned long long addr, unsigned long long* size);
#else
    #include "NewTypes.h"

    void stubReadProcessMemory(byte_t* dest, uint64 va, uint64 size);
#endif


#endif // IMPORTS_H
