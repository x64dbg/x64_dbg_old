#include <windows.h>

char global[10]="0";

int main()
{
    GetTickCount();
    GetCurrentProcessId();
    GetCurrentProcess();
    DWORD old=0;
    VirtualProtect(global, 10, PAGE_GUARD, &old);
    asm("int3");
    return 0;
}
