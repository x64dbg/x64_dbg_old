#ifndef _VALUE_H
#define _VALUE_H

#include "_global.h"

//functions
bool isflag(const char* string);
bool isregister(const char* string);
bool valfromstring(const char* string, uint* value, int* value_size, bool* isvar);

#endif // _VALUE_H
