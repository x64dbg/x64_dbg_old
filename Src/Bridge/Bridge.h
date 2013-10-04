#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include "NewTypes.h"
#include "Imports.h"


class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = 0);
    void readProcessMemory(byte_t* dest, uint64 rva, uint64 size);
    uint64 getSize();
    
signals:
    
public slots:

private:
    QByteArray* mData;

};

#endif // BRIDGE_H
