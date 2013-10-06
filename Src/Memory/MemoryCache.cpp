#include "MemoryCache.h"

MemoryCache::MemoryCache(QObject *parent) : QObject(parent)
{
    mMemDataCache = (MemDataCacheStruct_t){new QByteArray(), 0, 0, false};
}

void MemoryCache::setMemoryToCache(uint64 parBase, uint64 parSize)
{
    mBase = parBase;
    mSize = parSize;
}

void MemoryCache::resetCache()
{
    mMemDataCache.isInit = false;
}


byte_t* MemoryCache::readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize)
{
    byte_t* wBytePtr = 0;

    if((parLength > 0) && (mSize > parRVA))
    {
        // Bound parSize and parCacheNewSize to the max value they can take
        if(parLength > (mSize - parRVA))
            parLength = mSize - parRVA;

        if(parCacheNewSize > (mSize - parRVA))
            parCacheNewSize = mSize - parRVA;

        if((mMemDataCache.isInit == true) && (parRVA >= mMemDataCache.rva) && ((parRVA + parLength) <= (mMemDataCache.rva + (uint64)mMemDataCache.memDataCacheSize)))
        {
            // Cache Success
            //qDebug() << "Cache Success";
            wBytePtr = reinterpret_cast<byte_t*>(mMemDataCache.memDataCachePtr->data()) + (parRVA - mMemDataCache.rva);
        }
        else
        {
            // Cache Miss
            //qDebug() << "Cache has been updated. Old Size:" << mMemDataCache.memDataCacheSize << " Old RVA:" << mMemDataCache.rva << " New Size:" << parCacheNewSize << " New RVA:" << parRVA;
            mMemDataCache.memDataCacheSize = parCacheNewSize;
            mMemDataCache.memDataCachePtr->resize(parCacheNewSize);
            mMemDataCache.rva = parRVA;
            wBytePtr = reinterpret_cast<byte_t*>(mMemDataCache.memDataCachePtr->data());
            // TODO: Fill cache
            Bridge::getBridge()->readProcessMemory(wBytePtr, mBase + parRVA, parCacheNewSize);

            mMemDataCache.isInit = true;
        }
    }

    return wBytePtr;
}
