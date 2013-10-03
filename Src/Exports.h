#ifndef EXPORTS_H
#define EXPORTS_H



/************************************************************************************
                            Exported Function Prototypes
************************************************************************************/

#ifdef BUILD_LIB
    extern "C" __declspec(dllexport) int launchGUI(int argc, char *argv[]);
    extern "C" __declspec(dllexport) int GuiChangeEIP(unsigned long long eip);
#endif


#endif // EXPORTS_H
