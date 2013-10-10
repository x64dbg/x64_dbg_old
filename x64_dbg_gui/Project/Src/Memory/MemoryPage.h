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
    explicit MemoryPage(uint_t parBase, uint_t parSize, QObject *parent = 0);

    byte_t* readFromCache(uint_t parRVA, uint_t parLength, uint_t parCacheNewSize);
    uint_t getSize();
    uint_t getBase();
    void setAttributes(uint_t base, uint_t size);
    void resetCache();
    
signals:
    
public slots:

private:
    MemoryCache* mMemCache;
    uint_t mBase;
    uint_t mSize;

    
};

#endif // MEMORYPAGE_H
