#include "QBeaEngine.h"


QBeaEngine::QBeaEngine(QWidget *parent) : QWidget(parent)
{
    QFile file("AsmCode.bin");

    if(file.open(QIODevice::ReadOnly) == false)
         qDebug() << "File has not been opened.";

    QByteArray data = file.readAll();
    qDebug() << "Size: " << data.size();

    if(data.size() == 0)
    {
        qDebug() << "No Data";
    }

    DISASM MyDisasm;
    int len, i = 0;
    int Error = 0;

    // Init the Disasm structure
    (void) memset (&MyDisasm, 0, sizeof(DISASM));

    // Init EIP
    MyDisasm.EIP = (UIntPtr)data.data();

    // Loop for Disasm
    while ((!Error) && (i<data.size()))
    {
        len = Disasm(&MyDisasm);
        if (len != UNKNOWN_OPCODE)
        {
            //(void) puts(MyDisasm.CompleteInstr);
            qDebug() << "i= " << (int)MyDisasm.EIP-(int)data.data() << QString(MyDisasm.CompleteInstr);
            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
            i += (int)len;
        }
        else {
            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)1;
            i += (int)len;
            /*
            Error = 1;
            qDebug() << "error ---------------------------------------------";
            */
        }
    }
    qDebug() << "End --------------------------------------------- i " << i;
}



void toto()
{
    int i = 0;

    while(i < 10)
    {
        i = i*2+4;
    }
}
