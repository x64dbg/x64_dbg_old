#ifndef _VALUE_H
#define _VALUE_H

#include "_global.h"

//functions
bool valuesignedcalc();
void valuesetsignedcalc(bool a);
bool valapifromstring(const char* name, uint* value, int* value_size, bool printall, bool* hexonly);
bool valfromstring(const char* string, uint* value, int* value_size, bool* isvar, bool silent, bool* hexonly);

#endif // _VALUE_H
