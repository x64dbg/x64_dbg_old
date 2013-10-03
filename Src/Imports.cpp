#include "Imports.h"

#ifndef BUILD_LIB

#include <QtGui>

QByteArray* mData;

void iInitDbg()
{
    mData = new QByteArray();

    QFile wFile("AsmCode.bin");

    if(wFile.open(QIODevice::ReadOnly) == false)
         qDebug() << "File has not been opened.";

    *mData = wFile.readAll();
    qDebug() << "Size: " << mData->size();

    if(mData->size() == 0)
    {
        qDebug() << "No Data";
    }
}


void iReadProcessMemory(byte_t* dest, uint64 rva, uint64 size)
{
    uint64 wI;

    for(wI = 0; wI < size; wI++)
    {
        dest[wI] = mData->data()[rva + wI];
    }
}

unsigned long long iGetMemSize()
{
    return mData->size();
}


#endif
