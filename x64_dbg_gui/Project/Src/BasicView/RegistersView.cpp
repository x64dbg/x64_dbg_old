#include "RegistersView.h"
#ifdef _WIN64
#include "ui_RegistersView64.h"
#else
#include "ui_RegistersView32.h"
#endif


RegistersView::RegistersView(QWidget *parent) : QWidget(parent), ui(new Ui::RegistersView)
{
    int wI = 0;
    ui->setupUi(this);

    // Set background color
    QPalette wPalette;
    wPalette.setColor(QPalette::Window, QColor(255, 251, 240));
    this->setAutoFillBackground(true);
    this->setPalette(wPalette);

    // Context Menu
    this->setContextMenuPolicy(Qt::CustomContextMenu);



    mSelected = -1;

    mRegList = new QList<QWidget*>();
    mRegNamesList = new QList<REGISTER_NAME>();

    mRegList->append(ui->AXRegLabel);
    mRegNamesList->append(CAX);
    mRegList->append(ui->CXRegLabel);
    mRegNamesList->append(CCX);
    mRegList->append(ui->DXRegLabel);
    mRegNamesList->append(CDX);
    mRegList->append(ui->BXRegLabel);
    mRegNamesList->append(CBX);
    mRegList->append(ui->DIRegLabel);
    mRegNamesList->append(CDI);
    mRegList->append(ui->BPRegLabel);
    mRegNamesList->append(CBP);
    mRegList->append(ui->SIRegLabel);
    mRegNamesList->append(CSI);
    mRegList->append(ui->SPRegLabel);
    mRegNamesList->append(CSP);

#ifdef _WIN64
    mRegList->append(ui->R8RegLabel);
    mRegNamesList->append(R8);
    mRegList->append(ui->R9RegLabel);
    mRegNamesList->append(R9);
    mRegList->append(ui->R10RegLabel);
    mRegNamesList->append(R10);
    mRegList->append(ui->R11RegLabel);
    mRegNamesList->append(R11);
    mRegList->append(ui->R12RegLabel);
    mRegNamesList->append(R12);
    mRegList->append(ui->R13RegLabel);
    mRegNamesList->append(R13);
    mRegList->append(ui->R14RegLabel);
    mRegNamesList->append(R14);
    mRegList->append(ui->R15RegLabel);
    mRegNamesList->append(R15);
#endif

    mRegList->append(ui->IPRegLabel);
    mRegNamesList->append(CIP);

    mRegList->append(ui->FLAGSRegLabel);
    mRegNamesList->append(CFLAGS);

    mRegList->append(ui->CFRegLabel);
    mRegNamesList->append(CF);
    mRegList->append(ui->PFRegLabel);
    mRegNamesList->append(PF);
    mRegList->append(ui->AFRegLabel);
    mRegNamesList->append(AF);
    mRegList->append(ui->ZFRegLabel);
    mRegNamesList->append(ZF);
    mRegList->append(ui->SFRegLabel);
    mRegNamesList->append(SF);
    mRegList->append(ui->TFRegLabel);
    mRegNamesList->append(TF);
    mRegList->append(ui->IFRegLabel);
    mRegNamesList->append(IF);
    mRegList->append(ui->DFRegLabel);
    mRegNamesList->append(DF);
    mRegList->append(ui->OFRegLabel);
    mRegNamesList->append(OF);

    mRegList->append(ui->GSRegLabel);
    mRegNamesList->append(GS);
    mRegList->append(ui->FSRegLabel);
    mRegNamesList->append(FS);
    mRegList->append(ui->ESRegLabel);
    mRegNamesList->append(ES);
    mRegList->append(ui->DSRegLabel);
    mRegNamesList->append(DS);
    mRegList->append(ui->CSRegLabel);
    mRegNamesList->append(CS);
    mRegList->append(ui->SSRegLabel);
    mRegNamesList->append(SS);

    mRegList->append(ui->DR0RegLabel);
    mRegNamesList->append(DR0);
    mRegList->append(ui->DR1RegLabel);
    mRegNamesList->append(DR1);
    mRegList->append(ui->DR2RegLabel);
    mRegNamesList->append(DR2);
    mRegList->append(ui->DR5RegLabel);
    mRegNamesList->append(DR5);
    mRegList->append(ui->DR6RegLabel);
    mRegNamesList->append(DR6);
    mRegList->append(ui->DR7RegLabel);
    mRegNamesList->append(DR7);


    for(wI = 0; wI < mRegList->size(); wI++)
    {
        QFont wFont(mRegList->at(wI)->font());
        wFont.setFixedPitch(true);

        mRegList->at(wI)->setAutoFillBackground(true);

        mRegList->at(wI)->setFont(wFont);
    }


    connect(Bridge::getBridge(), SIGNAL(updateRegisters()), this, SLOT(updateRegistersSlot()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayCustomContextMenuSlot(QPoint)));
}

