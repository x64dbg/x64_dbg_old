#ifndef QBEAENGINE_H
#define QBEAENGINE_H

#include <QtGui>
#include <QDebug>

#include "BeaEngine.h"



typedef struct _Instruction_t
{
    QString instStr;
    QByteArray dump;
    ulong rva;
    int lentgh;
    DISASM disasm;
} Instruction_t;



class QBeaEngine : public QWidget
{
    Q_OBJECT
public:
    explicit QBeaEngine(QWidget *parent = 0);

    ulong DisassembleBack(char *data, ulong base, ulong size, ulong ip, int n);
    ulong DisassembleNext(char* data, ulong base, ulong size, ulong ip, int n);
    
    Instruction_t DisassembleAt(char* data, ulong base, ulong size, ulong ip);
signals:
    
public slots:


private:
    DISASM mDisasmStruct;
    
};

#endif // QBEAENGINE_H
