#ifndef _VARIABLE_H
#define _VARIABLE_H

#include "_global.h"
#include "command.h"

//enums
enum VAR_TYPE
{
    VAR_SYSTEM=1,
    VAR_USER=2,
    VAR_READONLY=3
};

//structures
struct VAR
{
    char* name;
    VAR_TYPE type;
    void* value;
    VAR* next;
};

//variables
extern VAR* vars;

//functions
void varinit();
bool varnew(const char* name, void* value, VAR_TYPE type);
bool varget(const char* name, void* value, VAR_TYPE* type);
bool varset(const char* name, void* value);
bool vardel(const char* name);
bool getvaluefromstring(const char* string, void* value_);

#endif // _VARIABLE_H
