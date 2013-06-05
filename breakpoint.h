#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H

#include "_global.h"
#include "TitanEngine.h"

//enums
enum BP_TYPE
{
    BPNORMAL=UE_BREAKPOINT,
    BPSINGLESHOOT=UE_SINGLESHOOT,
    BPHARDWARE=UE_HARDWARE,
    BPMEMORY=UE_MEMORY
};

//structs
struct BREAKPOINT
{
    char* name;
    uint addr;
    BP_TYPE type;
    BREAKPOINT* next;
};

//functions
BREAKPOINT* bpinit();
BREAKPOINT* bpfind(BREAKPOINT* breakpoint_list, const char* name, uint addr, BREAKPOINT** link);
bool bpnew(BREAKPOINT* breakpoint_list, const char* name, uint addr, BP_TYPE type);
bool bpsetname(BREAKPOINT* breakpoint_list, uint addr, const char* name);
bool bpdel(BREAKPOINT* breakpoint_list, const char* name, uint addr);

#endif // _BREAKPOINT_H
