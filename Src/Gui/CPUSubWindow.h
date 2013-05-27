#ifndef CPUSUBWINDOW_H
#define CPUSUBWINDOW_H

#include <QWidget>
#include <QtGui>

class CPUSubWindow : public QWidget
{
    //Q_OBJECT

public:
    CPUSubWindow(QWidget *parent = 0);
private:
    QHBoxLayout* mMainLayout;
    QHBoxLayout* mTopLeftHorizLayout;

    QSplitter* mHorizSplitter;
    QSplitter* mTopVertSplitter;
    QSplitter* mBottomVertSplitter;

    QFrame* mTopLeftContainer;
    QFrame* mTopRightContainer;
    QFrame* mBottomLeftContainer;
    QFrame* mBottomRightContainer;

    QPushButton* pb1;
    QPushButton* pb2;
    QPushButton* pb3;
    QPushButton* pb4;

    QListWidget* listView;

};

#endif // CPUSUBWINDOW_H
