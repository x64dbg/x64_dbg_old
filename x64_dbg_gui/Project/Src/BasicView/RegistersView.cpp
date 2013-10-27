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

    QPalette wPalette;
    wPalette.setColor(QPalette::Window, QColor(255, 251, 240));

    this->setAutoFillBackground(true);
    this->setPalette(wPalette);

    mSelected = -1;

    mRegList = new QList<QWidget*>();

    mRegList->append(ui->AXRegLabel);
    mRegList->append(ui->CXRegLabel);
    mRegList->append(ui->DXRegLabel);
    mRegList->append(ui->BXRegLabel);
    mRegList->append(ui->DIRegLabel);
    mRegList->append(ui->BPRegLabel);
    mRegList->append(ui->SIRegLabel);
    mRegList->append(ui->SPRegLabel);

#ifdef _WIN64
    mRegList->append(ui->R8RegLabel);
    mRegList->append(ui->R9RegLabel);
    mRegList->append(ui->R10RegLabel);
    mRegList->append(ui->R11RegLabel);
    mRegList->append(ui->R12RegLabel);
    mRegList->append(ui->R13RegLabel);
    mRegList->append(ui->R14RegLabel);
    mRegList->append(ui->R15RegLabel);
#endif

    mRegList->append(ui->IPRegLabel);

    mRegList->append(ui->FLAGSRegLabel);

    mRegList->append(ui->CFRegLabel);
    mRegList->append(ui->PFRegLabel);
    mRegList->append(ui->AFRegLabel);
    mRegList->append(ui->ZFRegLabel);
    mRegList->append(ui->SFRegLabel);
    mRegList->append(ui->TFRegLabel);
    mRegList->append(ui->IFRegLabel);
    mRegList->append(ui->DFRegLabel);
    mRegList->append(ui->OFRegLabel);

    mRegList->append(ui->GSRegLabel);
    mRegList->append(ui->FSRegLabel);
    mRegList->append(ui->ESRegLabel);
    mRegList->append(ui->DSRegLabel);
    mRegList->append(ui->CSRegLabel);
    mRegList->append(ui->SSRegLabel);

    mRegList->append(ui->DR0RegLabel);
    mRegList->append(ui->DR1RegLabel);
    mRegList->append(ui->DR2RegLabel);
    mRegList->append(ui->DR5RegLabel);
    mRegList->append(ui->DR6RegLabel);
    mRegList->append(ui->DR7RegLabel);


    for(wI = 0; wI < mRegList->size(); wI++)
    {
        QFont wFont(mRegList->at(wI)->font());
        wFont.setFixedPitch(true);

        mRegList->at(wI)->setAutoFillBackground(true);

        mRegList->at(wI)->setFont(wFont);
    }


    connect(Bridge::getBridge(), SIGNAL(updateRegisters()), this, SLOT(updateRegistersSlot()));
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
    ui->ESRegLabel->setText(QString("%1").arg(wRegDumpStruct.es, 4, QChar('0')).toUpper());
    ui->DSRegLabel->setText(QString("%1").arg(wRegDumpStruct.ds, 4, QChar('0')).toUpper());
    ui->CSRegLabel->setText(QString("%1").arg(wRegDumpStruct.cs, 4, QChar('0')).toUpper());
    ui->SSRegLabel->setText(QString("%1").arg(wRegDumpStruct.ss, 4, 16, QChar('0')).toUpper());

    ui->DR0RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr0, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR1RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr1, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR2RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr2, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR5RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr5, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR6RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr6, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
    ui->DR7RegLabel->setText(QString("%1").arg(wRegDumpStruct.dr7, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
}
