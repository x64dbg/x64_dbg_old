#include "Disassembly.h"



Disassembly::Disassembly(MemoryPage* parMemPage, QWidget *parent) : AbstractTableView(parent)
{
    setMemoryPage(parMemPage);

    mSelection = (SelectionData_t){0, 0, 0};

    mCipRva = 0;

    mDisasm = new QBeaEngine();

    mIsLastInstDisplayed = false;

    mGuiState = Disassembly::NoState;

    setRowCount(parMemPage->getSize());

    qDebug() << "size" << parMemPage->getSize();

    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);

    connect(Bridge::getBridge(), SIGNAL(eipChanged(uint_t)), this, SLOT(disassambleAt(uint_t)));
}


/************************************************************************************
                            Reimplemented Functions
************************************************************************************/
/**
 * @brief       This method has been reimplemented. It returns the string to paint or paints it
 *              by its own.
 *
 * @param[in]   painter     Pointer to the painter that allows painting by its own
 * @param[in]   rowBase     Index of the top item (Table offset)
 * @param[in]   rowOffset   Index offset starting from rowBase
 * @param[in]   col         Column index
 * @param[in]   x           Rectangle x
 * @param[in]   y           Rectangle y
 * @param[in]   w           Rectangle width
 * @param[in]   h           Rectangle heigth
 *
 * @return      Nothing.
 */
QString Disassembly::paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h)
{
    //return QString("Disassembly: Col:") + QString::number(col) + "Row:" + QString::number(rowBase + rowOffset);

    QString wStr = "";
    int wRva = getIndexFromCount(rowBase, rowOffset);
    Instruction_t wInst = DisassembleAt(wRva);


    if(isSelected(rowBase, rowOffset) == true)
        painter->fillRect(QRect(x, y, w, h), QBrush(QColor(192,192,192)));

    switch(col)
    {
        case 0:
        {
            //ulong wAddr = (ulong)instruction.rva + (ulong)mMemoryView->getBase();
            //wStr = QString("%1").arg(wAddr, 8, 16, QChar('0')).toUpper();
            // QString::number(wInst.rva)



            if(wInst.rva == mCipRva)
            {
                painter->fillRect(QRect(x, y, w, h), QBrush(QColor(0,0,0)));
                painter->save();
                painter->setPen(QPen(QColor("#ffffff")));
                painter->drawText(QRect(x + 4, y , w - 4 , h), Qt::AlignVCenter | Qt::AlignLeft, QString("%1").arg(wInst.rva, 8, 16, QChar('0')).toUpper());
                painter->restore();
            }
            else
                wStr += QString("%1").arg(wInst.rva, 8, 16, QChar('0')).toUpper();

            break;
        }

        case 1:
        {
            for(int i = 0; i < wInst.dump.size(); i++)
                wStr += QString("%1").arg((unsigned char)(wInst.dump.at(i)), 2, 16, QChar('0')).toUpper();

            paintGraphicDump(painter, x + 5, y, wRva);

            // Draw cell content
            painter->drawText(QRect(x + 15, y, getColumnWidth(col) - 15, getRowHeight()), 0, wStr);

            wStr = "";
            break;
        }

        case 2:
        {
            wStr = wInst.instStr.toUpper();
            break;
        }

        default:
            break;
    }
    return wStr;
}


