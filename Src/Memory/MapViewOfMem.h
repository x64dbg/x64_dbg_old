#ifndef MAPVIEWOFMEM_H
#define MAPVIEWOFMEM_H

#include <QWidget>


class MapViewOfMem
{

    public:
        MapViewOfMem();
        explicit MapViewOfMem(unsigned long long startAddress , unsigned long long size);
        ~MapViewOfMem();
        QString readByte(unsigned long long address);
        unsigned long long size();

    private:
        unsigned long long mStartAddress;
        unsigned long long mEndAddress;
        unsigned long long mSize;
};

#endif // MAPVIEWOFMEM_H

