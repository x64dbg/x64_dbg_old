#ifndef NEWTYPES_H
#define NEWTYPES_H

#include "Imports.h"

typedef int int32;
typedef unsigned int uint32;

typedef duint uint_t;
typedef dsint int_t;

typedef unsigned char byte_t;

#ifdef _WIN64 //defined by default
#define fhex "%.16llX"
#define fext "ll"
#else
#define fhex "%.8X"
#define fext ""
#endif // _WIN64


#endif // NEWTYPES_H
