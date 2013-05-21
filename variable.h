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

//functions
void varinit();
VAR* vargetptr();
bool varnew(const char* name, void* value, VAR_TYPE type);
bool varget(const char* name, void* value, VAR_TYPE* type);
bool varset(const char* name, void* value, bool setreadonly);
bool vardel(const char* name_, bool delsystem);
bool getvaluefromstring(const char* string, void* value, int* value_size, VAR_TYPE* var_type, bool* isvar);

#endif // _VARIABLE_H
