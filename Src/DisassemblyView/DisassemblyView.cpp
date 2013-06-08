#include "DisassemblyView.h"

DisassemblyView::DisassemblyView(MapViewOfMem memory, QWidget *parent) : QAbstractScrollArea(parent)
{
    setFont(QFont("Monospace", 8));
    setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setRowHeight(14);
    setRowCount(10);

    addColumn();
    addColumn();
    addColumn();
    addColumn();
    addColumn();

    mGuiState = DisassemblyView::NoState;

    mColResizeData.splitHandle = false;


    mSelectedRowsData.fromIndex = -1;
    mSelectedRowsData.toIndex = -1;

    mMemoryView = memory;

    verticalScrollBar()->setRange(0, memory.size());

    mTopTableAddress = 0;


    mMultiSelTimer = new QTimer(this);
    mMultiSelTimer->start(100);

    connect(mMultiSelTimer, SIGNAL(timeout()), this, SLOT(multiSelTimerSlot()));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vertSliderMoved(int)));
}

void DisassemblyView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this->viewport());

    int viewableRowsCount = this->viewport()->height() / rowHeight() + 1;
    int x = 0;
    int y = 0;

    // Paint background
    painter.fillRect(painter.viewport(), QBrush(QColor(255, 251, 240)));

    // Iterate over all columns and cells
    for(int i = 0; i < columnCount(); i++)
    {
        for(int j = 0; j < viewableRowsCount; j++)
        {
            if(i < (columnCount() - 1)) // For all columns except the last one
            {
                // Highlight selected rows
                if(((mTopTableAddress + j) >= mSelectedRowsData.fromIndex) && ((mTopTableAddress + j) <= mSelectedRowsData.toIndex))
                    painter.fillRect(QRect(x, y, columnWidth(i), rowHeight()), QBrush(QColor(192,192,192)));

                //  Draw cell content
                painter.drawText(QRect(x + 4, y, columnWidth(i) - 4, rowHeight()), 0, mMemoryView.readByte(mTopTableAddress + j));

                // Draw cell right border
                painter.save() ;
                painter.setPen(QColor(128, 128, 128));
                painter.drawLine(x + columnWidth(i) - 1, y, x + columnWidth(i) - 1, y + rowHeight() - 1);
                painter.restore();

                // Update y for the next iteration
                y += rowHeight();
            }
            else // For the last column only
            {
                // Highlight selected rows
                if(((mTopTableAddress + j) >= mSelectedRowsData.fromIndex) && ((mTopTableAddress + j) <= mSelectedRowsData.toIndex))
                    painter.fillRect(QRect(x, y, painter.viewport().width() - x, rowHeight()), QBrush(QColor(192,192,192)));

                // Draw cell right border
                painter.drawText(QRect(x + 4, y, painter.viewport().width() - x - 4, rowHeight()), 0, mMemoryView.readByte(mTopTableAddress + j));

                // Update y for the next iteration
                y += rowHeight();
            }
        }
        y = 0;
        x += columnWidth(i);
    }
}

void DisassemblyView::mouseMoveEvent(QMouseEvent* event)
{
    int wColIndex = getColumnIndexFromX(event->x());
    int wStartPos = getColumnPosition(wColIndex); // Position X of the start of column
    int wEndPos = getColumnPosition(wColIndex) + columnWidth(wColIndex); // Position X of the end of column

    switch (mGuiState) {
        case DisassemblyView::NoState:
        {
            qDebug() << "State = NoState";

            if(event->buttons() == Qt::NoButton)
            {
                bool handle = true;
                bool hasCursor;

                if(cursor().shape() == Qt::SplitHCursor)
                    hasCursor = true;
                else
                    hasCursor = false;

                if(((wColIndex != 0) && (event->x() >= wStartPos) && (event->x() <= (wStartPos + 2))) || ((wColIndex != (columnCount() - 1)) && (event->x() <= wEndPos) && (event->x() >= (wEndPos - 2))))
                {
                    handle = true;
                }
                else
                {
                    handle = false;
                }

                if((handle == true) && (hasCursor == false))
                {
                    setCursor(Qt::SplitHCursor);
                    mColResizeData.splitHandle = true;
                }
                else if ((handle == false) && (hasCursor == true))
                {
                    unsetCursor();
                    mColResizeData.splitHandle = false;
                }
            }
            else
            {
                QWidget::mouseMoveEvent(event);
            }
            break;
        }
        case DisassemblyView::ResizeColumnState:
        {
            qDebug() << "State = ResizeColumnState";

            int delta = event->x() - mColResizeData.lastPosX;
            int wNewSize = ((columnWidth(mColResizeData.index) + delta) >= 20) ? (columnWidth(mColResizeData.index) + delta) : (20);

            setColumnWidth(mColResizeData.index, wNewSize);
            this->viewport()->repaint();

            mColResizeData.lastPosX = event->x();

            break;
        }
        case DisassemblyView::MultiRowsSelectionState:
        {
            qDebug() << "State = MultiRowsSelectionState";

            int wRowIndex = getRowIndexFromY(event->y());

            if((mTopTableAddress + wRowIndex) < mSelectedRowsData.firstSelectedIndex)
            {
                mSelectedRowsData.fromIndex = mTopTableAddress + wRowIndex;
                mSelectedRowsData.toIndex = mSelectedRowsData.firstSelectedIndex;
            }
            else if((mTopTableAddress + wRowIndex) > mSelectedRowsData.firstSelectedIndex)
            {
                mSelectedRowsData.fromIndex = mSelectedRowsData.firstSelectedIndex;
                mSelectedRowsData.toIndex = mTopTableAddress + wRowIndex;
            }

            viewport()->repaint();

            break;
        }
        default:
            break;
    }
}


