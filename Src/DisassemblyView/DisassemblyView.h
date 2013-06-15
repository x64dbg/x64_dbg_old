#ifndef DISASSEMBLYVIEW_H
#define DISASSEMBLYVIEW_H

#include <QWidget>
#include <QtGui>
#include <qdebug.h>
#include "MapViewOfMem.h"
#include "QBeaEngine.h"

typedef struct _HeaderButton_t
{
    bool isClickable;
    int height;
    QPushButton* button;
} HeaderButton_t;


typedef struct _ColumnItem_t
{
    int width;
    HeaderButton_t header;
} ColumnItem_t;

typedef struct _RowItem_t
{
    bool isSelected;
} RowItem_t;

typedef struct _ColumnResizingData_t
{
    bool splitHandle;
    int index;
    int lastPosX;
} ColumnResizingData_t;



class DisassemblyView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    enum GuiState_t {NoState, ResizeColumnState, MultiRowsSelectionState};
    enum GraphicDump_t {GD_Nothing, GD_FootToTop, GD_FootToBottom, GD_HeadFromTop, GD_HeadFromBottom, GD_Vert}; // GD_FootToTop = '- , GD_FootToBottom = ,- , GD_HeadFromTop = '-> , GD_HeadFromBottom = ,-> , GD_Vert = |

    explicit DisassemblyView(MapViewOfMem memory, QWidget *parent = 0);

    QString getStringToPrint(int topTableAddress, int rowIndex, int colIndex);
    void paintGraphicDump(QPainter *painter, int x, int y, int topTableRVA, int rowIndex);

    // Instructions Management
    int getPreviousInstructionRVA(int address, int count);
    int getNextInstructionRVA(int address, int count);
    Instruction_t DisassembleAt(ulong rva);
    Instruction_t DisassembleAt(ulong rva, ulong count);

    // Selection Management
    bool isSelected(ulong rva, ulong count);
    void expandSelectionUpTo(ulong rva, ulong count);
    void setSingleSelection(ulong rva, ulong count);
    int getInitialSelected();
    int getSelectionHead();

    // ScrollBar Management
    void updateVertScrollbarRange();

    // Coordinate analysis
    int getRowIndexFromY(int y);
    int getColumnIndexFromX(int x);
    int getColumnPosition(int index);

    // Reimplemented Functions
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);


    // Getters and Setters
    int rowHeight();
    void setRowHeight(int heigth);
    int rowCount();
    void setRowCount(int count);
    int columnCount();
    void addColumn();
    int columnWidth(int index);
    void setColumnWidth(int index, int width);
    int headerOffset();

signals:
    
public slots:
    // ScrollBar Management
    void vertSliderActionSlot(int action);

    // Selection Management
    void multiSelTimerSlot();

private:
    MapViewOfMem mMemoryView;

    int mRowHeight;
    int mRowCount;

    QList<ColumnItem_t> mColumnItemList;
    QList<RowItem_t> mRowItemList;

    GuiState_t mGuiState;

    ColumnResizingData_t mColResizeData;


    int mTopTableRVA;   // RVA of the first displayed instruction

    QTimer* mMultiSelTimer;

    QBeaEngine mDisasm;

    bool isHeaderVisible;


};

#endif // DISASSEMBLYVIEW_H
