#include "AbstractTableView.h"

AbstractTableView::AbstractTableView(QWidget *parent) : QAbstractScrollArea(parent)
{
    // Class variable initialization
    mSelection = (SelectionData_t){0, 0, 0};
    mTableOffset = 0;
    mHeader = (Header_t){true, 20, -1};

    setFont(QFont("Monospace", 8));

    int wRowsHeight = QFontMetrics(this->font()).height();
    wRowsHeight = (wRowsHeight * 105) / 100;
    wRowsHeight = (wRowsHeight % 2) == 0 ? wRowsHeight : wRowsHeight + 1;
    mRowHeight = wRowsHeight;


    mHeaderButtonSytle.setStyleSheet(" QPushButton {\n     background-color: rgb(192, 192, 192);\n     border-style: outset;\n     border-width: 2px;\n     border-color: rgb(128, 128, 128);\n }\n QPushButton:pressed {\n     background-color: rgb(192, 192, 192);\n     border-style: inset;\n }");


    mColResizeData = (ColumnResizingData_t){false, 0, 0};

    mGuiState = AbstractTableView::NoState;

    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);
    addColumnAt(getColumnCount(), 100, false);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setMouseTracking(true);


    mMultiSelTimer = new QTimer(this);
    mMultiSelTimer->start(100);

    connect(mMultiSelTimer, SIGNAL(timeout()), this, SLOT(multiSelTimerSlot()));
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(vertSliderActionSlot(int)));
}


void AbstractTableView::callVirtual()
{
    AbstractMemberFunction();
}




/************************************************************************************
                            Reimplemented Functions
************************************************************************************/
/**
 * @brief       The reimplementation of the paintEvent acts in the GUI management.
 *
 * @param[in]   event       Paint event
 *
 * @return      Nothing.
 */
void AbstractTableView::paintEvent(QPaintEvent* event)
{
    QPainter wPainter(this->viewport());

    int wViewableRowsCount = getViewableRowsCount();
    int x = 0;
    int y = 0;

    // Paint background
    wPainter.fillRect(wPainter.viewport(), QBrush(QColor(255, 251, 240)));

    // Draw header
    if(mHeader.isVisible == true)
    {
        for(int i = 0; i < getColumnCount(); i++)
        {
            QStyleOptionButton wOpt;

            if((mColumnList[i].header.isPressed == true) && (mColumnList[i].header.isMouseOver == true))
                wOpt.state = QStyle::State_Sunken;
            else
                wOpt.state = QStyle::State_Enabled;

            wOpt.rect = QRect(x, y, getColumnWidth(i), getHeaderHeigth());

            mHeaderButtonSytle.style()->drawControl(QStyle::CE_PushButton, &wOpt, &wPainter,&mHeaderButtonSytle);

            x += getColumnWidth(i);
        }
        x = 0;
        y = getHeaderHeigth();
    }

    // Iterate over all columns and cells
    for(int j = 0; j < getColumnCount(); j++)
    {
        for(int i = 0; i < wViewableRowsCount; i++)
        {
            // Highlight selected rows
            /*
            if(isSelected(mTableOffset + i) == true)
                wPainter.fillRect(QRect(x, y, getColumnWidth(j), getRowHeight()), QBrush(QColor(192,192,192)));
                */

            paintSelection(&wPainter, mTableOffset, i, j, x, y, getColumnWidth(j), getRowHeight());

            //  Draw cell content
            if((mTableOffset + i) < getRowCount())
            {
               bool wFlag = paintContent(&wPainter, mTableOffset, i, j, x, y, getColumnWidth(j), getRowHeight());

                if(wFlag == false)
                    wPainter.drawText(QRect(x + 4, y, getColumnWidth(j) - 4, getRowHeight()), Qt::AlignVCenter | Qt::AlignLeft, getStringToPrint(mTableOffset, i, j));
            }
            // Draw cell right border
            wPainter.save() ;
            wPainter.setPen(QColor(128, 128, 128));
            wPainter.drawLine(x + getColumnWidth(j) - 1, y, x + getColumnWidth(j) - 1, y + getRowHeight() - 1);
            wPainter.restore();

            // Update y for the next iteration
            y += getRowHeight();
        }

        y = getHeaderHeigth();
        x += getColumnWidth(j);
    }
}

