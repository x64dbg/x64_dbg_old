#ifndef MAPVIEWOFMEM_H
#define MAPVIEWOFMEM_H

#include <QtGui>
#include <QDebug>

class MapViewOfMem
{

    public:
        MapViewOfMem();
        explicit MapViewOfMem(QString file);
        explicit MapViewOfMem(unsigned long long startAddress , unsigned long long size);
        ~MapViewOfMem();
        QString readByte(unsigned long long address);
        unsigned long long size();
        unsigned char *data();

    private:
        unsigned long mStartAddress;
        unsigned long mEndAddress;
        unsigned long mSize;

        QByteArray mData;
};

#endif // MAPVIEWOFMEM_H

