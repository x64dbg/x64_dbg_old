#ifndef PROCESSMEMORYCACHE_H
#define PROCESSMEMORYCACHE_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include "NewTypes.h"
#include "windows.h"

class ProcessMemoryCache : public QObject
{
    Q_OBJECT
public:
    explicit ProcessMemoryCache(QObject *parent = 0);

    void setMemoryToCache(uint64 parBase, uint64 parSize);
    byte_t* readFromCache(uint64 parRVA, uint64 parLength, uint64 parCacheNewSize);
    
signals:
    
public slots:

private:

    typedef struct _MemDataCacheStruct_t
    {
        QByteArray* memDataCachePtr;
        uint64 memDataCacheSize;
        uint64 rva;
        bool isInit;
    } MemDataCacheStruct_t;


    MemDataCacheStruct_t mMemDataCache;

    uint64 mBase;
    uint64 mSize;
    
};

#endif // PROCESSMEMORYCACHE_H
