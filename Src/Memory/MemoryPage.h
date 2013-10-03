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
    explicit MemoryPage(MEMORY_BASIC_INFORMATION parMemInfo, QObject *parent = 0);

    byte_t* readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize);
    
signals:
    
public slots:

private:
    MEMORY_BASIC_INFORMATION mMBI;
    MemoryCache* mMemCache;

    
};

#endif // MEMORYPAGE_H
