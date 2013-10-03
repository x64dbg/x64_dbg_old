#include "MemoryPage.h"

MemoryPage::MemoryPage(MEMORY_BASIC_INFORMATION parMemInfo, QObject *parent) : QObject(parent)
{
    mMBI = parMemInfo;
    mMemCache = new MemoryCache();
    //mMemCache->setMemoryToCache(reinterpret_cast<uint64>(parMemInfo.BaseAddress), parMemInfo.RegionSize);

    mMemCache->setMemoryToCache(0x00401000, getBridge()->getSize());
}

byte_t* MemoryPage::readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize)
{
    return mMemCache->readFromCache(parRVA, parLength, parCacheNewSize);
}