void DisassemblyView::mousePressEvent(QMouseEvent* event)
{
    if(((event->buttons() & Qt::LeftButton) != 0) && ((event->buttons() & Qt::RightButton) == 0))
    {
        if(mColResizeData.splitHandle == true)
        {
            int wColIndex = getColumnIndexFromX(event->x());
            int wStartPos = getColumnPosition(wColIndex); // Position X of the start of column

            mGuiState = DisassemblyView::ResizeColumnState;

            if(event->x() <= (wStartPos + 2))
            {
                mColResizeData.index = wColIndex - 1;
            }
            else
            {
                mColResizeData.index = wColIndex;
            }

            mColResizeData.lastPosX = event->x();
        }
        else
        {
            int wRowIndex = getRowIndexFromY(event->y());

            mSelectedRowsData.firstSelectedIndex = mTopTableAddress + wRowIndex;
            mSelectedRowsData.fromIndex = mTopTableAddress + wRowIndex;
            mSelectedRowsData.toIndex = mTopTableAddress + wRowIndex;

            viewport()->repaint();

            mGuiState = DisassemblyView::MultiRowsSelectionState;
        }
    }

    //QWidget::mousePressEvent(event);
}


void DisassemblyView::mouseReleaseEvent(QMouseEvent* event)
{
    if((event->buttons() & Qt::LeftButton) == 0)
    {
        if(mGuiState == DisassemblyView::ResizeColumnState)
        {
            mGuiState = DisassemblyView::NoState;
        }
        else if(mGuiState == DisassemblyView::MultiRowsSelectionState)
        {
            mGuiState = DisassemblyView::NoState;
        }
        else
        {
            QWidget::mouseReleaseEvent(event);
        }
    }
}

void DisassemblyView::wheelEvent(QWheelEvent* event)
{
    QAbstractScrollArea::wheelEvent(event);
    vertSliderMoved(verticalScrollBar()->value());
}

int DisassemblyView::getRowIndexFromY(int y)
{
    return (y / rowHeight());
}

int DisassemblyView::getColumnIndexFromX(int x)
{
    int wX = 0;
    int index = 0;

    while(index < columnCount())
    {
        wX += columnWidth(index);

        if(x <= wX)
        {
            return index;
        }
        else if(index < columnCount())
        {
            index++;
        }
    }
    return -1;
}

int DisassemblyView::getColumnPosition(int index)
{
    int posX = 0;

    if((index >= 0) && (index < columnCount()))
    {
        for(int i = 0; i <= (index - 1); i++)
        {
            posX += columnWidth(i);
        }

        return posX;
    }
    else
    {
        return -1;
    }
}


void DisassemblyView::vertSliderMoved(int value)
{
    qDebug() << "Scroll slot " << value;
    mTopTableAddress = value;

    viewport()->repaint();
}

void DisassemblyView::multiSelTimerSlot()
{
    //qDebug() << "timeout";

    if(mGuiState == DisassemblyView::MultiRowsSelectionState)
    {
        int wY = this->mapFromGlobal(QCursor::pos()).y();

        if((wY < 0) || (wY > this->height()))
        {
            int wScrollBarValue = verticalScrollBar()->value();
            int wNewScrollBarValue;
            int viewableRowsCount = this->height() / rowHeight() + 1;
            int wRowIndex;

            wY = (wY < 0) ? wY : (wY - this->height());

            wNewScrollBarValue = wScrollBarValue + wY;



            if(wY < 0)
            {
                wRowIndex = 0;
            }
            else if(wY > 0)
            {
                wRowIndex = viewableRowsCount - 1;
            }

            if((wNewScrollBarValue + wRowIndex) < mSelectedRowsData.firstSelectedIndex)
            {
                mSelectedRowsData.fromIndex = wNewScrollBarValue + wRowIndex;
                mSelectedRowsData.toIndex = mSelectedRowsData.firstSelectedIndex;
            }
            else if((wNewScrollBarValue + wRowIndex) > mSelectedRowsData.firstSelectedIndex)
            {
                mSelectedRowsData.fromIndex = mSelectedRowsData.firstSelectedIndex;
                mSelectedRowsData.toIndex = wNewScrollBarValue + wRowIndex;
            }

            verticalScrollBar()->setValue(wNewScrollBarValue);
        }
    }
}

/************************************************************************************
                            Getter & Setter
 ***********************************************************************************/
int DisassemblyView::rowHeight()
{
    return mRowHeight;
}

void DisassemblyView::setRowHeight(int heigth)
{
    mRowHeight = heigth;
}

int DisassemblyView::rowCount()
{
    return mRowCount;
}

void DisassemblyView::setRowCount(int count)
{
    mRowCount = count;
}

void DisassemblyView::addColumn()
{
    ColumnItem_t wNewItem;

    wNewItem.width = 50;

    mColumnItemList.append(wNewItem);
}

int DisassemblyView::columnCount()
{
    return mColumnItemList.size();
}

int DisassemblyView::columnWidth(int index)
{
    if((index >= 0) && (index < columnCount()))
    {
        return mColumnItemList.at(index).width;
    }
    else
    {
        return -1;
    }
}

void DisassemblyView::setColumnWidth(int index, int width)
{
    mColumnItemList[index].width = width;
}