/**
 * @brief       This method has been reimplemented. It manages the following actions:
 *               - Multi-rows selection
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void Disassembly::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "Disassembly::mouseMoveEvent";

    bool wAccept = true;

    if(mGuiState == Disassembly::MultiRowsSelectionState)
    {
        qDebug() << "State = MultiRowsSelectionState";

        if((transY(event->y()) >= 0) && (transY(event->y()) <= this->getTableHeigth()))
        {
            int wRowIndex = getIndexFromCount(getTableOffset(), getIndexOffsetFromY(transY(event->y())));

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


/**
 * @brief       This method has been reimplemented. It manages the following actions:
 *               - Multi-rows selection
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void Disassembly::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Disassembly::mousePressEvent";

    bool wAccept = false;

    if(((event->buttons() & Qt::LeftButton) != 0) && ((event->buttons() & Qt::RightButton) == 0))
    {
        if(getGuiState() == AbstractTableView::NoState)
        {
            if(event->y() > getHeaderHeigth())
            {
                int wRowIndex = getIndexFromCount(getTableOffset(), getIndexOffsetFromY(transY(event->y())));

                if(wRowIndex < getRowCount())
                {
                    setSingleSelection(wRowIndex);

                    mGuiState = Disassembly::MultiRowsSelectionState;

                    viewport()->repaint();

                    wAccept = true;
                }
            }
        }
    }

    if(wAccept == false)
        AbstractTableView::mousePressEvent(event);
}


/**
 * @brief       This method has been reimplemented. It manages the following actions:
 *               - Multi-rows selection
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void Disassembly::mouseReleaseEvent(QMouseEvent* event)
{
    bool wAccept = true;

    if((event->buttons() & Qt::LeftButton) == 0)
    {
        if(mGuiState == Disassembly::MultiRowsSelectionState)
        {
            mGuiState = Disassembly::NoState;

            this->viewport()->repaint();

            wAccept = false;
        }
    }

    if(wAccept == true)
        AbstractTableView::mouseReleaseEvent(event);
}


/**
 * @brief       This method has been reimplemented. It realigns the slider on real instructions except
 *              when the type is QAbstractSlider::SliderNoAction. This type (QAbstractSlider::SliderNoAction)
 *              is used to force the disassembling at a specific address.
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
int Disassembly::sliderMovedAction(int type, int value, int delta)
{
    int newValue;

    if(type != QAbstractSlider::SliderNoAction) // QAbstractSlider::SliderNoAction is used print the disassembly at a specifi address
    {
        if(type == QAbstractSlider::SliderMove) // If it's a slider action, disassemble one instruction back and one instruction next in order to be aligned on a real instruction
        {
            if(value + delta > 0)
            {
                newValue = getIndexFromCount(value + delta, -1);
                newValue = getIndexFromCount(newValue, 1);
            }
            else
                newValue = 0;
        }
        else    // For other action, disassebmle according to the delta
        {
            newValue = getIndexFromCount(value, delta);
        }
    }
    else
    {
        newValue = value + delta;
    }
    return newValue;
}


/**
 * @brief       This method has been reimplemented. It processes the Up/Down key events.
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void Disassembly::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    qDebug() << "keyPressEvent " << getLineToPrintcount();

    if(key == Qt::Key_Up || key == Qt::Key_Down)
    {
        int botRVA = getTableOffset();
        int topRVA = getIndexFromCount(getTableOffset(), getLineToPrintcount() - 1);

        if(key == Qt::Key_Up)
            selectPrevious();
        else
            selectNext();

        if(getInitialSelection() < botRVA)
        {
            forceScrollBarValue(getInitialSelection());
        }
        else if(getInitialSelection() >= topRVA)
        {
            forceScrollBarValue(getIndexFromCount(getInitialSelection(),-getLineToPrintcount() + 2));
        }

        viewport()->repaint();
    }
    else
    {
        AbstractTableView::keyPressEvent(event);
    }
}


/************************************************************************************
                            Graphic Dump
************************************************************************************/
/**
 * @brief       This method processes the Up/Down key events.
 *
 * @param[in]   painter     Pointer to the painter that allows painting by its own
 * @param[in]   x           Rectangle x
 * @param[in]   y           Rectangle y
 * @param[in]   addr        RVA of address to process
 *
 * @return      Nothing.
 */
