#include "WordEditDialog.h"
#include "ui_WordEditDialog.h"

WordEditDialog::WordEditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::WordEditDialog)
{
    ui->setupUi(this);
    setModal(true);

    mWord = 0;

#ifdef _WIN64
    ui->hexLineEdit->setInputMask("hh hh hh hh hh hh hh hh");
#else
    ui->hexLineEdit->setInputMask("hh hh hh hh");
#endif

    connect(ui->expressionLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(expressionChanged(QString)));
    connect(ui->hexLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(hexaChanged(QString)));
    connect(ui->signedLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(signedChanged(QString)));
    connect(ui->unsignedLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(unsignedChanged(QString)));
}

WordEditDialog::~WordEditDialog()
{
    delete ui;
}

void WordEditDialog::expressionChanged(QString s)
{
    bool wIsValid = Bridge::getBridge()->isValidExpression(reinterpret_cast<const char*>(s.toAscii().data()));

    if(wIsValid == true)
    {
        ui->expressionLineEdit->setStyleSheet("");
        ui->buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
        mWord = Bridge::getBridge()->getValFromString(reinterpret_cast<const char*>(s.toAscii().data()));
        ui->hexLineEdit->setText(QString("%1").arg(mWord, sizeof(uint_t) * 2, 16, QChar('0')).toUpper());
        ui->signedLineEdit->setText(QString::number((int_t)mWord));
        ui->unsignedLineEdit->setText(QString::number((uint_t)mWord));
    }
    else
    {
        ui->expressionLineEdit->setStyleSheet("border: 1px solid red");
        ui->buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void WordEditDialog::hexaChanged(QString s)
{

}

void WordEditDialog::signedChanged(QString s)
{

}

void WordEditDialog::unsignedChanged(QString s)
{

}