bool AbstractTableView::paintContent(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h)
{
    return false;
}

void AbstractTableView::paintSelection(QPainter* painter, int rowBase, int rowOffset, int col, int x, int y, int w, int h)
{
    if(isSelected(rowBase + rowOffset) == true)
        painter->fillRect(QRect(x, y, getColumnWidth(col), getRowHeight()), QBrush(QColor(192,192,192)));
}


/**
 * @brief       The reimplementation of the mouseMoveEvent acts in the GUI management.
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void AbstractTableView::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "mouseMoveEvent";

    switch (mGuiState)
    {
        case AbstractTableView::NoState:
        {
            qDebug() << "State = NoState";

            int wColIndex = getColumnIndexFromX(event->x());
            int wStartPos = getColumnPosition(wColIndex); // Position X of the start of column
            int wEndPos = getColumnPosition(wColIndex) + getColumnWidth(wColIndex); // Position X of the end of column

            if(event->buttons() == Qt::NoButton)
            {
                bool wHandle = true;
                bool wHasCursor;

                wHasCursor = cursor().shape() == Qt::SplitHCursor ? true : false;

                if(((wColIndex != 0) && (event->x() >= wStartPos) && (event->x() <= (wStartPos + 2))) || ((wColIndex != (getColumnCount() - 1)) && (event->x() <= wEndPos) && (event->x() >= (wEndPos - 2))))
                {
                    wHandle = true;
                }
                else
                {
                    wHandle = false;
                }

                if((wHandle == true) && (wHasCursor == false))
                {
                    setCursor(Qt::SplitHCursor);
                    mColResizeData.splitHandle = true;
                }
                else if ((wHandle == false) && (wHasCursor == true))
                {
                    unsetCursor();
                    mColResizeData.splitHandle = false;
                }
            }
            else
            {
                QWidget::mouseMoveEvent(event);
            }
            break;
        }
        case AbstractTableView::ResizeColumnState:
        {
            qDebug() << "State = ResizeColumnState";

            int delta = event->x() - mColResizeData.lastPosX;

            int wNewSize = ((getColumnWidth(mColResizeData.index) + delta) >= 20) ? (getColumnWidth(mColResizeData.index) + delta) : (20);

            setColumnWidth(mColResizeData.index, wNewSize);

            mColResizeData.lastPosX = event->x();

            this->viewport()->repaint();

            break;
        }
        case AbstractTableView::MultiRowsSelectionState:
        {
            qDebug() << "State = MultiRowsSelectionState";

            if((transY(event->y()) >= 0) && (transY(event->y()) <= this->getTableHeigth()))
            {
                int wRowIndex = getIndexFromCount(mTableOffset, getRowOffsetFromY(transY(event->y())));

                if(wRowIndex < getRowCount())
                {
                    expandSelectionUpTo(wRowIndex);

                    this->viewport()->repaint();
                }
            }
            break;
        }
        case AbstractTableView::HeaderButtonPressed:
        {
            qDebug() << "State = HeaderButtonPressed";

            int wColIndex = getColumnIndexFromX(event->x());

            if((wColIndex == mHeader.activeButtonIndex) && (event->y() <= getHeaderHeigth()) && (event->y() >= 0))
            {
                mColumnList[mHeader.activeButtonIndex].header.isMouseOver = true;
            }
            else
            {
                mColumnList[mHeader.activeButtonIndex].header.isMouseOver = false;
            }

            this->viewport()->repaint();
        }
        default:
            break;
    }
}


/**
 * @brief       The reimplementation of the mousePressEvent handles several GUI actions as column resizing, selection and header.
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void AbstractTableView::mousePressEvent(QMouseEvent* event)
{
    if(((event->buttons() & Qt::LeftButton) != 0) && ((event->buttons() & Qt::RightButton) == 0))
    {

        if(mColResizeData.splitHandle == true)
        {
            int wColIndex = getColumnIndexFromX(event->x());
            int wStartPos = getColumnPosition(wColIndex); // Position X of the start of column

            mGuiState = AbstractTableView::ResizeColumnState;

            if(event->x() <= (wStartPos + 2))
            {
                mColResizeData.index = wColIndex - 1;
            }
            else
            {
                mColResizeData.index = wColIndex;
            }

            mColResizeData.lastPosX = event->x();
        }
        else if((mHeader.isVisible == true) && (event->y() <= getHeaderHeigth()) && (event->y() >= 0))
        {
            int wColIndex = getColumnIndexFromX(event->x());

            qDebug() << "Button " << wColIndex << "has been pressed.";
            emit headerButtonPressed(wColIndex);

            mColumnList[wColIndex].header.isPressed = true;
            mColumnList[wColIndex].header.isMouseOver = true;

            mHeader.activeButtonIndex = wColIndex;

            mGuiState = AbstractTableView::HeaderButtonPressed;

            this->viewport()->repaint();
        }
        else
        {
            int wRowIndex = getIndexFromCount(mTableOffset, getRowOffsetFromY(transY(event->y())));

            if(wRowIndex < getRowCount())
            {
                setSingleSelection(wRowIndex);

                mGuiState = AbstractTableView::MultiRowsSelectionState;

                viewport()->repaint();
            }
        }
    }

    //QWidget::mousePressEvent(event);
}


/**
 * @brief       The reimplementation of the mouseReleaseEvent handles several GUI actions as column resizing, selection and header.
 *
 * @param[in]   event       Mouse event
 *
 * @return      Nothing.
 */
