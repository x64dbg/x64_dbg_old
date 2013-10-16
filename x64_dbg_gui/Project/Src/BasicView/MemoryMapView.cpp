#include "MemoryMapView.h"

MemoryMapView::MemoryMapView(StdTable *parent) : StdTable(parent)
{
    enableMultiSelection(false);

    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);


    setRowCount(100);

}
