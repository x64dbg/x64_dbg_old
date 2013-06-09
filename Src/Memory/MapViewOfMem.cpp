#include "MapViewOfMem.h"


MapViewOfMem::MapViewOfMem()
{

}

MapViewOfMem::MapViewOfMem(QString file)
{
    qDebug() << "MapViewOfMem() Load data from file.";

    QFile wFile(file);

    if(wFile.open(QIODevice::ReadOnly) == false)
         qDebug() << "File has not been opened.";

    mData = wFile.readAll();
    qDebug() << "Size: " << mData.size();

    if(mData.size() == 0)
    {
        qDebug() << "No Data";
    }

    mSize = mData.size();
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

unsigned char* MapViewOfMem::data()
{
    return (unsigned char*)(mData.data());
}
