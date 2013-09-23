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

    void AbstractMemberFunction(void);
    int sliderMovedAction(int type, int value, int delta);

    //QString getStringToPrint(int rowBase, int rowOffset, int col);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    int getPreviousInstructionRVA(int address, int count);
    int getNextInstructionRVA(int address, int count);
    Instruction_t DisassembleAt(ulong rva);
    Instruction_t DisassembleAt(ulong rva, ulong count);

    QString paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h);
    void paintGraphicDump(QPainter* painter, int x, int y, int addr);

    // Selection Management
    void expandSelectionUpTo(int to);
    void setSingleSelection(int index);
    int getInitialSelection();
    bool isSelected(int base, int offset);
    void selectNext();
    void selectPrevious();


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
