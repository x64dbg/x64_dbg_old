#ifndef REGISTERSVIEW_H
#define REGISTERSVIEW_H

#include <QtGui>

namespace Ui {
class RegistersView;
}

class RegistersView : public QWidget
{
    Q_OBJECT
    
public:
    explicit RegistersView(QWidget *parent = 0);
    ~RegistersView();
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    
private:
    Ui::RegistersView *ui;
    QList<QWidget*>* mRegList;
    int mSelected;
};

#endif // REGISTERSVIEW_H
