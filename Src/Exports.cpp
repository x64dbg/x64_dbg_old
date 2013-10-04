#include "Exports.h"


#ifdef BUILD_LIB

#include "main.h"


__declspec(dllexport) int _gui_guiinit(int argc, char *argv[])
{
    return main(argc, argv);
}

__declspec(dllexport) int _gui_guichangeeip(unsigned long long eip)
{

}


#endif


