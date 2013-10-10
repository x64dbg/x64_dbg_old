#ifndef MAPVIEWOFMEM_H
#define MAPVIEWOFMEM_H

#include <QtGui>
#include <QDebug>
#include "NewTypes.h"


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
        byte_t readByte(uint_t rva);
        unsigned long long size();
        unsigned char *data();

        Selection_t getSelection();
        void setSelection(Selection_t sel);

        ulong getBase();
        byte_t* getDataPtrForGui(uint_t rva, uint32 maxNbrOfBytesToRead, uint32 newCacheSize);

    private:
        typedef struct _MemDataCacheStruct_t
        {
            QVector<byte_t>* memDataCachePtr;
            uint32 memDataCacheSize;
            uint_t rva;
            bool isInit;
        } MemDataCacheStruct_t;


        ulong mBase;
        unsigned long mStartAddress;
        unsigned long mEndAddress;
        unsigned long mSize;

        QByteArray mData;

        Selection_t mSelectedData;

        MemDataCacheStruct_t mGuiMemDataCache;
};

#endif // MAPVIEWOFMEM_H

