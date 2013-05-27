#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include "ColumnWidget.h"

namespace Ui {
class CPUWidget;
}

class CPUWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CPUWidget(QWidget *parent = 0);
    ~CPUWidget();
    void setDefaultDisposition(void);
    
private:
    Ui::CPUWidget *ui;
    ColumnWidget* cw;
};

#endif // CPUWIDGET_H
