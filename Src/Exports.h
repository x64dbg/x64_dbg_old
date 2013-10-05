#ifndef EXPORTS_H
#define EXPORTS_H

/************************************************************************************
                            Exported Function Prototypes
************************************************************************************/

#ifdef BUILD_LIB
    extern "C" __declspec(dllexport) int _gui_guiinit(int argc, char *argv[]);
    extern "C" __declspec(dllexport) int _gui_guichangeeip(unsigned long long eip);
#endif


#endif // EXPORTS_H
