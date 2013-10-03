#ifndef IMPORTS_H
#define IMPORTS_H

#ifdef BUILD_LIB
    // TODO
#else
    #include "NewTypes.h"

    void iInitDbg();
    void iReadProcessMemory(byte_t* dest, uint64 rva, uint64 size);
    unsigned long long iGetMemSize();
#endif


#endif // IMPORTS_H
