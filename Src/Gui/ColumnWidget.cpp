#include "ColumnWidget.h"

ColumnWidget::ColumnWidget(int n, QWidget *parent) :QWidget(parent)
{
    this->setFixedSize(800,800);

    mHSplitter = new QSplitter(Qt::Horizontal, this);
    mHSplitter->setHandleWidth(1);

    // Setup Layouts
    mHLayout = new QHBoxLayout(this);
    mHLayout->setContentsMargins(0, 0, 0, 0);
    mHLayout->addWidget(mHSplitter);

    mWidgetList = new QList<QWidget*>;

    mTopButtonList = new QList<QPushButton*>;

    mColumnList = new QList<QFrame*>;

    mVLayoutList = new QList<QVBoxLayout*>;


    for(int wI = 0; wI < n; wI++)
    {
        mWidgetList->append(new QWidget(this));
        mHSplitter->addWidget(mWidgetList->at(wI));

        mVLayoutList->append(new QVBoxLayout(mWidgetList->at(wI)));
        mVLayoutList->at(wI)->setContentsMargins(0, 0, 0, 0);
        mVLayoutList->at(wI)->setSpacing(0);

        mTopButtonList->append(new QPushButton(this));
        mVLayoutList->at(wI)->addWidget(mTopButtonList->at(wI));

        mColumnList->append(new QFrame(this));
        mColumnList->at(wI)->setFrameShape(QFrame::Box);
        mVLayoutList->at(wI)->addWidget(mColumnList->at(wI));
    }
}

int ColumnWidget::getColumnCount(void)
{
    return mWidgetList->size();
}
