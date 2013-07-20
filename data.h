#ifndef _DATA_H
#define _DATA_H

#include "_global.h"

struct DATA;

//typedefs
typedef void (*CBTYPE)(DATA*);

//structs
struct TYPE
{
    unsigned int* length;
    unsigned int* data_type;
    CBTYPE cbType;
};

struct DISASM_OPTIONS
{
    uint mem_va;
    unsigned int codebase;
    unsigned int code_size;
    unsigned int ip;
    uint reserved[16];
};

struct STACK_OPTIONS
{
    uint mem_va;
    unsigned int sp;
};

struct DATA
{
    unsigned int flags;
    unsigned char* mem;
    unsigned int mem_size;
    TYPE type;
    union
    {
        DISASM_OPTIONS disasm;
        STACK_OPTIONS stack;
    } options;
};

#endif // _DATA_H