void AbstractTableView::mouseReleaseEvent(QMouseEvent* event)
{
    if((event->buttons() & Qt::LeftButton) == 0)
    {
        if(mGuiState == AbstractTableView::ResizeColumnState)
        {
            mGuiState = AbstractTableView::NoState;
        }
        else if(mGuiState == AbstractTableView::MultiRowsSelectionState)
        {
            mGuiState = AbstractTableView::NoState;
        }
        else if(mGuiState == AbstractTableView::HeaderButtonPressed)
        {
            if(mColumnList[mHeader.activeButtonIndex].header.isMouseOver == true)
            {
                qDebug() << "Button " << mHeader.activeButtonIndex << "has been released.";
                emit headerButtonReleased(mHeader.activeButtonIndex);
            }
            mGuiState = AbstractTableView::NoState;
        }
        else
        {
            QWidget::mouseReleaseEvent(event);
        }


        for(int i = 0; i < getColumnCount(); i++)
        {
            mColumnList[i].header.isPressed = false;
        }

        this->viewport()->repaint();
    }
}


/**
 * @brief       The reimplementation of the mouseReleaseEvent handles key events.
 *
 * @param[in]   event       Key event
 *
 * @return      Nothing.
 */
void AbstractTableView::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int wSelIndex = getInitialSelection();
    int viewableRowsCount = getViewableRowsCount() - 1;
    int wNewSliderValue = mTableOffset;

    if(key == Qt::Key_Up)
    {
        //wSelIndex -= wSelIndex <= 0 ? 0 : 1;
        wSelIndex = getIndexFromCount(wSelIndex, -1);

        if(wSelIndex <= mTableOffset)
        {
            wNewSliderValue = wSelIndex;
        }
        else if(wSelIndex > getIndexFromCount(mTableOffset, viewableRowsCount - 1))
        {
            wNewSliderValue = getIndexFromCount(wSelIndex, -viewableRowsCount + 1);
        }

        selectPrevious();
    }
    else if(key == Qt::Key_Down)
    {
        //wSelIndex += wSelIndex < getRowCount() - 1 ? 1 : 0;
        wSelIndex = getIndexFromCount(wSelIndex, 1);

        if(wSelIndex < mTableOffset)
        {
            wNewSliderValue = wSelIndex;
        }
        else if(wSelIndex >= getIndexFromCount(mTableOffset, viewableRowsCount - 1))
        {
            wNewSliderValue = getIndexFromCount(wSelIndex, -viewableRowsCount + 1);
        }

        selectNext();
    }

    if(wNewSliderValue != mTableOffset)
    {
        verticalScrollBar()->setValue(wNewSliderValue);
        verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
    }
    viewport()->repaint();
}


void AbstractTableView::wheelEvent(QWheelEvent* event)
{
    qDebug() << "wheelEvent";

    //QAbstractScrollArea::wheelEvent(event);

    int value = verticalScrollBar()->value();

    if(event->delta() > 0)
    {
        value -=3;
    }
    else
    {
        value +=3;
    }

    verticalScrollBar()->setValue(value);
    verticalScrollBar()->triggerAction(QAbstractSlider::SliderNoAction);
}


