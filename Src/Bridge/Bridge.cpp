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



void Bridge::readProcessMemory(byte_t* dest, uint64 rva, uint64 size)
{
#ifdef BUILD_LIB
    DbgMemRead(dest, rva, size);
#else
    stubReadProcessMemory(dest, rva, size);
#endif
}

void Bridge::emitEIPChangedSignal(uint64 eip)
{
#ifdef BUILD_LIB
    emit eipChanged(eip);
#endif
}

uint64 Bridge::getSize(uint64 va)
{
#ifdef BUILD_LIB
    return DbgMemGetPageSize(va);
#else
    return mData->size();
#endif
}

uint64 Bridge::getBase(uint64 addr)
{
#ifdef BUILD_LIB
    return DbgMemFindBaseAddr(addr,0);
#else
    return 0x00401000;
#endif
}


/************************************************************************************
                            Static Functions
************************************************************************************/
Bridge* Bridge::getBridge()
{
    return mBridge;
}


Bridge* Bridge::initBridge()
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



    __declspec(dllexport) int _gui_changecip(unsigned long long eip)
    {
        Bridge::getBridge()->emitEIPChangedSignal(eip);
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






























