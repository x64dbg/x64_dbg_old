#ifndef COLUMNWIDGET_H
#define COLUMNWIDGET_H

#include <QWidget>
#include <QtGui>

class ColumnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColumnWidget(int n, QWidget *parent = 0);
    int getColumnCount(void);
    
signals:
    
public slots:

private:
    QHBoxLayout* mHLayout;

    QSplitter* mHSplitter;

    QList<QWidget*>* mWidgetList;
    QList<QPushButton*>* mTopButtonList;
    QList<QFrame*>* mColumnList;
    QList<QVBoxLayout*>* mVLayoutList;
};

#endif // COLUMNWIDGET_H
