#include "HexDump.h"

#include "HexDump.h"

HexDump::HexDump(QWidget *parent) :AbstractTableView(parent)
{
    mSelection = (SelectionData_t){0, 0, 0};

    mMemoryView = new MapViewOfMem("AsmCode.bin");


    mGuiState = HexDump::NoState;


    mByteWidth = QFontMetrics(this->font()).width(QChar('L')) + 4;
    mDumpByteWidth = 16;

    setRowCount(mMemoryView->size() / 16);

    qDebug() << "size" << getRowCount();
}



void HexDump::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "HexDump::mouseMoveEvent";

    bool wAccept = true;

    if(mGuiState == HexDump::MultiRowsSelectionState)
    {
        qDebug() << "State = MultiRowsSelectionState";

        if((transY(event->y()) >= 0) && (transY(event->y()) <= this->getTableHeigth()))
        {
            int wRowIndex = getTableOffset() +getRowOffsetFromY(transY(event->y()));

            if(wRowIndex < getRowCount())
            {
                expandSelectionUpTo(wRowIndex);

                this->viewport()->repaint();

                wAccept = false;
            }
        }
    }

    if(wAccept == true)
        AbstractTableView::mouseMoveEvent(event);
}



void HexDump::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "HexDump::mousePressEvent";

    bool wAccept = false;

    if(((event->buttons() & Qt::LeftButton) != 0) && ((event->buttons() & Qt::RightButton) == 0))
    {
        if(getGuiState() == AbstractTableView::NoState)
        {
            if(event->y() > getHeaderHeigth())
            {
                int wRowIndex = getTableOffset()+ getRowOffsetFromY(transY(event->y()));
                int wColIndex = getColumnIndexFromX(event->x());
                int wByteIndex;

                if(wRowIndex < getRowCount())
                {
                    if(wColIndex == 1)
                    {
                        int wX = event->x() - getColumnPosition(wColIndex);

                    }
                    else if(wColIndex == 2)
                    {
                        int wColBegin = getColumnPosition(wColIndex);

                    }

                    setSingleSelection(wRowIndex);

                    mGuiState = HexDump::MultiRowsSelectionState;

                    viewport()->repaint();

                    wAccept = true;
                }
            }
        }
    }

    if(wAccept == false)
        AbstractTableView::mousePressEvent(event);
}



void HexDump::mouseReleaseEvent(QMouseEvent* event)
{
    bool wAccept = true;

    if((event->buttons() & Qt::LeftButton) == 0)
    {
        if(mGuiState == HexDump::MultiRowsSelectionState)
        {
            mGuiState = HexDump::NoState;

            this->viewport()->repaint();

            wAccept = false;
        }
    }

    if(wAccept == true)
        AbstractTableView::mouseReleaseEvent(event);
}





QString HexDump::paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h)
{
    //return QString("HexDump: Col:") + QString::number(col) + "Row:" + QString::number(rowBase + rowOffset);

    QString wStr = "";
    int wRva = (rowBase + rowOffset) * mDumpByteWidth;


    //if(isSelected(rowBase, rowOffset) == true)
    //    painter->fillRect(QRect(x, y, w, h), QBrush(QColor(192,192,192)));

    switch(col)
    {
        case 0:
        {
            //ulong wAddr = (ulong)instruction.rva + (ulong)mMemoryView->getBase();
            //wStr = QString("%1").arg(wAddr, 8, 16, QChar('0')).toUpper();
            wStr += QString::number(wRva);
            break;
        }

        case 1:
        {
            for(int i = 0; i < mDumpByteWidth; i++)
                wStr += QString("%1").arg(mMemoryView->data()[wRva + i], 2, 16, QChar('0')).toUpper() + " ";
            break;
        }

        case 2:
        {
            wStr = "ToDo";
            break;
        }

        default:
            break;
    }

    return wStr;
}




/************************************************************************************
                                Selection Management
************************************************************************************/
void HexDump::expandSelectionUpTo(int to)
{
    if(to < mSelection.firstSelectedIndex)
    {
        mSelection.fromIndex = to;
        mSelection.toIndex = mSelection.firstSelectedIndex;
    }
    else if(to > mSelection.firstSelectedIndex)
    {
        mSelection.fromIndex = mSelection.firstSelectedIndex;
        mSelection.toIndex = to;
    }
}


void HexDump::setSingleSelection(int index)
{
    mSelection.firstSelectedIndex = index;
    mSelection.fromIndex = index;
    mSelection.toIndex = index;
}


int HexDump::getInitialSelection()
{
    return mSelection.firstSelectedIndex;
}


bool HexDump::isSelected(int base, int offset)
{
    /*
    int wAddr = base;

    if(offset < 0)
        wAddr = getPreviousInstructionRVA(getTableOffset(), offset);
    else if(offset > 0)
        wAddr = getNextInstructionRVA(getTableOffset(), offset);

    if(wAddr >= mSelection.fromIndex && wAddr <= mSelection.toIndex)
        return true;
    else
        return false;
        */
    return false;
}