/************************************************************************************
                            ScrollBar Management
 ***********************************************************************************/
/**
 * @brief       vertSliderActionSlot is the slot connected to the actionTriggered signal from the vertical scrollbar.
 *
 * @param[in]   action      Slider action type
 *
 * @return      Nothing.
 */
void AbstractTableView::vertSliderActionSlot(int action)
{
    int wSliderPosition = 0;
    int wOldValue = mTableOffset;
    int wDelta;

    if(verticalScrollBar()->sliderPosition() < 0)
        wSliderPosition = 0;
    else if(verticalScrollBar()->sliderPosition() > getRowCount() - 1)
        wSliderPosition = getRowCount() - 1;
    else
        wSliderPosition = verticalScrollBar()->sliderPosition();

    wDelta = wSliderPosition - wOldValue;
    qDebug() << "wDelta " << wDelta;
    qDebug() << "mTableOffset " << mTableOffset;

    qDebug() << "Scroll Action Slot " << wSliderPosition;

    switch(action)
    {
        case QAbstractSlider::SliderSingleStepSub:
        {
            qDebug() << "SliderSingleStepSub";
            int value = wSliderPosition;
            mTableOffset = getIndexFromCount(mTableOffset, wDelta);
        }
        break;

        case QAbstractSlider::SliderPageStepSub:
        {
            qDebug() << "SliderPageStepSub";
            int value = wSliderPosition;
            mTableOffset = getIndexFromCount(mTableOffset, wDelta);
        }
        break;

        case QAbstractSlider::SliderSingleStepAdd:
        {
            qDebug() << "SliderSingleStepAdd";
            int value = wSliderPosition;
            mTableOffset = getIndexFromCount(mTableOffset, wDelta);
        }
        break;

        case QAbstractSlider::SliderPageStepAdd:
        {
            qDebug() << "SliderPageStepAdd";
            int value = wSliderPosition;
            mTableOffset = getIndexFromCount(mTableOffset, wDelta);
        }
        break;

        case QAbstractSlider::SliderToMinimum:
        case QAbstractSlider::SliderToMaximum:
        case QAbstractSlider::SliderMove:
        {
            qDebug() << "SliderMove";
            int value = wSliderPosition;

            if(value > 0)
            {
                mTableOffset = getIndexFromCount(value, -1);
                mTableOffset = getIndexFromCount(mTableOffset, 1);
            }
            else
            {
                mTableOffset = 0;
            }

            // mTableOffset = value;
        }
        break;

        case QAbstractSlider::SliderNoAction:
        {
            qDebug() << "SliderNoAction";
            int value = wSliderPosition;
            mTableOffset = getIndexFromCount(mTableOffset, wDelta);
        }
        break;

        default:
            break;
    }

    verticalScrollBar()->setValue(mTableOffset);

    sliderMovedAction(action, wOldValue, wSliderPosition - wOldValue);

    qDebug() << "New mTableOffset: " << mTableOffset;

    viewport()->repaint();
}


/**
 * @brief       This method is the slot connected to the timeout() signal from the timer used for the multilines selection.
 *
 * @return      Nothing.
 */
void AbstractTableView::multiSelTimerSlot()
{
    //qDebug() << "timeout";

    if(mGuiState == AbstractTableView::MultiRowsSelectionState)
    {
        int wY = transY(this->mapFromGlobal(QCursor::pos()).y());

        if((wY < 0) || (wY > getTableHeigth()))
        {
            int wNewScrollBarValue;
            int viewableRowsCount = getViewableRowsCount();
            int wNewMultiSelEnd;

            wY = (wY < 0) ? wY : (wY - getTableHeigth());

            if(wY < 0)
            {
                wNewScrollBarValue = getIndexFromCount(mTableOffset, wY);
                wNewScrollBarValue = (wNewScrollBarValue < 0) ? 0 : wNewScrollBarValue;
                wNewMultiSelEnd = wNewScrollBarValue;
            }
            else if(wY > 0)
            {
                wNewScrollBarValue = getIndexFromCount(mTableOffset, wY);
                wNewScrollBarValue = (wNewScrollBarValue > getRowCount() - 1) ? getRowCount() - 1 : wNewScrollBarValue;
                wNewMultiSelEnd = getIndexFromCount(wNewScrollBarValue, viewableRowsCount - 1);
                wNewMultiSelEnd = (wNewMultiSelEnd > getRowCount() - 1) ? getRowCount() - 1 : wNewMultiSelEnd;
            }

            expandSelectionUpTo(wNewMultiSelEnd);

            verticalScrollBar()->setValue(wNewScrollBarValue);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderMove);
        }
    }
}


