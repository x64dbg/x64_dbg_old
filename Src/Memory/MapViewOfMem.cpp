#include "MapViewOfMem.h"


MapViewOfMem::MapViewOfMem()
{

}


MapViewOfMem::MapViewOfMem(unsigned long long startAddress , unsigned long long size)
{
    mStartAddress = startAddress;
    mEndAddress = startAddress + size - 1;
    mSize = size;
}

MapViewOfMem::~MapViewOfMem()
{

}

QString MapViewOfMem::readByte(unsigned long long address)
{
    return QString::number(address);
}

unsigned long long MapViewOfMem::size()
{
    return mSize;
}

