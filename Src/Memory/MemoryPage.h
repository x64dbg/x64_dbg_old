#ifndef MEMORYPAGE_H
#define MEMORYPAGE_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include "NewTypes.h"
#include "windows.h"
#include "MemoryCache.h"
#include "main.h"

class MemoryPage : public QObject
{
    Q_OBJECT
public:
    explicit MemoryPage(uint64 parBase, uint64 parSize, QObject *parent = 0);

    byte_t* readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize);
    uint64 getSize();
    uint64 getBase();
    void setAttributes(uint64 base, uint64 size);
    void resetCache();
    
signals:
    
public slots:

private:
    MemoryCache* mMemCache;
    uint64 mBase;
    uint64 mSize;

    
};

#endif // MEMORYPAGE_H
