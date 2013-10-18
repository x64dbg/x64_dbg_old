#include "MemoryMapView.h"

MemoryMapView::MemoryMapView(StdTable *parent) : StdTable(parent)
{
    enableMultiSelection(false);

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
            setCellContent(wI, 0, QString::number((uint_t)(wMemMapStruct.page)[wI].mbi.BaseAddress));
        }

    }

}
