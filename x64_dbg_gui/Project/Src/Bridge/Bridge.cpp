#include "Bridge.h"

/************************************************************************************
                            Global Variables
************************************************************************************/
Bridge* mBridge;



/************************************************************************************
                            Class Members
************************************************************************************/
Bridge::Bridge(QObject *parent) : QObject(parent)
{
    mData = new QByteArray();

    QFile wFile("AsmCode.bin");

    if(wFile.open(QIODevice::ReadOnly) == false)
         qDebug() << "File has not been opened.";

    *mData = wFile.readAll();
    qDebug() << "Size: " << mData->size();

    if(mData->size() == 0)
    {
        qDebug() << "No Data";
    }
}



void Bridge::readProcessMemory(byte_t* dest, uint_t va, uint_t size)
{
#ifdef BUILD_LIB
    DbgMemRead(va, dest, size);
#else
    stubReadProcessMemory(dest, va, size);
#endif
}

void Bridge::emitDisassembleAtSignal(uint_t va, uint_t eip)
{
#ifdef BUILD_LIB
    emit disassembleAt(va, eip);
#endif
}

uint_t Bridge::getSize(uint_t va)
{
#ifdef BUILD_LIB
    return DbgMemGetPageSize(va);
#else
    return mData->size();
#endif
}

uint_t Bridge::getBase(uint_t addr)
{
#ifdef BUILD_LIB
    return DbgMemFindBaseAddr(addr,0);
#else
    return 0x00401000;
#endif
}


bool Bridge::execCmd(const char* cmd)
{
    return DbgCmdExec(cmd);
}


/************************************************************************************
                            Static Functions
************************************************************************************/
Bridge* Bridge::getBridge()
{
    return mBridge;
}


void Bridge::initBridge()
{
    mBridge = new Bridge();
}



/************************************************************************************
                            Exported Functions
************************************************************************************/

#ifdef BUILD_LIB

    __declspec(dllexport) int _gui_guiinit(int argc, char *argv[])
    {
        return main(argc, argv);
    }



    __declspec(dllexport) int _gui_disassembleAt(unsigned long long va, unsigned long long eip)
    {
        Bridge::getBridge()->emitDisassembleAtSignal(va, eip);
    }
#endif


/************************************************************************************
                            Imported Functions (Stub)
************************************************************************************/
#ifndef BUILD_LIB
    void stubReadProcessMemory(byte_t* dest, uint64 va, uint64 size)
    {
        uint64 wI;

        for(wI = 0; wI < size; wI++)
        {
            dest[wI] = Bridge::getBridge()->mData->data()[(va - Bridge::getBridge()->getBase(0)) + wI];
        }
    }
#endif






























