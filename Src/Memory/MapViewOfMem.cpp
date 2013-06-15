#include "MapViewOfMem.h"


MapViewOfMem::MapViewOfMem()
{
    mSelectedData.fromIndex = -1;
    mSelectedData.toIndex = -1;
}

MapViewOfMem::MapViewOfMem(QString file)
{

    mSelectedData.fromIndex = -1;
    mSelectedData.toIndex = -1;

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
    mBase = 0x00401000;
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


Selection_t MapViewOfMem::getSelection()
{
    return mSelectedData;
}

void MapViewOfMem::setSelection(Selection_t sel)
{
    mSelectedData = sel;
}

ulong MapViewOfMem::getBase()
{
    return mBase;
}
