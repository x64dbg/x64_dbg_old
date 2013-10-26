#include "RegistersView.h"
#include "ui_RegistersView.h"

RegistersView::RegistersView(QWidget *parent) : QWidget(parent), ui(new Ui::RegistersView)
{
    ui->setupUi(this);

    QPalette wPalette;
    wPalette.setColor(QPalette::Window, QColor(255, 251, 240));

    this->setAutoFillBackground(true);
    this->setPalette(wPalette);

    mSelected = -1;

    mRegList = new QList<QWidget*>();
    mRegList->append(ui->RAXRegLabel);
    mRegList->append(ui->RCXRegLabel);
    mRegList->append(ui->RDXRegLabel);
    mRegList->append(ui->RBXRegLabel);
    mRegList->append(ui->RDIRegLabel);
    mRegList->append(ui->RBPRegLabel);
    mRegList->append(ui->RSIRegLabel);
    mRegList->append(ui->RSPRegLabel);
    mRegList->append(ui->R9RegLabel);
    mRegList->append(ui->RFLAGSRegLabel);
    mRegList->append(ui->R8RegLabel);
    mRegList->append(ui->R10RegLabel);
    mRegList->append(ui->RIPRegLabel);
    mRegList->append(ui->R12RegLabel);
    mRegList->append(ui->R13RegLabel);
    mRegList->append(ui->R11RegLabel);
    mRegList->append(ui->R14RegLabel);
    mRegList->append(ui->R15RegLabel);
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
    mRegList->append(ui->CFRegLabel);
    mRegList->append(ui->PFRegLabel);
    mRegList->append(ui->AFRegLabel);
    mRegList->append(ui->ZFRegLabel);
    mRegList->append(ui->IFRegLabel);
    mRegList->append(ui->DFRegLabel);
    mRegList->append(ui->SFRegLabel);
    mRegList->append(ui->TFRegLabel);
    mRegList->append(ui->OFRegLabel);

    ui->RAXRegLabel->setAutoFillBackground(true);
    ui->RCXRegLabel->setAutoFillBackground(true);
    ui->RDXRegLabel->setAutoFillBackground(true);
    ui->RBXRegLabel->setAutoFillBackground(true);
    ui->RDIRegLabel->setAutoFillBackground(true);
    ui->RBPRegLabel->setAutoFillBackground(true);
    ui->RSIRegLabel->setAutoFillBackground(true);
    ui->RSPRegLabel->setAutoFillBackground(true);
    ui->R9RegLabel->setAutoFillBackground(true);
    ui->RFLAGSRegLabel->setAutoFillBackground(true);
    ui->R8RegLabel->setAutoFillBackground(true);
    ui->R10RegLabel->setAutoFillBackground(true);
    ui->RIPRegLabel->setAutoFillBackground(true);
    ui->R12RegLabel->setAutoFillBackground(true);
    ui->R13RegLabel->setAutoFillBackground(true);
    ui->R11RegLabel->setAutoFillBackground(true);
    ui->R14RegLabel->setAutoFillBackground(true);
    ui->R15RegLabel->setAutoFillBackground(true);
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
    ui->CFRegLabel->setAutoFillBackground(true);
    ui->PFRegLabel->setAutoFillBackground(true);
    ui->AFRegLabel->setAutoFillBackground(true);
    ui->ZFRegLabel->setAutoFillBackground(true);
    ui->IFRegLabel->setAutoFillBackground(true);
    ui->DFRegLabel->setAutoFillBackground(true);
    ui->SFRegLabel->setAutoFillBackground(true);
    ui->TFRegLabel->setAutoFillBackground(true);
    ui->OFRegLabel->setAutoFillBackground(true);
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
