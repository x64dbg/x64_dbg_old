#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include <QtGui>
#include <qdebug.h>
#include "NewTypes.h"
#include "AbstractTableView.h"
#include "QBeaEngine.h"
#include "MemoryPage.h"
#include "BeaHighlight.h"

class Disassembly : public AbstractTableView
{
    Q_OBJECT
public:
    explicit Disassembly(MemoryPage *parMemPage, QWidget *parent = 0);

    // Memory page
    void setMemoryPage(MemoryPage* parMemPage);

    // Reimplemented Functions
    QString paintContent(QPainter* painter, int_t rowBase, int rowOffset, int col, int x, int y, int w, int h);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    int_t sliderMovedHook(int type, int_t value, int_t delta);
    void keyPressEvent(QKeyEvent* event);

    // Graphic Dump
    void paintGraphicDump(QPainter* painter, int x, int y, int_t addr);

    // Instructions Management
    int_t getPreviousInstructionRVA(int_t rva, int_t count);
    int_t getNextInstructionRVA(int_t rva, int_t count);
    Instruction_t DisassembleAt(int_t rva);
    Instruction_t DisassembleAt(int_t rva, int_t count);

    // Selection Management
    void expandSelectionUpTo(int_t to);
    void setSingleSelection(int_t index);
    int_t getInitialSelection();
    void selectNext();
    void selectPrevious();
    bool isSelected(int_t base, int_t offset);

    // Index Management
    int_t getIndexFromCount(int_t index, int_t count);
    void prepareData();

signals:
    
public slots:
    void disassambleAt(int_t parVA, int_t parCIP);
    void debugStateChangedSlot(DBGSTATE state);

private:
    void disassembleClear();
    void paintRichText(QPainter* painter, int x, int y, int w, int h, int xinc, const QList<CustomRichText_t>* richText);

    enum GuiState_t {NoState, MultiRowsSelectionState};
    enum GraphicDump_t {GD_Nothing, GD_FootToTop, GD_FootToBottom, GD_HeadFromTop, GD_HeadFromBottom, GD_Vert}; // GD_FootToTop = '- , GD_FootToBottom = ,- , GD_HeadFromTop = '-> , GD_HeadFromBottom = ,-> , GD_Vert = |

    typedef struct _SelectionData_t
    {
        int_t firstSelectedIndex;
        int_t fromIndex;
        int_t toIndex;
    } SelectionData_t;

    QBeaEngine* mDisasm;

    SelectionData_t mSelection;

    bool mIsLastInstDisplayed;

    GuiState_t mGuiState;

    MemoryPage* mMemPage;

    int_t mCipRva;

    QList<Instruction_t> mInstBuffer;
};

#endif // DISASSEMBLY_H
