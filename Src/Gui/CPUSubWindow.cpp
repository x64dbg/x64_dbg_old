#include "CPUSubWindow.h"

CPUSubWindow::CPUSubWindow(QWidget *parent) : QWidget(parent)
{
    // Setup Layouts
    mMainLayout = new QHBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    // Splitters
    mHorizSplitter = new QSplitter(Qt::Horizontal, this);
    mHorizSplitter->setHandleWidth(1);

    mTopVertSplitter = new QSplitter(Qt::Vertical, this);
    mTopVertSplitter->setHandleWidth(1);

    mBottomVertSplitter = new QSplitter(Qt::Vertical, this);
    mBottomVertSplitter->setHandleWidth(1);

    // QFrame Containers
    mTopLeftContainer = new QFrame(this);
    mTopLeftContainer->setFrameShape(QFrame::Box);

    mTopRightContainer = new QFrame(this);
    mTopRightContainer->setFrameShape(QFrame::Box);

    mBottomLeftContainer = new QFrame(this);
    mBottomLeftContainer->setFrameShape(QFrame::Box);

    mBottomRightContainer = new QFrame(this);
    mBottomRightContainer->setFrameShape(QFrame::Box);
/*
    // Interlock all widgets
    mMainLayout->addWidget(mHorizSplitter);

    mHorizSplitter->addWidget(mTopVertSplitter);
    mHorizSplitter->addWidget(mBottomVertSplitter);

    mTopVertSplitter->addWidget(mTopLeftContainer);
    mTopVertSplitter->addWidget(mTopRightContainer);

    mBottomVertSplitter->addWidget(mBottomLeftContainer);
    mBottomVertSplitter->addWidget(mBottomRightContainer);

    pb1 = new QPushButton("ddd", mTopLeftContainer);
    pb2 = new QPushButton("eee", mTopRightContainer);
    pb3 = new QPushButton("jj", mBottomLeftContainer);
    pb4 = new QPushButton("kk", mBottomRightContainer);

    */


    // Interlock all widgets
    mMainLayout->addWidget(mHorizSplitter);

    mHorizSplitter->addWidget(mTopVertSplitter);
    mHorizSplitter->addWidget(mBottomVertSplitter);

    mTopVertSplitter->addWidget(mTopLeftContainer);
    mTopVertSplitter->addWidget(mTopRightContainer);

    mBottomVertSplitter->addWidget(mBottomLeftContainer);
    mBottomVertSplitter->addWidget(mBottomRightContainer);

   // mTopLeftHorizLayout = new QHBoxLayout(mTopLeftContainer);
   // mTopLeftHorizLayout->setContentsMargins(0, 0, 0, 0);
   // mTopLeftHorizLayout->addWidget(pb1);
   // mTopLeftHorizLayout->addWidget(pb2);


    listView = new QListWidget(mTopLeftContainer);

    listView->addItem(QString("eee"));




    //pb1 = new QPushButton("ddd", mTopLeftContainer);
    //pb2 = new QPushButton("eee", mTopRightContainer);
    pb3 = new QPushButton("jj", mBottomLeftContainer);
    pb4 = new QPushButton("kk", mBottomRightContainer);

}