RegistersView::~RegistersView()
{
    delete ui;
}

void RegistersView::mousePressEvent(QMouseEvent* event)
{
    int wI = 0;
    int wSelected = -1;
    QPalette wPalette;


    for(wI = 0; wI < mRegList->size(); wI++)
    {
        if(mRegList->at(wI)->geometry().contains(event->pos()))
        {
            wSelected = wI;
        }
    }


    if(wSelected != mSelected)
    {
        // Unselect previous register
        if(mSelected != -1)
        {
            wPalette.setColor(QPalette::Window, QColor(192,192,192,0));
            mRegList->at(mSelected)->setPalette(wPalette);
        }

        // Select new register
        mSelected = wSelected;
        if(mSelected != -1)
        {
            wPalette.setColor(QPalette::Window, QColor(192,192,192,255));
            mRegList->at(mSelected)->setPalette(wPalette);
        }
    }
}



void RegistersView::mouseDoubleClickEvent(QMouseEvent* event)
{
    int wI = 0;
    int wSelected = -1;
    QPalette wPalette;

    for(wI = 0; wI < mRegList->size(); wI++)
    {
        if(mRegList->at(wI)->geometry().contains(event->pos()))
        {
            wSelected = wI;
        }
    }
    if(wSelected == -1) //no register selection
        return;

    //double click test code, toggle enabled
    if(mRegList->at(wSelected)->isEnabled())
        mRegList->at(wSelected)->setEnabled(false);
    else
        mRegList->at(wSelected)->setEnabled(true);
}


