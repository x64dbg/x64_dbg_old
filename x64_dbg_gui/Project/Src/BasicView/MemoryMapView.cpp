#include "MemoryMapView.h"

MemoryMapView::MemoryMapView(StdTable *parent) : StdTable(parent)
{
    enableMultiSelection(false);

    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);


    //setRowCount(100);


    connect(Bridge::getBridge(), SIGNAL(dbgStateChanged(DBGSTATE)), this, SLOT(stateChangedSlot(DBGSTATE)));
}


void MemoryMapView::stateChangedSlot(DBGSTATE state)
{

    if(state == paused)
    {
        MEMMAP wMemMapStruct;
        int wI;

        memset(&wMemMapStruct, 0, sizeof(MEMMAP));


        Bridge::getBridge()->getMemMapFromDbg(&wMemMapStruct);

        qDebug() << "count " << wMemMapStruct.count;

        setRowCount(wMemMapStruct.count);

        for(wI = 0; wI < wMemMapStruct.count; wI++)
        {
            QString wS;
            MEMORY_BASIC_INFORMATION wMbi = (wMemMapStruct.page)[wI].mbi;

            // Base address
            wS = QString("%1").arg((uint_t)wMbi.BaseAddress, 16, 16, QChar('0')).toUpper();
            setCellContent(wI, 0, wS);

            // Size
            wS = QString("%1").arg((uint_t)wMbi.RegionSize, 16, 16, QChar('0')).toUpper();
            setCellContent(wI, 1, wS);

            // Module Name
            wS = QString((wMemMapStruct.page)[wI].mod);
            setCellContent(wI, 2, wS);

            // State
            switch(wMbi.State)
            {
                case MEM_FREE:
                    wS = QString("Free");
                    break;
                case MEM_COMMIT:
                    wS = QString("Commited");
                    break;
                case MEM_RESERVE:
                    wS = QString("Reserved");
                    break;
                default:
                    wS = QString("N/A");
            }
            setCellContent(wI, 3, wS);

            // Access
            if(wMbi.State != MEM_COMMIT)
            {
                wS = QString("N/A");
            }
            else
            {
                switch(wMbi.Protect & 0xFF)
                {
                    case PAGE_EXECUTE:
                        wS = QString("Execute");
                        break;
                    case PAGE_EXECUTE_READ:
                        wS = QString("Execute/Read");
                        break;
                    case PAGE_EXECUTE_READWRITE:
                        wS = QString("Execute/Read/Write");
                        break;
                    case PAGE_NOACCESS:
                        wS = QString("No Access");
                        break;
                    case PAGE_READONLY:
                        wS = QString("Read");
                        break;
                    case PAGE_READWRITE:
                        wS = QString("Read/Write");
                        break;
                    case PAGE_WRITECOPY:
                        wS = QString("Copy on Write");
                        break;
                    case PAGE_EXECUTE_WRITECOPY:
                        wS = QString("Execute/Copy on Write");
                        break;
                }

                switch(wMbi.Protect & 0xFF00)
                {
                    case PAGE_GUARD:
                        wS += QString(" + Guard");
                }

                setCellContent(wI, 4, wS);
            }

            // Type
            switch(wMbi.Type)
            {
                case MEM_IMAGE:
                    wS = QString("Image");
                    break;
                case MEM_MAPPED:
                    wS = QString("Mapped");
                    break;
                case MEM_PRIVATE:
                    wS = QString("Private");
                    break;
                default:
                    wS = QString("N/A");
                    break;
            }
            setCellContent(wI, 5, wS);

        }

    }

}
