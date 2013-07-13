#include "HexDump.h"

HexDump::HexDump(QWidget *parent) :AbstractTableView(parent)
{
    mSelection = (SelectionData_t){0, 0, 0};

    mMemoryView = new MapViewOfMem("AsmCode.bin");
    mDisasm = new QBeaEngine();

    setRowCount(mMemoryView->size());
}


void HexDump::AbstractMemberFunction(void)
{
    qDebug() << "Disassembly: AbstractMemberFunction";
}


QString HexDump::getStringToPrint(int rowBase, int rowOffset, int col)
{
    return "Row: " + QString::number(rowBase + rowOffset)+" Col: " + QString::number(col);
}
