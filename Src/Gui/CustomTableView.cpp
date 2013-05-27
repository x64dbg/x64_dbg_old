#include "CustomTableView.h"

CustomTableView::CustomTableView(QWidget *parent) :QTableWidget(parent)
{
    ms = NoState;

    //this->setIndentation(0);
    this->setFixedSize(400,200);
    this->verticalHeader()->setVisible(false);

    this->setColumnCount(3);
    this->setSortingEnabled(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setFocusPolicy(Qt::NoFocus);
    this->setShowGrid(false);

    this->setMouseTracking(true);

    this->setStyleSheet ( " QTableWidget::item:focus { border: 0px }" );

    /*
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i)
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    this->insertTopLevelItems(0, items);*/

    for (int i = 0; i < 10; ++i)
        this->insertRow(i);

}


/*
void CustomTableView::paintEvent(QPaintEvent *event)
{
    QTreeWidget::paintEvent(event);
    QPainter painter(viewport());
    int myHeight = this->at height();
    int i = 0;
    for (i = 0; i < columnCount(); i++) {
        int startPos = this->vi;
        QPoint myFrom = QPoint(startPos, 0);
        QPoint myTo = QPoint(startPos, height());
        painter.drawLine(myFrom, myTo);
        startPos += horizontalHeader()->sectionSize(i) - 3;
        myFrom = QPoint(startPos, 0);
        myTo = QPoint(startPos, height());
        painter.drawLine(myFrom, myTo);
    }
    for (i = 0; i < rowCount(); i++) {
        int startPos = verticalHeader()->sectionViewportPosition(i);
        QPoint myFrom = QPoint(0, startPos);
        QPoint myTo = QPoint(width(), startPos);
        painter.drawLine(myFrom, myTo);
        startPos += verticalHeader()->sectionSize(i);
        myFrom = QPoint(0, startPos);
        myTo = QPoint(width(), startPos);
        painter.drawLine(myFrom, myTo);
    }
}
*/
/*
void CustomTableView::paintEvent(QPaintEvent* event)
{
    QTableWidget::paintEvent(event);

   int pHeight;
   int pWidth;

   QPainter p;
   p.begin(viewport());

   int rc = rowCount();
   int py = 0;

   for(int i = 0; i < rc; i++)
   {
       pHeight = rowHeight(i);
       pWidth = columnWidth(0);

       p.drawRect(1,py,pWidth-2,pHeight+5);
       p.drawRect(columnWidth(0)-1,py,columnWidth(1),pHeight+5);
       py += pHeight+5;
   }

   p.end();

}
*/
void CustomTableView::paintEvent(QPaintEvent *event)
{
    /*
    QTableWidget::paintEvent(event);

    //firstVisualRow is the visual index of the first visible row.  lastVisualRow is the visual index of the last visible Row.
    //same goes for ...VisualColumn
    int firstVisualRow = verticalHeader()->visualIndexAt(0);
    int lastVisualRow = verticalHeader()->visualIndexAt(verticalHeader()->viewport()->height());

    int firstVisualColumn = horizontalHeader()->visualIndexAt(0);
    int lastVisualColumn = horizontalHeader()->visualIndexAt(horizontalHeader()->viewport()->width());

    firstVisualRow = 0;
    lastVisualRow = 4;

    firstVisualColumn=1;
    lastVisualColumn=1;

    int pHeight;
    int pWidth;

    QPainter painter;
    painter.begin(viewport());


    QRect dirtyArea(viewport()->x(),viewport()->y(),viewport()->width(),viewport()->height());
    bool rightToLeft = false;

    const int gridSize = 1;

    // get the horizontal start and end visual sections
    int left = horizontalHeader()->visualIndexAt(dirtyArea.left());
    int right = horizontalHeader()->visualIndexAt(dirtyArea.right());
    if (rightToLeft)
        qSwap(left, right);
    if (left == -1) left = 0;
    if (right == -1) right = horizontalHeader()->count() - 1;

    // get the vertical start and end visual sections and if alternate color
    int bottom = verticalHeader()->visualIndexAt(dirtyArea.bottom());
    if (bottom == -1) bottom = verticalHeader()->count() - 1;
    int top = 0;


    top = verticalHeader()->visualIndexAt(dirtyArea.top());


    if (top == -1 || top > bottom)
        return;


    // Paint each row
    for (int visualIndex = top; visualIndex <= bottom; ++visualIndex) {
        int row = verticalHeader()->logicalIndex(visualIndex);
        if (verticalHeader()->isSectionHidden(row))
            continue;
        int rowY = rowViewportPosition(row);
        //rowY += offset.y();
        int rowh = rowHeight(row) - gridSize;
        painter.drawLine(dirtyArea.left(), rowY + rowh, dirtyArea.right(), rowY + rowh);
    }

    // Paint each column
    for (int h = left; h <= right; ++h) {
        int col = horizontalHeader()->logicalIndex(h);
        if (horizontalHeader()->isSectionHidden(col))
            continue;
        int colp = columnViewportPosition(col);
       // colp += offset.x();

        if (!rightToLeft)
            colp +=  columnWidth(col) - gridSize;
        painter.drawLine(colp, dirtyArea.top(), colp, dirtyArea.bottom());
    }

    painter.end();
*/

    QTableWidget::paintEvent(event);
    QPainter painter(viewport());
    int myHeight = horizontalHeader()->height();
    int i = 0;

    for (i = 0; i < columnCount(); i++) {
        int startPos = horizontalHeader()->sectionViewportPosition(i);
        QPoint myFrom = QPoint(startPos - 1, 0);
        QPoint myTo = QPoint(startPos - 1, height());
        painter.drawLine(myFrom, myTo);
        /*
        startPos += horizontalHeader()->sectionSize(i) - 3;
        myFrom = QPoint(startPos, 0);
        myTo = QPoint(startPos, height());
        painter.drawLine(myFrom, myTo);
        */
    }
    /*
    for (i = 0; i < rowCount(); i++) {
        int startPos = verticalHeader()->sectionViewportPosition(i);
        QPoint myFrom = QPoint(0, startPos);
        QPoint myTo = QPoint(width(), startPos);
        painter.drawLine(myFrom, myTo);
        startPos += verticalHeader()->sectionSize(i);
        myFrom = QPoint(0, startPos);
        myTo = QPoint(width(), startPos);
        painter.drawLine(myFrom, myTo);
    }
*/
    }


void CustomTableView::mouseMoveEvent(QMouseEvent *e)
{
    int pos = e->x();
    if (pos < 0)
        return;

    switch (ms) {/*
        case QHeaderViewPrivate::ResizeSection: {
            Q_ASSERT(d->originalSize != -1);
            if (d->cascadingResizing) {
                int delta = d->reverse() ? d->lastPos - pos : pos - d->lastPos;
                int visual = visualIndex(d->section);
                d->cascadingResize(visual, d->headerSectionSize(visual) + delta);
            } else {
                int delta = d->reverse() ? d->firstPos - pos : pos - d->firstPos;
                resizeSection(d->section, qMax(d->originalSize + delta, minimumSectionSize()));
            }
            d->lastPos = pos;
            return;
        }
        case QHeaderViewPrivate::MoveSection: {
            if (qAbs(pos - d->firstPos) >= QApplication::startDragDistance()
                || !d->sectionIndicator->isHidden()) {
                int visual = visualIndexAt(pos);
                if (visual == -1)
                    return;
                int posThreshold = d->headerSectionPosition(visual) - d->offset + d->headerSectionSize(visual) / 2;
                int moving = visualIndex(d->section);
                if (visual < moving) {
                    if (pos < posThreshold)
                        d->target = d->logicalIndex(visual);
                    else
                        d->target = d->logicalIndex(visual + 1);
                } else if (visual > moving) {
                    if (pos > posThreshold)
                        d->target = d->logicalIndex(visual);
                    else
                        d->target = d->logicalIndex(visual - 1);
                } else {
                    d->target = d->section;
                }
                d->updateSectionIndicator(d->section, pos);
            }
            return;
        }
        case QHeaderViewPrivate::SelectSections: {
            int logical = logicalIndexAt(pos);
            if (logical == d->pressed)
                return; // nothing to do
            else if (d->pressed != -1)
                updateSection(d->pressed);
            d->pressed = logical;
            if (d->clickableSections && logical != -1) {
                emit sectionEntered(d->pressed);
                updateSection(d->pressed);
            }
            return;
        }*/
        case CustomTableView::NoState: {
            int handle = 1;//d->sectionHandleAt(pos);

            if(pos > 50 && pos < 55)
                handle = 1;
            else
                handle = -1;

            bool hasCursor = testAttribute(Qt::WA_SetCursor);
            if (handle != -1) {
                if (!hasCursor)
                    setCursor(Qt::SplitHCursor);
            } else if (hasCursor) {
                unsetCursor();
            }
            return;
        }
        default:
            break;
    }
}
