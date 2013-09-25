#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include <QtGui>
#include <qdebug.h>
#include "AbstractTableView.h"
#include "MapViewOfMem.h"
#include "QBeaEngine.h"

class Disassembly : public AbstractTableView
{
    Q_OBJECT
public:
    explicit Disassembly(QWidget *parent = 0);

    // Reimplemented Functions
    QString paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    int sliderMovedAction(int type, int value, int delta);
    void keyPressEvent(QKeyEvent* event);

    // Graphic Dump
    void paintGraphicDump(QPainter* painter, int x, int y, int addr);

    // Instructions Management
    int getPreviousInstructionRVA(int rva, int count);
    int getNextInstructionRVA(int rva, int count);
    Instruction_t DisassembleAt(ulong rva);
    Instruction_t DisassembleAt(ulong rva, ulong count);

    // Selection Management
    void expandSelectionUpTo(int to);
    void setSingleSelection(int index);
    int getInitialSelection();
    void selectNext();
    void selectPrevious();
    bool isSelected(int base, int offset);

    // Index Management
    int getIndexFromCount(int index, int count);
    int getLineToPrintcount();

signals:
    
public slots:

private:
    enum GuiState_t {NoState, MultiRowsSelectionState};
    enum GraphicDump_t {GD_Nothing, GD_FootToTop, GD_FootToBottom, GD_HeadFromTop, GD_HeadFromBottom, GD_Vert}; // GD_FootToTop = '- , GD_FootToBottom = ,- , GD_HeadFromTop = '-> , GD_HeadFromBottom = ,-> , GD_Vert = |

    typedef struct _SelectionData_t
    {
        int firstSelectedIndex;
        int fromIndex;
        int toIndex;
    } SelectionData_t;

    MapViewOfMem* mMemoryView;
    QBeaEngine* mDisasm;

    SelectionData_t mSelection;

    bool mIsLastInstDisplayed;

    GuiState_t mGuiState;
};

#endif // DISASSEMBLY_H
