#include "StdTable.h"

StdTable::StdTable(QWidget *parent) : AbstractTableView(parent)
{
    setRowCount(10000);
}


void StdTable::AbstractMemberFunction(void)
{
    qDebug() << "Disassembly: AbstractMemberFunction";
}

QString StdTable::getStringToPrint(int rowBase, int rowOffset, int col)
{
    return "Row: " + QString::number(rowBase + rowOffset)+" Col: " + QString::number(col);
}
