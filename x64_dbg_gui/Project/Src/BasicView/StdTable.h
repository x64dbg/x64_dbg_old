#ifndef STDTABLE_H
#define STDTABLE_H

#include <QtGui>
#include "AbstractTableView.h"

class StdTable : public AbstractTableView
{
    Q_OBJECT
public:
    explicit StdTable(QWidget *parent = 0);

    void AbstractMemberFunction(void);

    QString getStringToPrint(int rowBase, int rowOffset, int col);
    
signals:
    
public slots:
    
};

#endif // STDTABLE_H
