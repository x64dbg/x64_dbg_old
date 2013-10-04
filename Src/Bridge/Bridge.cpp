#include "Bridge.h"

Bridge::Bridge(QObject *parent) : QObject(parent)
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



void Bridge::readProcessMemory(byte_t* dest, uint64 rva, uint64 size)
{
    DbgMemRead(dest, 0x00401000 + rva, size);
}


uint64 Bridge::getSize()
{

    return DbgMemGetPageSize(0x00401000);

    //return mData->size();
}
