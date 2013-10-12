#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QtGui>
#include <QDebug>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent* event);
    
signals:
    void keyPressed(int parKey);
    
public slots:
    
};

#endif // LINEEDIT_H
