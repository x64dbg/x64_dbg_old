#ifndef _MEMORY_H
#define _MEMORY_H

#include "_global.h"

uint memfindbaseaddr(HANDLE hProcess, uint addr, uint* size);

#endif // _MEMORY_H
