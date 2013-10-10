#ifndef PROCESSMEMORYCACHE_H
#define PROCESSMEMORYCACHE_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include "NewTypes.h"
#include "Bridge.h"
#include "windows.h"
#include "main.h"

class MemoryCache : public QObject
{
    Q_OBJECT
public:
    explicit MemoryCache(QObject *parent = 0);

    void setMemoryToCache(uint_t parBase, uint_t parSize);
    void resetCache();
    byte_t* readFromCache(uint_t parRVA, uint_t parLength, uint_t parCacheNewSize);
    
signals:
    
public slots:

private:

    typedef struct _MemDataCacheStruct_t
    {
        QByteArray* memDataCachePtr;
        uint_t memDataCacheSize;
        uint_t rva;
        bool isInit;
    } MemDataCacheStruct_t;


    MemDataCacheStruct_t mMemDataCache;

    uint_t mBase;
    uint_t mSize;
};

#endif // PROCESSMEMORYCACHE_H
