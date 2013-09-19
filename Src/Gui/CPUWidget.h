#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QtGui>


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
    QVBoxLayout* getTopLeftWidget(void);
    QVBoxLayout* getTopRightWidget(void);
    QVBoxLayout* getBotLeftWidget(void);
    QVBoxLayout* getBotRightWidget(void);

private:
    Ui::CPUWidget *ui;

};

#endif // CPUWIDGET_H
