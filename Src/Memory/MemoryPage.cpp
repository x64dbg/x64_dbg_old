#include "MemoryPage.h"

MemoryPage::MemoryPage(uint64 parBase, uint64 parSize, QObject *parent) : QObject(parent)
{
    mBase = 0;
    mSize = 0;
    mMemCache = new MemoryCache();
    mMemCache->setMemoryToCache(0, 0);
}

byte_t* MemoryPage::readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize)
{
    return mMemCache->readFromCache(parRVA, parLength, parCacheNewSize);
}


uint64 MemoryPage::getSize()
{
    return mSize;
}


uint64 MemoryPage::getBase()
{
    return mBase;
}


void MemoryPage::setAttributes(uint64 base, uint64 size)
{
    mBase = base;
    mSize = size;
    mMemCache->setMemoryToCache(base, size);
}


void MemoryPage::resetCache()
{
    mMemCache->resetCache();
}
