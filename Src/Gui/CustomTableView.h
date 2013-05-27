#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableWidget>
#include <QtGui>

class CustomTableView : public QTableWidget
{
    Q_OBJECT

public:

    enum MouseState {ResizeSection, MoveSection, SelectSections, NoState};

    explicit CustomTableView(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
signals:

public slots:

private:
    MouseState ms;

};

#endif // CUSTOMTABLEVIEW_H
