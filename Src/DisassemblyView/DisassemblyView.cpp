#include "DisassemblyView.h"

/* TODO
 *
 * Change the selection system (When the user select a byte in an instruction, the instruction need to be hilighted)
 *
 *
 */



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

    updateVertScrollbarRange();


    mTopTableRVA = 0;


    mMultiSelTimer = new QTimer(this);
    mMultiSelTimer->start(100);

    connect(mMultiSelTimer, SIGNAL(timeout()), this, SLOT(multiSelTimerSlot()));
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(vertSliderActionSlot(int)));
}

/************************************************************************************
                            Reimplemented Functions
 ***********************************************************************************/
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
                if(isSelected(mTopTableRVA, j) == true)
                    painter.fillRect(QRect(x, y, columnWidth(i), rowHeight()), QBrush(QColor(192,192,192)));

                //  Draw cell content
                painter.drawText(QRect(x + 4, y, columnWidth(i) - 4, rowHeight()), 0, getStringToPrint(mTopTableRVA, j, i));

                // Draw cell right border
                painter.save() ;
                painter.setPen(QColor(128, 128, 128));
                painter.drawLine(x + columnWidth(i) - 1, y, x + columnWidth(i) - 1, y + rowHeight() - 1);
                painter.restore();

                // Update y for the next iteration
                y += rowHeight();
            }
            else // For the last column only (Stretch + No right border)
            {
                // Highlight selected rows
                if(isSelected(mTopTableRVA, j) == true)
                    painter.fillRect(QRect(x, y, painter.viewport().width() - x, rowHeight()), QBrush(QColor(192,192,192)));

                // Draw cell right border
                painter.drawText(QRect(x + 4, y, painter.viewport().width() - x - 4, rowHeight()), 0, getStringToPrint(mTopTableRVA, j, i));

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

            if((event->y() >= 0) || (event->y() <= this->height()))
            {

                int wRowIndex = getRowIndexFromY(event->y());

                expandSelectionUpTo(mTopTableRVA, wRowIndex);

                viewport()->repaint();
            }
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

            setSingleSelection(mTopTableRVA, wRowIndex);

            mGuiState = DisassemblyView::MultiRowsSelectionState;

            viewport()->repaint();
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
    qDebug() << "wheelEvent";

    //QAbstractScrollArea::wheelEvent(event);

    ulong rva = verticalScrollBar()->value();

    if(event->delta() > 0)
    {
        rva = getPreviousInstructionRVA(rva, 3);
    }
    else
    {
        rva = getNextInstructionRVA(rva, 3);
    }

    verticalScrollBar()->setValue(rva);
    verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
}


