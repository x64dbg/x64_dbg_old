#include "Bridge.h"

Bridge::Bridge(QObject *parent) : QObject(parent)
{

}



void Bridge::readProcessMemory(byte_t* dest, uint64 rva, uint64 size)
{
    iReadProcessMemory(dest, rva, size);
}


uint64 Bridge::getSize()
{
    return iGetMemSize();
}