void Disassembly::paintGraphicDump(QPainter* painter, int x, int y, int addr)
{
    ulong selHeadRVA = mSelection.fromIndex;
    ulong rva = addr;
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

        if(destRVA > (ulong)mMemPage->getBase())
        {
            destRVA -= (ulong)mMemPage->getBase();

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
        painter->drawLine(x, y, x, y + getRowHeight());
    }
    else if(wPict == GD_FootToBottom)
    {
        painter->drawLine(x, y + getRowHeight() / 2, x + 5, y + getRowHeight() / 2);
        painter->drawLine(x, y + getRowHeight() / 2, x, y + getRowHeight());
    }
    if(wPict == GD_FootToTop)
    {
        painter->drawLine(x, y + getRowHeight() / 2, x + 5, y + getRowHeight() / 2);
        painter->drawLine(x, y, x, y + getRowHeight() / 2);
    }
    else if(wPict == GD_HeadFromBottom)
    {
        QPoint wPoints[] = {
             QPoint(x + 3, y + getRowHeight() / 2 - 2),
             QPoint(x + 5, y + getRowHeight() / 2),
             QPoint(x + 3, y + getRowHeight() / 2 + 2),
         };

        painter->drawLine(x, y + getRowHeight() / 2, x + 5, y + getRowHeight() / 2);
        painter->drawLine(x, y + getRowHeight() / 2, x, y + getRowHeight());
        painter->drawPolyline(wPoints, 3);
    }
    if(wPict == GD_HeadFromTop)
    {
        QPoint wPoints[] = {
             QPoint(x + 3, y + getRowHeight() / 2 - 2),
             QPoint(x + 5, y + getRowHeight() / 2),
             QPoint(x + 3, y + getRowHeight() / 2 + 2),
         };

        painter->drawLine(x, y + getRowHeight() / 2, x + 5, y + getRowHeight() / 2);
        painter->drawLine(x, y, x, y + getRowHeight() / 2);
        painter->drawPolyline(wPoints, 3);
    }

    painter->restore();
}


/************************************************************************************
                            Instructions Management
 ***********************************************************************************/
/**
 * @brief       Returns the RVA of count-th instructions before the given instruction RVA.
 *
 * @param[in]   address     Instruction RVA
 * @param[in]   count       Instruction count
 *
 * @return      RVA of count-th instructions before the given instruction RVA.
 */
int Disassembly::getPreviousInstructionRVA(int rva, int count)
{
    int_t wBottomByteRealRVA = (int_t)rva - 16 * (count + 3);
    wBottomByteRealRVA = wBottomByteRealRVA < 0 ? 0 : wBottomByteRealRVA;

    int_t wVirtualRVA = (int_t)rva - wBottomByteRealRVA;

    int_t wMaxByteCountToRead = wVirtualRVA + 1 + 16;

    uint32 wNewCacheSize = wMaxByteCountToRead > 16 * (getViewableRowsCount() + 20) ? wMaxByteCountToRead : 16 * (getViewableRowsCount() + 20);

    //unsigned char* wVirtualBaseAddr = mMemoryView->getDataPtrForGui(wBottomByteRealRVA, wMaxByteCountToRead,  wNewCacheSize);
    byte_t* wVirtualBaseAddr = mMemPage->readFromCache(wBottomByteRealRVA, wMaxByteCountToRead, wNewCacheSize);

    uint_t addr = mDisasm->DisassembleBack((byte_t*)wVirtualBaseAddr, 0,  wMaxByteCountToRead, wVirtualRVA, count);

    addr += rva - wVirtualRVA;

    return addr;

}


/**
 * @brief       Returns the RVA of count-th instructions after the given instruction RVA.
 *
 * @param[in]   address     Instruction RVA
 * @param[in]   count       Instruction count
 *
 * @return      RVA of count-th instructions after the given instruction RVA.
 */
int Disassembly::getNextInstructionRVA(int rva, int count)
{
    int_t wVirtualRVA = 0;
    int_t wRemainingBytes = mMemPage->getSize() - rva;
    int_t wMaxByteCountToRead = 16 * (count + 1);
    wMaxByteCountToRead = wRemainingBytes > wMaxByteCountToRead ? wMaxByteCountToRead : wRemainingBytes;

    uint32 wNewCacheSize = wMaxByteCountToRead > 16 * (getViewableRowsCount() + 20) ? wMaxByteCountToRead : 16 * (getViewableRowsCount() + 20);

    //unsigned char* wVirtualBaseAddr = mMemoryView->getDataPtrForGui(rva, wMaxByteCountToRead,  wNewCacheSize);
    byte_t* wVirtualBaseAddr = mMemPage->readFromCache(rva, wMaxByteCountToRead, wNewCacheSize);

    uint_t addr = mDisasm->DisassembleNext((byte_t*)wVirtualBaseAddr, 0,  wMaxByteCountToRead, wVirtualRVA, count);
    addr += rva;

    return addr;
}


/**
 * @brief       Disassembles the instruction at the given RVA.
 *
 * @param[in]   rva     RVA of instruction to disassemble
 *
 * @return      Return the disassembled instruction.
 */
