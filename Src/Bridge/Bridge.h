#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include "NewTypes.h"

#ifdef BUILD_LIB
    #include "Exports.h"
    #include "main.h"
#endif

#include "Imports.h"


class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = 0);
    void readProcessMemory(byte_t* dest, uint64 rva, uint64 size);
    uint64 getSize();
    void emitEIPChangedSignal(uint64 eip);
    static Bridge* getBridge();
    static Bridge* initBridge();
    
signals:
    void eipChanged(uint64 eip);
    
public slots:

private:

public:
    QByteArray* mData;
};

#endif // BRIDGE_H