/**
 * @brief       Set the number of rows.
 *
 * @param[in]   count      Row count
 *
 * @return      Nothing.
 */
void AbstractTableView::setRowCount(int count)
{
    mRowCount = count > 0 ? count : 0;
    verticalScrollBar()->setRange(0, count > 0 ? count - 1 : 0);
}


/**
 * @brief       Move slider to value without trigger the actionTriggered signal of the vertical scrollbar.
 *
 * @param[in]   value      Set the number of rows
 *
 * @return      Nothing.
 */
void AbstractTableView::setScrollBarValue(int value)
{
    verticalScrollBar()->setSliderPosition(value);
    mTableOffset = value;
}


/**
 * @brief       This virtual method is called at the end of the vertSliderActionSlot(...) method.
 *              It allows changing the slider postion according to the type of action, the new value
 *              of the scrollbar and the scrollbar delta.
 *
 * @param[in]   type      Type of action (Refer to the QAbstractSlider::SliderAction enum)
 * @param[in]   value     New value of the scrollbar (Index of the top table item)
 * @param[in]   delta     Scrollbar value delta compared to the previous state
 *
 * @return      Nothing.
 */
void AbstractTableView::sliderMovedAction(int type, int value, int delta)
{
    qDebug() << "AbstractTableView: sliderMovedAction: value " << value << " delta " << delta;
}


QScrollBar* AbstractTableView::getScrollbar()
{
    return this->verticalScrollBar();
}




/************************************************************************************
                                Selection Management
************************************************************************************/
/**
 * @brief       Expand the multilines selection up to the index to.
 *
 * @param[in]   to      End of the multilines selection
 *
 * @return      Nothing.
 */
void AbstractTableView::expandSelectionUpTo(int to)
{
    if(to < mSelection.firstSelectedIndex)
    {
        mSelection.fromIndex = to;
        mSelection.toIndex = mSelection.firstSelectedIndex;
    }
    else if(to > mSelection.firstSelectedIndex)
    {
        mSelection.fromIndex = mSelection.firstSelectedIndex;
        mSelection.toIndex = to;
    }
}


/**
 * @brief       Selects the line at the index index.
 *
 * @param[in]   index      Index of the single line selection
 *
 * @return      Nothing.
 */
void AbstractTableView::setSingleSelection(int index)
{
    mSelection.firstSelectedIndex = index;
    mSelection.fromIndex = index;
    mSelection.toIndex = index;
}


/**
 * @brief       Returns the index of the selection in case of single line selection or the first selected line in case of
 *              multilines selection.
 *
 * @return      Initial selection index.
 */
int AbstractTableView::getInitialSelection()
{
    return mSelection.firstSelectedIndex;
}


/**
 * @brief       Checks if the row at index index is selected.
 *
 * @param[in]   index      Row index
 *
 * @return      True if the line is selected. False if the line is not selected.
 */
bool AbstractTableView::isSelected(int index)
{
    if(index >= mSelection.fromIndex && index <= mSelection.toIndex)
        return true;
    else
        return false;
}


void AbstractTableView::selectNext()
{
    int wInit = mSelection.firstSelectedIndex + 1;

    if(wInit > getRowCount() - 1)
        wInit = getRowCount() - 1;

    mSelection.firstSelectedIndex = wInit;
    mSelection.fromIndex = wInit;
    mSelection.toIndex = wInit;
}


void AbstractTableView::selectPrevious()
{
    int wInit = mSelection.firstSelectedIndex - 1;

    if(wInit < 0)
        wInit = 0;

    mSelection.firstSelectedIndex = wInit;
    mSelection.fromIndex = wInit;
    mSelection.toIndex = wInit;
}


/************************************************************************************
                            GUI Utils
************************************************************************************/
/**
 * @brief       Returns the absolute row index from the given y coordinate. The header heigth needs to be substract from
 *              y before calling this method.
 *
 * @param[in]   y      Pixel offset starting from the top of the table (Without the header)
 *
 * @return      Absolute row index.
 */
