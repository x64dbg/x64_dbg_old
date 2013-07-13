#include "Disassembly.h"

Disassembly::Disassembly(QWidget *parent) :AbstractTableView(parent)
{
    mSelection = (SelectionData_t){0, 0, 0};

    mMemoryView = new MapViewOfMem("AsmCode.bin");
    mDisasm = new QBeaEngine();

    mIsLastInstDisplayed = false;

    setRowCount(mMemoryView->size());

    qDebug() << "size" << mMemoryView->size();
}


void Disassembly::AbstractMemberFunction(void)
{
    qDebug() << "Disassembly: AbstractMemberFunction";
}

/*
void Disassembly::sliderMovedAction(int type, int value, int delta)
{
    qDebug() << "Disassembly: sliderMovedAction: value " << value << " delta " << delta;

    int rva = value;

    if(type != QAbstractSlider::SliderMove)
    {
        if(delta < 0)
        {
            rva = getPreviousInstructionRVA(rva, qAbs(delta));
        }
        else
        {
            rva = getNextInstructionRVA(rva, qAbs(delta));
        }

        setScrollBarValue(rva);
    }
    else
    {
        if(delta < 0)
        {
            rva = getPreviousInstructionRVA(rva + delta, 1);
        }
        else
        {
            rva = getNextInstructionRVA(rva + delta, 1);
        }

        setTableOffset(rva);
        getScrollbar()->setValue(rva);
    }
}
*/

QString Disassembly::getStringToPrint(int rowBase, int rowOffset, int col)
{
   // qDebug() << "Disassembly : getStringToPrint : row : " << row;
    QString string = "";
    int wRva = getIndexFromCount(rowBase, rowOffset);

    qDebug() << "getStringToPrint rowBase:" << rowBase << "rowOffset:" << rowOffset << "col:" << col << "wRva:" << wRva;

    if(wRva < mMemoryView->size())
    {
        Instruction_t instruction = DisassembleAt(wRva);
        qDebug() << "instruction rva:" << instruction.rva << "lentgh:" << instruction.lentgh;
        qDebug() << "mIsLastInstDisplayed:" << mIsLastInstDisplayed;

        if(rowOffset == 0)// && (int)instruction.rva + (int)instruction.lentgh - 1 < getRowCount() - 1)
            mIsLastInstDisplayed = false;

        if(mIsLastInstDisplayed == false)
        {
            switch(col)
            {
                case 0:
                {
                    ulong addr = (ulong)instruction.rva + (ulong)mMemoryView->getBase();
                    //string = QString("%1").arg(addr, 8, 16, QChar('0')).toUpper();
                    string += QString::number(instruction.rva);
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

            if((int)instruction.rva + (int)instruction.lentgh - 1 >= getRowCount() - 1)
                mIsLastInstDisplayed = true;
        }
    }

    return string;
}


bool Disassembly::paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h)
{

    if(col == 1)
    {
        int wAddr = getIndexFromCount(rowBase, rowOffset);

        paintGraphicDump(painter, x + 5, y, wAddr);

        // Draw cell content
        painter->drawText(QRect(x + 15, y, getColumnWidth(col) - 15, getRowHeight()), 0, getStringToPrint(rowBase, rowOffset, col));

        return true;
    }
    else
        return false;
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
{/*
    int wRowDelta = to - getTableOffset();
    int wAddr = to;

    if(wRowDelta < 0)
        wAddr = getPreviousInstructionRVA(getTableOffset(), wRowDelta);
    else if(wRowDelta > 0)
        wAddr = getNextInstructionRVA(getTableOffset(), wRowDelta);
*/
    //wAddr = to;
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
{/*
    int wRowDelta = index - getTableOffset();
    int wAddr = index;

    if(wRowDelta < 0)
        wAddr = getPreviousInstructionRVA(getTableOffset(), wRowDelta);
    else if(wRowDelta > 0)
        wAddr = getNextInstructionRVA(getTableOffset(), wRowDelta);
*/
    mSelection.firstSelectedIndex = index;
    mSelection.fromIndex = index;
    mSelection.toIndex = index;
}


int Disassembly::getInitialSelection()
{
    return mSelection.firstSelectedIndex;
}


bool Disassembly::isSelected(int index)
{
    int wRowDelta = index - getTableOffset();
    int wAddr = index;

    if(wRowDelta < 0)
        wAddr = getPreviousInstructionRVA(getTableOffset(), wRowDelta);
    else if(wRowDelta > 0)
        wAddr = getNextInstructionRVA(getTableOffset(), wRowDelta);

    if(wAddr >= mSelection.fromIndex && wAddr <= mSelection.toIndex)
        return true;
    else
        return false;
}

void Disassembly::selectNext()
{
    int wAddr = getNextInstructionRVA(mSelection.firstSelectedIndex, 1);

    if(wAddr > getRowCount() - 1)
        wAddr = getRowCount() - 1;

    mSelection.firstSelectedIndex = wAddr;
    mSelection.fromIndex = wAddr;
    mSelection.toIndex = wAddr;
}


void Disassembly::selectPrevious()
{
    int wAddr = getPreviousInstructionRVA(mSelection.firstSelectedIndex, 1);

    if(wAddr < 0)
        wAddr = 0;

    mSelection.firstSelectedIndex = wAddr;
    mSelection.fromIndex = wAddr;
    mSelection.toIndex = wAddr;
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


/*
QString Disassembly::getStringToPrint(int topTableAddress, int rowIndex, int colIndex) //getStringToPrint(int row, int col)
{
    ulong ip = getNextInstructionRVA(topTableAddress, rowIndex);
    QString string = "";

    if(ip < mMemoryView->size())
    {
        Instruction_t instruction = DisassembleAt(ip);

        switch(colIndex)
        {
            case 0:
            {
                ulong addr = (ulong)instruction.rva + (ulong)mMemoryView->getBase();
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


void Disassembly::paintGraphicDump(QPainter* painter, int x, int y, int topTableRVA, int rowIndex)
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
*/







































