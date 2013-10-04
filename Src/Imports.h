#ifndef IMPORTS_H
#define IMPORTS_H

#ifdef BUILD_LIB
    extern "C" bool __declspec(dllimport) BridgeInit();
    extern "C" void __declspec(dllimport) DbgMemRead(unsigned char* dest, unsigned long long va, unsigned long long size);
    extern "C" unsigned long long __declspec(dllimport) DbgMemGetPageSize(unsigned long long base);
#else
    #include "NewTypes.h"

    void iInitDbg();
    void iReadProcessMemory(byte_t* dest, uint64 rva, uint64 size);
    unsigned long long iGetMemSize();
#endif


#endif // IMPORTS_H
