#ifndef ABSTRACTTABLEVIEW_H
#define ABSTRACTTABLEVIEW_H

#include <QtGui>
#include <QAbstractScrollArea>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QStyle>
#include <QScrollBar>
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
    virtual QString paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h) = 0;

    // Reimplemented Functions
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);

    // ScrollBar Management
    virtual int sliderMovedAction(int type, int value, int delta);
    void forceScrollBarValue(int val);
    void moveScrollBar(int delta);
    void refresh();

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
    virtual void setRowCount(int count);
    int getRowCount();
    int getColumnCount();
    int getRowHeight();
    int getColumnWidth(int index);
    void setColumnWidth(int index, int width);
    int getHeaderHeigth();
    int getTableHeigth();
    int getTableOffset();
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
    int mRowCount;


    int mTableOffset;
    Header_t mHeader;

    int mNbrOfLineToPrint;

    int mPrevTableOffset;

    bool mShouldRepaint;
};

#endif // ABSTRACTTABLEVIEW_H
