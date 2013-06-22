#include "DisassemblyView.h"

/* TODO
 *
 *
 *
 */



DisassemblyView::DisassemblyView(MapViewOfMem memory, QWidget *parent) : QAbstractScrollArea(parent)
{
    setFont(QFont("Monospace", 9));
    setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    int wRowsHeight = QFontMetrics(this->font()).height();
    wRowsHeight = (wRowsHeight * 105) / 100;
    wRowsHeight = (wRowsHeight % 2) == 0 ? wRowsHeight : wRowsHeight + 1;

    setRowHeight(wRowsHeight);
    setRowCount(10);


    mHeader.height = 20;

    addColumn();
    addColumn();
    addColumn();
    addColumn();
    addColumn();

    setHeaderVisible(true);


    mGuiState = DisassemblyView::NoState;

    mColResizeData.splitHandle = false;

    button.setStyleSheet(" QPushButton {\n     background-color: rgb(192, 192, 192);\n     border-style: outset;\n     border-width: 2px;\n     border-color: rgb(128, 128, 128);\n }\n QPushButton:pressed {\n     background-color: rgb(192, 192, 192);\n     border-style: inset;\n }");

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

    int viewableRowsCount = this->trueHeight() / rowHeight() + 1;
    int x = 0;
    int y = 0;

    // Paint background
    painter.fillRect(painter.viewport(), QBrush(QColor(255, 251, 240)));
    //painter.fillRect(painter.viewport(), QBrush(QColor(192, 192, 192)));
/*
    QTextDocument doc;
    doc.setDocumentMargin(0);
    doc.setUndoRedoEnabled(false);
    doc.setTextWidth(this->width());
    doc.setHtml("<span style=\" font-family:'Monospace'; font-size:9pt;\">CALL <FONT style=\"BACKGROUND-COLOR: yellow\">next </FONT></span>");
    painter.save();
    painter.translate(0,0);
    doc.drawContents(&painter, QRectF(QRect(0,0,50,40)));
    painter.restore();
*/


    // Draw header
    if(mHeader.isHeaderVisible == true)
    {
        for(int i = 0; i < columnCount(); i++)
        {
            if(i < (columnCount() - 1)) // For all columns except the last one
            {
                QStyleOptionButton opt;
                if((mColumnItemList.at(i).header.isPressed == true) && (mColumnItemList[mPressedHeaderCol].header.isMouseOver == true))
                    opt.state = QStyle::State_Sunken;
                else
                    opt.state = QStyle::State_Enabled;
                opt.rect = QRect(x, y, columnWidth(i), mHeader.height);
                button.style()->drawControl(QStyle::CE_PushButton, &opt, &painter,&button);

                x += columnWidth(i);
            }
            else
            {
                QStyleOptionButton opt;
                if(mColumnItemList.at(i).header.isPressed == false)
                    opt.state = QStyle::State_Enabled;
                else
                    opt.state = QStyle::State_Sunken;
                opt.rect = QRect(x, y, painter.viewport().width() - x, mHeader.height);
                button.style()->drawControl(QStyle::CE_PushButton, &opt, &painter,&button);

                x += columnWidth(i);
            }
        }
        x = 0;
        y = headerOffset();
    }

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


                if(i == 1)
                {
                    paintGraphicDump(&painter, x + 5, y, mTopTableRVA, j);

                    //  Draw cell content
                    painter.drawText(QRect(x + 15, y, columnWidth(i) - 15, rowHeight()), 0, getStringToPrint(mTopTableRVA, j, i));
                }
                else
                {
                    //  Draw cell content
                    painter.drawText(QRect(x + 4, y, columnWidth(i) - 4, rowHeight()), 0, getStringToPrint(mTopTableRVA, j, i));
                }

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

                //  Draw cell content
                painter.drawText(QRect(x + 4, y, painter.viewport().width() - x - 4, rowHeight()), 0, getStringToPrint(mTopTableRVA, j, i));

                // Update y for the next iteration
                y += rowHeight();
            }
        }
        y = headerOffset();
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

            mColResizeData.lastPosX = event->x();

            this->viewport()->repaint();

            break;
        }
        case DisassemblyView::MultiRowsSelectionState:
        {
            qDebug() << "State = MultiRowsSelectionState";

            if((transY(event->y()) >= 0) || (transY(event->y()) <= this->trueHeight()))
            {

                int wRowIndex = getRowIndexFromY(transY(event->y()));

                expandSelectionUpTo(mTopTableRVA, wRowIndex);

                this->viewport()->repaint();
            }
            break;
        }
        case DisassemblyView::HeaderButtonPressed:
        {
            qDebug() << "State = HeaderButtonPressed";

            int wColIndex = getColumnIndexFromX(event->x());

            if((wColIndex == mPressedHeaderCol) && (event->y() <= mHeader.height) && (event->y() >= 0))
            {
                mColumnItemList[mPressedHeaderCol].header.isMouseOver = true;
            }
            else
            {
                mColumnItemList[mPressedHeaderCol].header.isMouseOver = false;
            }

            this->viewport()->repaint();
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
        else if((mHeader.isHeaderVisible == true) && (event->y() <= mHeader.height) && (event->y() >= 0))
        {
            int wColIndex = getColumnIndexFromX(event->x());

            qDebug() << "Button " << wColIndex << "has been pressed.";

            mColumnItemList[wColIndex].header.isPressed = true;
            mColumnItemList[wColIndex].header.isMouseOver = true;

            mPressedHeaderCol = wColIndex;

            mGuiState = DisassemblyView::HeaderButtonPressed;

            this->viewport()->repaint();
        }
        else
        {
            int wRowIndex = getRowIndexFromY(transY(event->y()));

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
        else if(mGuiState == DisassemblyView::HeaderButtonPressed)
        {
            if(mColumnItemList[mPressedHeaderCol].header.isMouseOver == true)
            {
                //TODO: send event
                qDebug() << "Button " << mPressedHeaderCol << "has been released.";
            }
            mGuiState = DisassemblyView::NoState;
        }
        else
        {
            QWidget::mouseReleaseEvent(event);
        }


        for(int i = 0; i < columnCount(); i++)
        {
            mColumnItemList[i].header.isPressed = false;
        }

        this->viewport()->repaint();
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
    ulong rva = getInitialSelected();
    ulong newRVA;
    int viewableRowsCount = trueHeight() / rowHeight(); // Rounded down
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
    return columnCount() - 1;
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
        int wY = transY(this->mapFromGlobal(QCursor::pos()).y());

        if((wY < 0) || (wY > this->trueHeight()))
        {
            int wScrollBarValue = verticalScrollBar()->value();
            int wNewScrollBarValue;
            int viewableRowsCount = this->trueHeight() / rowHeight() + 1;
            int wRowIndex;

            wY = (wY < 0) ? wY : (wY - this->trueHeight());

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
    Selection_t selection = mMemoryView.getSelection();
    int wRVA = getNextInstructionRVA(rva, count);
    ulong startRVA = wRVA;
    ulong stopRVA = wRVA + DisassembleAt(wRVA).lentgh - 1;

    if(((startRVA >= selection.fromIndex) && (startRVA <= selection.toIndex)) || ((stopRVA >= selection.fromIndex) && (stopRVA <= selection.toIndex)))
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
    Selection_t selection = mMemoryView.getSelection();
    int wRVA = getNextInstructionRVA(rva, count);
    Instruction_t instruction = DisassembleAt(wRVA);

    if(wRVA < selection.firstSelectedIndex)
    {
        selection.fromIndex = wRVA;
        selection.toIndex = selection.firstSelectedIndex;
    }
    else if(wRVA > selection.firstSelectedIndex)
    {
        selection.fromIndex = selection.firstSelectedIndex;
        selection.toIndex = wRVA + instruction.lentgh - 1;
    }
    mMemoryView.setSelection(selection);
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
    Selection_t selection = mMemoryView.getSelection();
    ulong wRVA = getNextInstructionRVA(rva, count);
    Instruction_t instruction = DisassembleAt(wRVA);

    selection.firstSelectedIndex = wRVA;
    selection.fromIndex = wRVA;
    selection.toIndex = wRVA + instruction.lentgh - 1;

    mMemoryView.setSelection(selection);
}

/**
 * @brief       Return the rva of the selected instruction. In case of multi-selection, the initial instructions is returned (Instruction that was pressed).
 *
 * @return      Return the rva of the selected instruction.
 */
int DisassemblyView::getInitialSelected()
{
    return mMemoryView.getSelection().firstSelectedIndex;
}


/**
 * @brief       Return the rva of the selection head.
 *
 * @return      Return the rva of the selection head.
 */
int DisassemblyView::getSelectionHead()
{
    return mMemoryView.getSelection().fromIndex;
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

Instruction_t DisassemblyView::DisassembleAt(ulong rva)
{
    char* data = (char*)mMemoryView.data();
    ulong base = mMemoryView.getBase();
    ulong size = mMemoryView.size();
    ulong ip = base + rva;

    return mDisasm.DisassembleAt(data, base, size, ip);
}

Instruction_t DisassemblyView::DisassembleAt(ulong rva, ulong count)
{
    rva = getNextInstructionRVA(rva, count);

    char* data = (char*)mMemoryView.data();
    ulong base = mMemoryView.getBase();
    ulong size = mMemoryView.size();
    ulong ip = base + rva;

    return mDisasm.DisassembleAt(data, base, size, ip);
}


QString DisassemblyView::getStringToPrint(int topTableAddress, int rowIndex, int colIndex)
{
    ulong ip = getNextInstructionRVA(topTableAddress, rowIndex);
    QString string = "";

    if(ip < mMemoryView.size())
    {
        Instruction_t instruction = DisassembleAt(ip);

        switch(colIndex)
        {
            case 0:
            {
                ulong addr = (ulong)instruction.rva + (ulong)mMemoryView.getBase();
                string = QString("%1").arg(addr, 8, 16, QChar('0')).toUpper();
                break;
            }
            case 1:
            {
                for(int i = 0; i < instruction.dump.size(); i++)
                    string += QString("%1").arg((unsigned char)(instruction.dump.at(i)), 2, 16, QChar('0')).toUpper();
                break;
            }
            case 2:
            {
                string = instruction.instStr.toUpper();
                break;
            }
            case 3:
            {
                break;
            }
            default:
                break;
        }
    }

    return string;
}


void DisassemblyView::paintGraphicDump(QPainter* painter, int x, int y, int topTableRVA, int rowIndex)
{
    ulong selHeadRVA = getSelectionHead();
    ulong rva = getNextInstructionRVA(topTableRVA, rowIndex);
    Instruction_t instruction = DisassembleAt(selHeadRVA);
    Int32 branchType = instruction.disasm.Instruction.BranchType;
    GraphicDump_t wPict = GD_Nothing;

    if(     branchType == (Int32)JO      ||
            branchType == (Int32)JC      ||
            branchType == (Int32)JE      ||
            branchType == (Int32)JA      ||
            branchType == (Int32)JS      ||
            branchType == (Int32)JP      ||
            branchType == (Int32)JL      ||
            branchType == (Int32)JG      ||
            branchType == (Int32)JB      ||
            branchType == (Int32)JECXZ   ||
            branchType == (Int32)JmpType ||
            branchType == (Int32)RetType ||
            branchType == (Int32)JNO     ||
            branchType == (Int32)JNC     ||
            branchType == (Int32)JNE     ||
            branchType == (Int32)JNA     ||
            branchType == (Int32)JNS     ||
            branchType == (Int32)JNP     ||
            branchType == (Int32)JNL     ||
            branchType == (Int32)JNG     ||
            branchType == (Int32)JNB)
    {
        ulong destRVA = (ulong)instruction.disasm.Instruction.AddrValue;

        if(destRVA > (ulong)mMemoryView.getBase())
        {
            destRVA -= (ulong)mMemoryView.getBase();

            if(destRVA < selHeadRVA)
            {
                if(rva == destRVA)
                    wPict = GD_HeadFromBottom;
                else if(rva > destRVA && rva < selHeadRVA)
                    wPict = GD_Vert;
                else if(rva == selHeadRVA)
                    wPict = GD_FootToTop;
            }
            else if(destRVA > selHeadRVA)
            {
                if(rva == selHeadRVA)
                    wPict = GD_FootToBottom;
                else if(rva > selHeadRVA && rva < destRVA)
                    wPict = GD_Vert;
                else if(rva == destRVA)
                    wPict = GD_HeadFromTop;
            }
        }
    }

    painter->save() ;
    painter->setPen(QColor(0, 0, 0));

    if(wPict == GD_Vert)
    {
        painter->drawLine(x, y, x, y + rowHeight());
    }
    else if(wPict == GD_FootToBottom)
    {
        painter->drawLine(x, y + rowHeight() / 2, x + 5, y + rowHeight() / 2);
        painter->drawLine(x, y + rowHeight() / 2, x, y + rowHeight());
    }
    if(wPict == GD_FootToTop)
    {
        painter->drawLine(x, y + rowHeight() / 2, x + 5, y + rowHeight() / 2);
        painter->drawLine(x, y, x, y + rowHeight() / 2);
    }
    else if(wPict == GD_HeadFromBottom)
    {
        QPoint wPoints[] = {
             QPoint(x + 3, y + rowHeight() / 2 - 2),
             QPoint(x + 5, y + rowHeight() / 2),
             QPoint(x + 3, y + rowHeight() / 2 + 2),
         };

        painter->drawLine(x, y + rowHeight() / 2, x + 5, y + rowHeight() / 2);
        painter->drawLine(x, y + rowHeight() / 2, x, y + rowHeight());
        painter->drawPolyline(wPoints, 3);
    }
    if(wPict == GD_HeadFromTop)
    {
        QPoint wPoints[] = {
             QPoint(x + 3, y + rowHeight() / 2 - 2),
             QPoint(x + 5, y + rowHeight() / 2),
             QPoint(x + 3, y + rowHeight() / 2 + 2),
         };

        painter->drawLine(x, y + rowHeight() / 2, x + 5, y + rowHeight() / 2);
        painter->drawLine(x, y, x, y + rowHeight() / 2);
        painter->drawPolyline(wPoints, 3);
    }

    painter->restore();
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

                int rva = verticalScrollBar()->sliderPosition();

                if((rva != 0) && (rva != (mMemoryView.size()-1)))
                {
                    rva = getPreviousInstructionRVA(rva, 1);
                    rva = getNextInstructionRVA(rva, 1);
                }

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
    int wViewableRowsCount = this->trueHeight() / rowHeight();  // Round down
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

    wNewItem.width = 100;
    wNewItem.header.isClickable = false;
    wNewItem.header.isPressed = false;

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


int DisassemblyView::headerOffset()
{
    if(mHeader.isHeaderVisible == true)
        return mHeader.height;
    else
        return 0;
}

int DisassemblyView::transY(int y)
{
    return y - headerOffset();
}

int DisassemblyView::trueHeight()
{
    return this->height() - headerOffset();
}

void DisassemblyView::setHeaderVisible(bool visible)
{
   mHeader.isHeaderVisible = visible;
}
