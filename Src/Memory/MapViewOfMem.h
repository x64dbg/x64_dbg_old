#ifndef MAPVIEWOFMEM_H
#define MAPVIEWOFMEM_H

#include <QtGui>
#include <QDebug>


typedef struct _Selection_t
{
    int firstSelectedIndex;
    int fromIndex;
    int toIndex;
} Selection_t;



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

        Selection_t getSelection();
        void setSelection(Selection_t sel);

        ulong getBase();

    private:
        ulong mBase;
        unsigned long mStartAddress;
        unsigned long mEndAddress;
        unsigned long mSize;

        QByteArray mData;

        Selection_t mSelectedData;
};

#endif // MAPVIEWOFMEM_H

