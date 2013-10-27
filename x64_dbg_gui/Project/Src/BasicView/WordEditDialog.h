#ifndef WORDEDITDIALOG_H
#define WORDEDITDIALOG_H

#include <QtGui>
#include "Bridge.h"

namespace Ui {
class WordEditDialog;
}

class WordEditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WordEditDialog(QWidget *parent = 0);
    ~WordEditDialog();

public slots:
    void expressionChanged(QString s);
    void hexaChanged(QString s);
    void signedChanged(QString s);
    void unsignedChanged(QString s);
    
private:
    Ui::WordEditDialog *ui;
    uint_t mWord;
};

#endif // WORDEDITDIALOG_H
