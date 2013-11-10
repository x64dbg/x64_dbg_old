#ifndef ABSTRACTTABLEVIEW_H
#define ABSTRACTTABLEVIEW_H

#include <QtGui>
#include <qdebug.h>
#include <NewTypes.h>


class AbstractTableView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    enum GuiState_t {NoState, ReadyToResize, ResizeColumnState, HeaderButtonPressed};

    // Constructor
    explicit AbstractTableView(QWidget *parent = 0);

    // Pure Virtual Methods
    virtual QString paintContent(QPainter* painter, int_t rowBase, int rowOffset, int col, int x, int y, int w, int h) = 0;

    // Reimplemented Functions
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);

    // ScrollBar Management
    virtual int_t sliderMovedAction(int type, int_t value, int_t delta);
    void forceScrollBarValue(int_t val);
    void moveScrollBar(int delta);
    int scaleFromUint64ToScrollBarRange(int_t value);
    int_t scaleFromScrollBarRangeToUint64(int value);
    bool updateScrollBarRange(int_t range);
    void repaint();

    // Coordinates Utils
    int getIndexOffsetFromY(int y);
    int getColumnIndexFromX(int x);
    int getColumnPosition(int index);
    int transY(int y);
    int getViewableRowsCount();
    virtual int getLineToPrintcount();
    virtual void prepareData();

    // New Columns
    virtual int addColumnAt(int at, int width, bool isClickable);

    // Getter & Setter
    virtual void setRowCount(int_t count);
    int_t getRowCount();
    int getColumnCount();
    int getRowHeight();
    int getColumnWidth(int index);
    void setColumnWidth(int index, int width);
    int getHeaderHeigth();
    int getTableHeigth();
    int_t getTableOffset();
    int getGuiState();
    int getNbrOfLineToPrint();
    void setNbrOfLineToPrint(int parNbrOfLineToPrint);
    void reloadData();

signals:
    void headerButtonPressed(int col);
    void headerButtonReleased(int col);

public slots:
    // ScrollBar Management
    void vertSliderActionSlot(int action);

private:
    typedef struct _ColumnResizingData_t
    {
        bool splitHandle;
        int index;
        int lastPosX;
    } ColumnResizingData_t;

    typedef struct _HeaderButton_t
    {
        bool isClickable;
        bool isPressed;
        bool isMouseOver;
    } HeaderButton_t;

    typedef struct _Column_t
    {
        int width;
        HeaderButton_t header;
    } Column_t;


    typedef struct _Header_t
    {
        bool isVisible;
        int height;
        int activeButtonIndex;
    } Header_t;

    GuiState_t mGuiState;

    ColumnResizingData_t mColResizeData;

    QPushButton mHeaderButtonSytle;

    QList<Column_t> mColumnList;

    int mRowHeight;
    int_t mRowCount;


    int_t mTableOffset;
    Header_t mHeader;

    int mNbrOfLineToPrint;

    int_t mPrevTableOffset;

    bool mShouldReload;

    int_t mOldTableOffset;

    typedef struct _ScrollBar64_t
    {
        bool is64;
        int rightShiftCount;
    } ScrollBar64_t;

    ScrollBar64_t mScrollBarAttributes;
};

#endif // ABSTRACTTABLEVIEW_H
