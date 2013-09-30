#include "MapViewOfMem.h"


MapViewOfMem::MapViewOfMem()
{
    mSelectedData.fromIndex = -1;
    mSelectedData.toIndex = -1;
}

MapViewOfMem::MapViewOfMem(QString file)
{
    mGuiMemDataCache = (MemDataCacheStruct_t){new QVector<byte_t>(0), 0, 0, false};

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

byte_t MapViewOfMem::readByte(uint64 rva)
{
    return mData.data()[(uint32)rva];
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


byte_t* MapViewOfMem::getDataPtrForGui(uint64 rva, uint32 maxNbrOfBytesToRead, uint32 newCacheSize)
{
    byte_t* wBytePtr = 0;

    if(maxNbrOfBytesToRead > 0)
    {
        // Bound maxNbrOfBytesToRead to the max value it can take
        if(maxNbrOfBytesToRead > (this->size() - rva))
            maxNbrOfBytesToRead = this->size() - rva;

        if((mGuiMemDataCache.isInit == true) && (rva >= mGuiMemDataCache.rva) && ((rva + (uint64)maxNbrOfBytesToRead) <= (mGuiMemDataCache.rva + (uint64)mGuiMemDataCache.memDataCacheSize)))
        {
            // Cache Success
            wBytePtr = mGuiMemDataCache.memDataCachePtr->data() + (rva - mGuiMemDataCache.rva);
        }
        else
        {
            // Cache Miss
            mGuiMemDataCache.memDataCacheSize = newCacheSize;
            mGuiMemDataCache.memDataCachePtr->resize(newCacheSize);
            mGuiMemDataCache.rva = rva;
            wBytePtr = mGuiMemDataCache.memDataCachePtr->data();
            // TODO: Fill cache
            for(int wI = 0; wI < newCacheSize; wI++)
            {
                wBytePtr[wI] = readByte(rva + (uint64)wI);
            }
            mGuiMemDataCache.isInit = true;
        }
    }

    return wBytePtr;
}














