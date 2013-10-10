#include "MemoryPage.h"

MemoryPage::MemoryPage(uint_t parBase, uint_t parSize, QObject *parent) : QObject(parent)
{
    mBase = 0;
    mSize = 0;
    mMemCache = new MemoryCache();
    mMemCache->setMemoryToCache(0, 0);
}

byte_t* MemoryPage::readFromCache(uint_t parRVA, uint_t parLength, uint_t parCacheNewSize)
{
    return mMemCache->readFromCache(parRVA, parLength, parCacheNewSize);
}


uint_t MemoryPage::getSize()
{
    return mSize;
}


uint_t MemoryPage::getBase()
{
    return mBase;
}


void MemoryPage::setAttributes(uint_t base, uint_t size)
{
    mBase = base;
    mSize = size;
    mMemCache->setMemoryToCache(base, size);
}


void MemoryPage::resetCache()
{
    mMemCache->resetCache();
}