Instruction_t Disassembly::DisassembleAt(ulong rva)
{
    ulong base = mMemPage->getBase();
    int_t wMaxByteCountToRead = 16 * 2;
    uint32 wNewCacheSize = wMaxByteCountToRead > 16 * (getViewableRowsCount() + 20) ? wMaxByteCountToRead : 16 * (getViewableRowsCount() + 20);

    //unsigned char* wVirtualBaseAddr = mMemoryView->getDataPtrForGui(rva, wMaxByteCountToRead,  wNewCacheSize);
    byte_t* wVirtualBaseAddr = mMemPage->readFromCache(rva, wMaxByteCountToRead, wNewCacheSize);

    return mDisasm->DisassembleAt(wVirtualBaseAddr, wMaxByteCountToRead, 0, base, rva);
}


/**
 * @brief       Disassembles the instruction count instruction afterc the instruction at the given RVA.
 *              Count can be positive or negative.
 *
 * @param[in]   rva     RVA of reference instruction
 * @param[in]   count   Number of instruction
 *
 * @return      Return the disassembled instruction.
 */
Instruction_t Disassembly::DisassembleAt(ulong rva, ulong count)
{
    rva = getNextInstructionRVA(rva, count);
    return DisassembleAt(rva);
}


/************************************************************************************
                                Selection Management
************************************************************************************/
void Disassembly::expandSelectionUpTo(int to)
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


void Disassembly::setSingleSelection(int index)
{
    mSelection.firstSelectedIndex = index;
    mSelection.fromIndex = index;
    mSelection.toIndex = index;
}


int Disassembly::getInitialSelection()
{
    return mSelection.firstSelectedIndex;
}


void Disassembly::selectNext()
{
    int wAddr = getIndexFromCount(getInitialSelection(), 1);

    setSingleSelection(wAddr);
}


void Disassembly::selectPrevious()
{
    int wAddr = getIndexFromCount(getInitialSelection(), -1);

    setSingleSelection(wAddr);
}


bool Disassembly::isSelected(int base, int offset)
{
    int wAddr = base;

    if(offset < 0)
        wAddr = getPreviousInstructionRVA(getTableOffset(), offset);
    else if(offset > 0)
        wAddr = getNextInstructionRVA(getTableOffset(), offset);

    if(wAddr >= mSelection.fromIndex && wAddr <= mSelection.toIndex)
        return true;
    else
        return false;
}


/************************************************************************************
                        Index Management
************************************************************************************/
int Disassembly::getIndexFromCount(int index, int count)
{
    int wAddr;

    if(count == 0)
        wAddr = index;
    if(count < 0)
        wAddr = getPreviousInstructionRVA(index, qAbs(count));
    else if(count > 0)
        wAddr = getNextInstructionRVA(index, qAbs(count));


    if(wAddr < 0)
        wAddr = 0;
    else if(wAddr > getRowCount() - 1)
        wAddr = getRowCount() - 1;

    return wAddr;
}


int Disassembly::getLineToPrintcount()
{
    int wViewableRowsCount = getViewableRowsCount();

    int wAddrPrev = getTableOffset();
    int wAddr = wAddrPrev;

    int wCount = 0;

    for(int wI = 0; wI < wViewableRowsCount; wI++)
    {
        wAddrPrev = wAddr;
        wAddr = getNextInstructionRVA(wAddr, 1);

        if(wAddr == wAddrPrev)
        {
            break;
        }

        wCount++;
    }

    return wCount;
}


/************************************************************************************
                        Memory Page
************************************************************************************/
void Disassembly::setMemoryPage(MemoryPage* parMemPage)
{
    mMemPage = parMemPage;
}


void Disassembly::disassambleAt(uint_t parVA)
{
    uint_t wBase = Bridge::getBridge()->getBase(parVA);
    uint_t wSize = Bridge::getBridge()->getSize(wBase);
    uint_t wRVA = parVA - wBase;

    setSingleSelection(wRVA);               // Selects disassembled instruction
    mMemPage->resetCache();                 // Reset cache in order to update it
    mMemPage->setAttributes(wBase, wSize);  // Set base and size (Useful when memory page changed)
    forceScrollBarValue(wRVA);
    mCipRva = wRVA;
    setRowCount(wSize);
}