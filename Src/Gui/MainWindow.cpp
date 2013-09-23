#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //Create QMdiSubWindow
    QMdiSubWindow* subWindow = new QMdiSubWindow();
    subWindow->setWindowTitle("Disassembly");
    subWindow->showMaximized();

    mCpuWin = new CPUWidget();


    subWindow->setWidget(mCpuWin);

    //Add subWindow to Main QMdiArea here
    mdiArea->addSubWindow(subWindow);


    setCentralWidget(mdiArea);


    // Setup Signals/Slots
    connect(ui->actionStepOver, SIGNAL(triggered()), mCpuWin, SLOT(stepOverSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