int AbstractTableView::getRowIndexFromY(int y)
{
    return (y / getRowHeight() + mTableOffset);
}


int AbstractTableView::getRowOffsetFromY(int y)
{
    return (y / getRowHeight());
}


/**
 * @brief       Returns the index of the column according to the given x coordinate.
 *
 * @param[in]   x      Pixel offset starting from the left of the table
 *
 * @return      Column index.
 */
int AbstractTableView::getColumnIndexFromX(int x)
{
    int wX = 0;
    int wColIndex = 0;

    while(wColIndex < getColumnCount())
    {
        wX += getColumnWidth(wColIndex);

        if(x <= wX)
        {
            return wColIndex;
        }
        else if(wColIndex < getColumnCount())
        {
            wColIndex++;
        }
    }
    return getColumnCount() - 1;
}


/**
 * @brief       Returns the x coordinate of the begining of the column at index index.
 *
 * @param[in]   index      Column index.
 *
 * @return      X coordinate of the column index.
 */
int AbstractTableView::getColumnPosition(int index)
{
    int posX = 0;

    if((index >= 0) && (index < getColumnCount()))
    {
        for(int i = 0; i <= (index - 1); i++)
        {
            posX += getColumnWidth(i);
        }

        return posX;
    }
    else
    {
        return -1;
    }
}


/**
 * @brief       Substract the header heigth from the given y.
 *
 * @param[in]   y      y coordinate
 *
 * @return      y - getHeaderHeigth().
 */
int AbstractTableView::transY(int y)
{
    return y - getHeaderHeigth();
}


/**
 * @brief       Returns the number of viewable rows (Partially viewable rows are aslo counted).
 *
 * @return      Number of viewable rows.
 */
int AbstractTableView::getViewableRowsCount()
{
    int wTableHeight = this->height() - getHeaderHeigth();
    int wCount = wTableHeight / getRowHeight();

    wCount += (wTableHeight % getRowHeight()) > 0 ? 1 : 0;

    return wCount;
}




/************************************************************************************
                                New Columns
************************************************************************************/
void AbstractTableView::addColumnAt(int at, int width, bool isClickable)
{
    HeaderButton_t wHeaderButton;
    Column_t wColumn;

    wHeaderButton.isPressed = false;
    wHeaderButton.isClickable = isClickable;
    wHeaderButton.isMouseOver = false;

    wColumn.header = wHeaderButton;
    wColumn.width = width;

    if(at < 0)
        at = 0;

    if(at > mColumnList.size())
        at = mColumnList.size();

    mColumnList.insert(at, wColumn);
}




/************************************************************************************
                                Getter & Setter
************************************************************************************/
int AbstractTableView::getRowCount()
{
    return mRowCount;
}


int AbstractTableView::getColumnCount()
{
    return mColumnList.size();
}

int AbstractTableView::getRowHeight()
{
    return mRowHeight;
}


int AbstractTableView::getColumnWidth(int index)
{
    if(index < 0)
    {
        return -1;
    }

    if(index < (getColumnCount() - 1))
    {
        return mColumnList.at(index).width;
    }
    else if(index == (getColumnCount() - 1))
    {
        int wGlobWidth = 0;

        for(int i = 0; i < getColumnCount() - 1; i++)
            wGlobWidth += getColumnWidth(i);

        return this->width() - wGlobWidth;
    }

    return 0;
}


void AbstractTableView::setColumnWidth(int index, int width)
{
    mColumnList[index].width = width;
}


int AbstractTableView::getHeaderHeigth()
{
    if(mHeader.isVisible == true)
        return mHeader.height;
    else
        return 0;
}


int AbstractTableView::getTableHeigth()
{
    return this->height() - getHeaderHeigth();
}


int AbstractTableView::getTableOffset()
{
    return mTableOffset;
}


int AbstractTableView::setTableOffset(int value)
{
    mTableOffset = value;
}


/**
 * @brief       Return the index of the count-th row after the top displayed row.
 *
 * @param[in]   count      Row count
 *
 * @return      Index of count-th row after the top displayed row.
 */
int AbstractTableView::getIndexFromCount(int index, int count)
{
    int wIndex = index + count;

    if(wIndex < 0)
        wIndex = 0;

    if(wIndex > getRowCount())
        wIndex = getRowCount() - 1;

    return wIndex;
}

