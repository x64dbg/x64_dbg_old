#ifndef DUMP_H
#define DUMP_H

#include <QtGui>
#include <qdebug.h>
#include "AbstractTableView.h"
#include "MapViewOfMem.h"
#include "QBeaEngine.h"

class HexDump : public AbstractTableView
{
    Q_OBJECT
public:
    explicit HexDump(QWidget *parent = 0);


    void AbstractMemberFunction(void);
    QString getStringToPrint(int rowBase, int rowOffset, int col);

    
signals:
    
public slots:

private:
    typedef struct _SelectionData_t
    {
        int firstSelectedIndex;
        int fromIndex;
        int toIndex;
    } SelectionData_t;

    MapViewOfMem* mMemoryView;
    QBeaEngine* mDisasm;

    SelectionData_t mSelection;
    
};

#endif // DUMP_H
