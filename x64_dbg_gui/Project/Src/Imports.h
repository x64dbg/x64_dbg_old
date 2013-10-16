#ifndef IMPORTS_H
#define IMPORTS_H

#ifdef BUILD_LIB
    #include "..\..\..\x64_dbg_bridge\bridgemain.h"
#else
    #include "NewTypes.h"
    void stubReadProcessMemory(byte_t* dest, uint64 va, uint64 size);
#endif


#endif // IMPORTS_H
