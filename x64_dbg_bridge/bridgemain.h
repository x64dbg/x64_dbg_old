#ifndef _BRIDGEMAIN_H_
#define _BRIDGEMAIN_H_

#include "_global.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Bridge
const char* DLL_EXPORT BridgeInit();
const char* DLL_EXPORT BridgeStart();

//Debugger
void DLL_EXPORT DbgMemRead(unsigned char* dest, unsigned long long va, unsigned long long size);
unsigned long long DLL_EXPORT DbgMemGetPageSize(unsigned long long base);

//GUI
void DLL_EXPORT GuiChangeCIP(unsigned long long cip);

#ifdef __cplusplus
}
#endif

#endif // _BRIDGEMAIN_H_
