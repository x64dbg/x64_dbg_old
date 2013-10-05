#ifndef _BRIDGEMAIN_H_
#define _BRIDGEMAIN_H_

#ifdef _WIN64
typedef unsigned long long duint;
typedef long long dsint;
#else
typedef unsigned long duint;
typedef long dsint;
#endif //_WIN64

#ifndef DLL_IMPEXP
#ifdef BUILD_BRIDGE
#define DLL_IMPEXP __declspec(dllexport)
#else
#define DLL_IMPEXP __declspec(dllimport)
#endif //BUILD_BRIDGE
#endif //DLL_IMPEXP

#ifdef __cplusplus
extern "C"
{
#endif

//Bridge
const char* DLL_IMPEXP BridgeInit();
const char* DLL_IMPEXP BridgeStart();

//Debugger
void DLL_IMPEXP DbgMemRead(duint va, unsigned char* dest, duint size);
duint DLL_IMPEXP DbgMemGetPageSize(duint base);
duint DLL_IMPEXP DbgMemFindBaseAddr(duint addr, duint* size);

//GUI
void DLL_IMPEXP GuiChangeCIP(duint cip);

#ifdef __cplusplus
}
#endif

#endif // _BRIDGEMAIN_H_