void DisassemblyView::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    ulong rva = getFirstSelected();
    ulong newRVA;
    int viewableRowsCount = this->viewport()->height() / rowHeight(); // Rounded down
    ulong lastInstrRVA = getNextInstructionRVA(mTopTableRVA, viewableRowsCount - 1);

    if(key == Qt::Key_Up)
    {
        newRVA = getPreviousInstructionRVA(rva, 1);
        setSingleSelection(newRVA, 0);

        if(rva <= mTopTableRVA)
        {
            verticalScrollBar()->setValue(newRVA);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
        else if(rva > lastInstrRVA)
        {

            newRVA = getPreviousInstructionRVA(newRVA, viewableRowsCount - 1);

            verticalScrollBar()->setValue(newRVA);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
    }
    else if(key == Qt::Key_Down)
    {
        newRVA = getNextInstructionRVA(rva, 1);
        setSingleSelection(newRVA, 0);

        if(rva < mTopTableRVA)
        {
            verticalScrollBar()->setValue(newRVA);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
        else if(rva >= lastInstrRVA)
        {
            newRVA = getPreviousInstructionRVA(newRVA, viewableRowsCount - 1);

            verticalScrollBar()->setValue(newRVA);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
    }

    viewport()->repaint();
}


void DisassemblyView::contextMenuEvent(QContextMenuEvent* event)
{
    QPoint globalPos = event->globalPos();

    QMenu myMenu;
    myMenu.addAction("Menu Item 1");
    myMenu.addAction("Menu Item 2");
    myMenu.addAction("Menu Item 3");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
    }
    else
    {
        // nothing was chosen
    }
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

            if(wY < 0)
            {
                wRowIndex = 0;
                wNewScrollBarValue = getPreviousInstructionRVA(wScrollBarValue, qAbs(wY));
                wNewScrollBarValue = (wNewScrollBarValue < 0) ? 0 : wNewScrollBarValue;
            }
            else if(wY > 0)
            {
                wRowIndex = viewableRowsCount - 1;
                wNewScrollBarValue = getNextInstructionRVA(wScrollBarValue, qAbs(wY));
            }

            expandSelectionUpTo(wNewScrollBarValue, wRowIndex);


            verticalScrollBar()->setValue(wNewScrollBarValue);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
    }
}

/************************************************************************************
                            Selection Management
 ***********************************************************************************/
/**
 * @brief       Check if the instruction located count instructions after the address rva is selected or not.
 *
 * @param[in]   rva     Address an instruction
 * @param[in]   count   Number of instructions
 *
 * @return      Return true if the instruction is selected or false otherwise.
 */
bool DisassemblyView::isSelected(ulong rva, ulong count)
{
    int wRVA = getNextInstructionRVA(rva, count);

    if((wRVA >= mSelectedRowsData.fromIndex) && (wRVA <= mSelectedRowsData.toIndex))
        return true;
    else
        return false;
}


/**
 * @brief       Expand the selection up to the instruction located count instructions after the address rva. @n
 *              If the instruction pointed by the parameters is already selected, the selction will be reduce up to this instruction.
 *
 * @param[in]   rva     Address an instruction
 * @param[in]   count   Offset (In number of instructions)
 *
 * @return      Nothing.
 */
void DisassemblyView::expandSelectionUpTo(ulong rva, ulong count)
{
    int wRVA = getNextInstructionRVA(rva, count);

    if(wRVA < mSelectedRowsData.firstSelectedIndex)
    {
        mSelectedRowsData.fromIndex = wRVA;
        mSelectedRowsData.toIndex = mSelectedRowsData.firstSelectedIndex;
    }
    else if(wRVA > mSelectedRowsData.firstSelectedIndex)
    {
        mSelectedRowsData.fromIndex = mSelectedRowsData.firstSelectedIndex;
        mSelectedRowsData.toIndex = wRVA;
    }
}


/**
 * @brief       Select only the instruction located count instructions after the address rva to the selection.
 *
 * @param[in]   rva     Address an instruction
 * @param[in]   count   Number of instructions
 *
 * @return      Nothing.
 */
void DisassemblyView::setSingleSelection(ulong rva, ulong count)
{
    ulong wRVA = getNextInstructionRVA(rva, count);

    mSelectedRowsData.firstSelectedIndex = wRVA;
    mSelectedRowsData.fromIndex = wRVA;
    mSelectedRowsData.toIndex = wRVA;
}

/**
 * @brief       Return the rva of the selected instruction. In case of multi-selection, the first selected instruction.
 *
 * @return      Return the rva of the selected instruction.
 */
int DisassemblyView::getFirstSelected()
{
    return mSelectedRowsData.firstSelectedIndex;
}


/************************************************************************************
                            Instructions Management
 ***********************************************************************************/
int DisassemblyView::getPreviousInstructionRVA(int address, int count)
{
    ulong addr = mDisasm.DisassembleBack((char*)mMemoryView.data(), (ulong)mMemoryView.data(), (ulong)mMemoryView.size(), (ulong)(mMemoryView.data() + address), count);
    return addr - (ulong)mMemoryView.data();

    //return (count > address ? 0 : address - count);
}

int DisassemblyView::getNextInstructionRVA(int address, int count)
{
    ulong addr = mDisasm.DisassembleNext((char*)mMemoryView.data(),(ulong)mMemoryView.data(), (ulong)mMemoryView.size(), (ulong)(mMemoryView.data() + address), count);
    return addr - (ulong)mMemoryView.data();

    //return address + count;
}

QString DisassemblyView::getStringToPrint(int topTableAddress, int rowIndex, int colIndex)
{
    char* data = (char*)mMemoryView.data();
    ulong base = 0;
    ulong size = mMemoryView.size();
    ulong ip = getNextInstructionRVA(topTableAddress, rowIndex);
    Instruction_t instruction = mDisasm.DisassembleAt(data, base, size, ip);
    QString string = "";

    switch(colIndex)
    {
        case 0:
        {
            string = QString("%1").arg(instruction.rva, 8, 16, QChar('0')).toUpper();
            break;
        }
        case 1:
        {
            for(int i = 0; i < instruction.dump.size(); i++)
                string += QString::number((unsigned char)(instruction.dump.at(i)), 16).toUpper();;
            break;
        }
        case 2:
        {
            string = instruction.instStr.toUpper();
            break;
        }
        default:
            break;
    }

    return string;
}

/************************************************************************************
                            ScrollBar Management
 ***********************************************************************************/
void DisassemblyView::vertSliderActionSlot(int action)
{
    qDebug() << "Scroll Action Slot " << verticalScrollBar()->value();

    switch(action)
    {
        case QAbstractSlider::SliderSingleStepSub:
            {
                qDebug() << "SliderSingleStepSub";
                int address = verticalScrollBar()->value();
                address = getPreviousInstructionRVA(address, 1);
                verticalScrollBar()->setSliderPosition(address);
                mTopTableRVA = address;
            }
            break;
        case QAbstractSlider::SliderPageStepSub:
            {
                qDebug() << "SliderPageStepSub";
                int address = verticalScrollBar()->value();
                address = getPreviousInstructionRVA(address, verticalScrollBar()->pageStep());
                verticalScrollBar()->setSliderPosition(address);
                mTopTableRVA = address;
            }
            break;
        case QAbstractSlider::SliderSingleStepAdd:
            {
                qDebug() << "SliderSingleStepAdd";
                qDebug() << "debug";
                qDebug() << "mTopTableAddress " << mTopTableRVA;
                int address = verticalScrollBar()->value();
                address = getNextInstructionRVA(mTopTableRVA, 1);
                qDebug() << "address " << address;
                verticalScrollBar()->setSliderPosition(address);
                mTopTableRVA = address;
            }
            break;
        case QAbstractSlider::SliderPageStepAdd:
            {
                qDebug() << "SliderPageStepAdd";
                int address = verticalScrollBar()->value();
                address = getNextInstructionRVA(address, verticalScrollBar()->pageStep());
                verticalScrollBar()->setSliderPosition(address);
                mTopTableRVA = address;
            }
            break;

        case QAbstractSlider::SliderToMinimum:
        case QAbstractSlider::SliderToMaximum:
        case QAbstractSlider::SliderMove:
            {
                qDebug() << "SliderMove";
                /*
                int delta = verticalScrollBar()->sliderPosition() - mTopTableRVA;

                if(qAbs(delta) < 10)
                {
                    qDebug() << "SliderMove 1";
                    if(delta < 0)
                    {
                        qDebug() << "SliderMove 2";
                        int address = verticalScrollBar()->value();
                        address = getPreviousInstructionRVA(address, qAbs(delta));
                        verticalScrollBar()->setSliderPosition(address);
                        mTopTableRVA = address;
                    }
                    else if(delta > 0)
                    {
                        qDebug() << "SliderMove 3";
                        int address = verticalScrollBar()->value();
                        address = getNextInstructionRVA(address, qAbs(delta));
                        qDebug() << "address retruned " << address;
                        verticalScrollBar()->setValue(address);
                        mTopTableRVA = address;
                    }
                    qDebug() << "SliderMove 4";
                }
                else
                {
                    mTopTableRVA = verticalScrollBar()->sliderPosition();
                }
                */

                int rva = verticalScrollBar()->sliderPosition();

                rva = getPreviousInstructionRVA(rva, 1);
                rva = getNextInstructionRVA(rva, 1);

                mTopTableRVA = rva;
            }
            break;
        case QAbstractSlider::SliderNoAction:
        default:
            break;
    }


    viewport()->repaint();
    qDebug() << "\n";
}


void DisassemblyView::updateVertScrollbarRange()
{
    int wMemSize = mMemoryView.size();
    int wViewableRowsCount = this->viewport()->height() / rowHeight();  // Round down
    int wScrollMax = (wMemSize > wViewableRowsCount) ? wMemSize - 1 : 0;

    verticalScrollBar()->setRange(0, wScrollMax);
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
