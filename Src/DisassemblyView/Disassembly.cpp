#include "Disassembly.h"

Disassembly::Disassembly(QWidget *parent) :AbstractTableView(parent)
{
    mSelection = (SelectionData_t){0, 0, 0};

    mMemoryView = new MapViewOfMem("AsmCode.bin");
    mDisasm = new QBeaEngine();

    mIsLastInstDisplayed = false;

    mGuiState = Disassembly::NoState;

    setRowCount(mMemoryView->size());

    qDebug() << "size" << mMemoryView->size();
}



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


int Disassembly::sliderMovedAction(int type, int value, int delta)
{
    int newValue;

    if(type == QAbstractSlider::SliderMove)
    {
        if(value + delta > 0)
        {
            newValue = getIndexFromCount(value + delta, -1);
            newValue = getIndexFromCount(newValue, 1);
        }
        else
            newValue = 0;
    }
    else
    {
        newValue = getIndexFromCount(value, delta);
    }

    return newValue;
}

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
            verticalScrollBar()->setValue(getInitialSelection());
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
        else if(getInitialSelection() >= topRVA)
        {
            verticalScrollBar()->setValue(getIndexFromCount(getInitialSelection(),-getLineToPrintcount() + 2));
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }

        viewport()->repaint();
    }
    else
    {
        AbstractTableView::keyPressEvent(event);
    }
}


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
            wStr += QString::number(wInst.rva);
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

        if(destRVA > (ulong)mMemoryView->getBase())
        {
            destRVA -= (ulong)mMemoryView->getBase();

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

int Disassembly::getPreviousInstructionRVA(int address, int count)
{
    ulong addr = mDisasm->DisassembleBack((char*)mMemoryView->data(), (ulong)mMemoryView->data(), (ulong)mMemoryView->size(), (ulong)(mMemoryView->data() + address), count);
    return addr - (ulong)mMemoryView->data();

    //return (count > address ? 0 : address - count);
}

int Disassembly::getNextInstructionRVA(int address, int count)
{
    ulong addr = mDisasm->DisassembleNext((char*)mMemoryView->data(),(ulong)mMemoryView->data(), (ulong)mMemoryView->size(), (ulong)(mMemoryView->data() + address), count);
    return addr - (ulong)mMemoryView->data();

    //return address + count;
}

Instruction_t Disassembly::DisassembleAt(ulong rva)
{
    char* data = (char*)mMemoryView->data();
    ulong base = mMemoryView->getBase();
    ulong size = mMemoryView->size();
    ulong ip = base + rva;

    return mDisasm->DisassembleAt(data, base, size, ip);
}

Instruction_t Disassembly::DisassembleAt(ulong rva, ulong count)
{
    rva = getNextInstructionRVA(rva, count);

    char* data = (char*)mMemoryView->data();
    ulong base = mMemoryView->getBase();
    ulong size = mMemoryView->size();
    ulong ip = base + rva;

    return mDisasm->DisassembleAt(data, base, size, ip);
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