void RegistersView::updateRegistersSlot()
{
    REGDUMP wRegDumpStruct;
    memset(&wRegDumpStruct, 0, sizeof(REGDUMP));

    Bridge::getBridge()->getRegDumpFromDbg(&wRegDumpStruct);

    ui->AXRegLabel->setText(QString("%1").arg(wRegDumpStruct.cax, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->CXRegLabel->setText(QString("%1").arg(wRegDumpStruct.ccx, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DXRegLabel->setText(QString("%1").arg(wRegDumpStruct.cdx, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->BXRegLabel->setText(QString("%1").arg(wRegDumpStruct.cbx, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DIRegLabel->setText(QString("%1").arg(wRegDumpStruct.cdi, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->BPRegLabel->setText(QString("%1").arg(wRegDumpStruct.cbp, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->SIRegLabel->setText(QString("%1").arg(wRegDumpStruct.csi, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->SPRegLabel->setText(QString("%1").arg(wRegDumpStruct.csp, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());

#ifdef _WIN64
    ui->R8RegLabel->setText(QString("%1").arg(wRegDumpStruct.r8, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R9RegLabel->setText(QString("%1").arg(wRegDumpStruct.r9, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R10RegLabel->setText(QString("%1").arg(wRegDumpStruct.r10, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R11RegLabel->setText(QString("%1").arg(wRegDumpStruct.r11, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R12RegLabel->setText(QString("%1").arg(wRegDumpStruct.r12, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R13RegLabel->setText(QString("%1").arg(wRegDumpStruct.r13, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R14RegLabel->setText(QString("%1").arg(wRegDumpStruct.r14, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->R15RegLabel->setText(QString("%1").arg(wRegDumpStruct.r15, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
#endif

    ui->IPRegLabel->setText(QString("%1").arg(wRegDumpStruct.cip, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());

    ui->FLAGSRegLabel->setText(QString("%1").arg(wRegDumpStruct.cflags, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->CFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.c, 1, 16, QChar('0')).toUpper());
    ui->PFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.p, 1, 16, QChar('0')).toUpper());
    ui->AFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.a, 1, 16, QChar('0')).toUpper());
    ui->ZFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.z, 1, 16, QChar('0')).toUpper());
    ui->SFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.s, 1, 16, QChar('0')).toUpper());
    ui->TFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.t, 1, 16, QChar('0')).toUpper());
    ui->IFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.i, 1, 16, QChar('0')).toUpper());
    ui->DFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.d, 1, 16, QChar('0')).toUpper());
    ui->OFRegLabel->setText(QString("%1").arg(wRegDumpStruct.flags.o, 1, 16, QChar('0')).toUpper());

    ui->GSRegLabel->setText(QString("%1").arg(wRegDumpStruct.gs, 4, 16, QChar('0')).toUpper());
    ui->FSRegLabel->setText(QString("%1").arg(wRegDumpStruct.fs, 4, 16, QChar('0')).toUpper());
    ui->ESRegLabel->setText(QString("%1").arg(wRegDumpStruct.es, 4, 16, QChar('0')).toUpper());
    ui->DSRegLabel->setText(QString("%1").arg(wRegDumpStruct.ds, 4, 16, QChar('0')).toUpper());
    ui->CSRegLabel->setText(QString("%1").arg(wRegDumpStruct.cs, 4, 16, QChar('0')).toUpper());
    ui->SSRegLabel->setText(QString("%1").arg(wRegDumpStruct.ss, 4, 16, QChar('0')).toUpper());

    ui->DR0RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr0, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR1RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr1, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR2RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr2, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR5RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr5, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR6RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr6, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR7RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr7, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
}




void RegistersView::displayCustomContextMenuSlot(QPoint pos)
{
    QMenu wMenu(this);

    if(mSelected != -1)
    {
        if(     mRegNamesList->at(mSelected) == CAX ||
                mRegNamesList->at(mSelected) == CCX ||
                mRegNamesList->at(mSelected) == CDX ||
                mRegNamesList->at(mSelected) == CBX ||
                mRegNamesList->at(mSelected) == CDI ||
                mRegNamesList->at(mSelected) == CBP ||
                mRegNamesList->at(mSelected) == CSI ||
                mRegNamesList->at(mSelected) == CSP ||

                mRegNamesList->at(mSelected) == R8 ||
                mRegNamesList->at(mSelected) == R9 ||
                mRegNamesList->at(mSelected) == R10 ||
                mRegNamesList->at(mSelected) == R11 ||
                mRegNamesList->at(mSelected) == R12 ||
                mRegNamesList->at(mSelected) == R13 ||
                mRegNamesList->at(mSelected) == R14 ||
                mRegNamesList->at(mSelected) == R15 ||

                mRegNamesList->at(mSelected) == CIP ||

                mRegNamesList->at(mSelected) == CFLAGS)
        {
            QAction* wEdit = wMenu.addAction("Edit");
            QAction* wSetTo0 = wMenu.addAction("Set to 0");
            QAction* wSetTo1 = wMenu.addAction("Set to 1");
            QAction* wAction = wMenu.exec(this->mapToGlobal(pos));

            if(wAction == wEdit)
            {
                 WordEditDialog wEditDial;
                 if(QDialog::Accepted == wEditDial.exec())
                 {

                 }

            }
            else if(wAction == wSetTo0)
            {
                setRegister(mRegNamesList->at(mSelected), 0);
            }
            else if(wAction == wSetTo1)
            {
                setRegister(mRegNamesList->at(mSelected), 1);
            }
        }
        else if(mRegNamesList->at(mSelected) == CF ||
                mRegNamesList->at(mSelected) == PF ||
                mRegNamesList->at(mSelected) == AF ||
                mRegNamesList->at(mSelected) == ZF ||
                mRegNamesList->at(mSelected) == SF ||
                mRegNamesList->at(mSelected) == TF ||
                mRegNamesList->at(mSelected) == IF ||
                mRegNamesList->at(mSelected) == DF ||
                mRegNamesList->at(mSelected) == OF)
        {
            QAction* wSetTo0 = wMenu.addAction("Set to 0");
            QAction* wSetTo1 = wMenu.addAction("Set to 1");
            QAction* wAction = wMenu.exec(this->mapToGlobal(pos));

            if(wAction == wSetTo0)
            {
                setRegister(mRegNamesList->at(mSelected), 0);
            }
            else if(wAction == wSetTo1)
            {
                setRegister(mRegNamesList->at(mSelected), 1);
            }
        }
    }
}

void RegistersView::setRegister(REGISTER_NAME reg, uint_t value)
{
    QString wCmd = "";

    // Basic registers
    if(reg == CAX)
    {
        #ifdef _WIN64
            wCmd += "rax=";
        #else
            wCmd += "eax=";
        #endif
    }
    else if(reg == CCX)
    {
        #ifdef _WIN64
            wCmd += "rcx=";
        #else
            wCmd += "ecx=";
        #endif
    }
    else if(reg == CDX)
    {
        #ifdef _WIN64
            wCmd += "rdx=";
        #else
            wCmd += "edx=";
        #endif
    }
    else if(reg == CBX)
    {
        #ifdef _WIN64
            wCmd += "rbx=";
        #else
            wCmd += "ebx=";
        #endif
    }
    else if(reg == CDI)
    {
        #ifdef _WIN64
            wCmd += "rdi=";
        #else
            wCmd += "edi=";
        #endif
    }
    else if(reg == CBP)
    {
        #ifdef _WIN64
            wCmd += "rbp=";
        #else
            wCmd += "ebp=";
        #endif
    }
    else if(reg == CSI)
    {
        #ifdef _WIN64
            wCmd += "rsi=";
        #else
            wCmd += "esi=";
        #endif
    }
    else if(reg == CSP)
    {
        #ifdef _WIN64
            wCmd += "rsp=";
        #else
            wCmd += "esp=";
        #endif
    }

    // General purpose register
    else if(reg == R8)
    {
        wCmd += "r8=";
    }
    else if(reg == R9)
    {
        wCmd += "r9=";
    }
    else if(reg == R10)
    {
        wCmd += "r10=";
    }
    else if(reg == R11)
    {
        wCmd += "r11=";
    }
    else if(reg == R12)
    {
        wCmd += "r12=";
    }
    else if(reg == R13)
    {
        wCmd += "r13=";
    }
    else if(reg == R14)
    {
        wCmd += "r14=";
    }
    else if(reg == R15)
    {
        wCmd += "r15=";
    }

    // Instruction pointer register
    else if(reg == CIP)
    {
        #ifdef _WIN64
            wCmd += "rip=";
        #else
            wCmd += "eip=";
        #endif
    }

    // Flags
    else if(reg == CFLAGS)
    {
        #ifdef _WIN64
            wCmd += "rflags=";
        #else
            wCmd += "eflags=";
        #endif
    }
    else if(reg == CF)
    {
        wCmd += "!cf=";
    }
    else if(reg == PF)
    {
        wCmd += "!pf=";
    }
    else if(reg == AF)
    {
        wCmd += "!af=";
    }
    else if(reg == ZF)
    {
        wCmd += "!zf=";
    }
    else if(reg == SF)
    {
        wCmd += "!sf=";
    }
    else if(reg == TF)
    {
        wCmd += "!tf=";
    }
    else if(reg == IF)
    {
        wCmd += "!if=";
    }
    else if(reg == DF)
    {
        wCmd += "!df=";
    }
    else if(reg == OF)
    {
        wCmd += "!of=";
    }

    if(wCmd.isEmpty() == false)
    {
        wCmd += QString::number(value);
        Bridge::getBridge()->execCmd(reinterpret_cast<const char*>(wCmd.toAscii().data()));
    }
}
