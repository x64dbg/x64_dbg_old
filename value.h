#ifndef _VALUE_H
#define _VALUE_H

#include "_global.h"

//functions
bool valuesignedcalc();
void valuesetsignedcalc(bool a);
bool isflag(const char* string);
bool isregister(const char* string);
bool valfromstring(const char* string, uint* value, int* value_size, bool* isvar, bool silent);

#endif // _VALUE_H
