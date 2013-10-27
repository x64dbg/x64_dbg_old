#include "RegistersView.h"
#ifdef _WIN64
#include "ui_RegistersView64.h"
#else
#include "ui_RegistersView32.h"
#endif


RegistersView::RegistersView(QWidget *parent) : QWidget(parent), ui(new Ui::RegistersView)
{
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

    mRegList->append(ui->DR2RegLabel);
    mRegList->append(ui->DR0RegLabel);
    mRegList->append(ui->DR7RegLabel);
    mRegList->append(ui->DR1RegLabel);
    mRegList->append(ui->DR6RegLabel);
    mRegList->append(ui->DR5RegLabel);


    ui->AXRegLabel->setAutoFillBackground(true);
    ui->CXRegLabel->setAutoFillBackground(true);
    ui->DXRegLabel->setAutoFillBackground(true);
    ui->BXRegLabel->setAutoFillBackground(true);
    ui->DIRegLabel->setAutoFillBackground(true);
    ui->BPRegLabel->setAutoFillBackground(true);
    ui->SIRegLabel->setAutoFillBackground(true);
    ui->SPRegLabel->setAutoFillBackground(true);
#ifdef _WIN64
    ui->R8RegLabel->setAutoFillBackground(true);
    ui->R9RegLabel->setAutoFillBackground(true);
    ui->R10RegLabel->setAutoFillBackground(true);
    ui->R11RegLabel->setAutoFillBackground(true);
    ui->R12RegLabel->setAutoFillBackground(true);
    ui->R13RegLabel->setAutoFillBackground(true);
    ui->R14RegLabel->setAutoFillBackground(true);
    ui->R15RegLabel->setAutoFillBackground(true);
#endif
    ui->IPRegLabel->setAutoFillBackground(true);

    ui->FLAGSRegLabel->setAutoFillBackground(true);
    ui->CFRegLabel->setAutoFillBackground(true);
    ui->PFRegLabel->setAutoFillBackground(true);
    ui->AFRegLabel->setAutoFillBackground(true);
    ui->ZFRegLabel->setAutoFillBackground(true);
    ui->SFRegLabel->setAutoFillBackground(true);
    ui->TFRegLabel->setAutoFillBackground(true);
    ui->IFRegLabel->setAutoFillBackground(true);
    ui->DFRegLabel->setAutoFillBackground(true);
    ui->OFRegLabel->setAutoFillBackground(true);

    ui->GSRegLabel->setAutoFillBackground(true);
    ui->FSRegLabel->setAutoFillBackground(true);
    ui->ESRegLabel->setAutoFillBackground(true);
    ui->DSRegLabel->setAutoFillBackground(true);
    ui->CSRegLabel->setAutoFillBackground(true);
    ui->SSRegLabel->setAutoFillBackground(true);

    ui->DR2RegLabel->setAutoFillBackground(true);
    ui->DR0RegLabel->setAutoFillBackground(true);
    ui->DR7RegLabel->setAutoFillBackground(true);
    ui->DR1RegLabel->setAutoFillBackground(true);
    ui->DR6RegLabel->setAutoFillBackground(true);
    ui->DR5RegLabel->setAutoFillBackground(true);

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
