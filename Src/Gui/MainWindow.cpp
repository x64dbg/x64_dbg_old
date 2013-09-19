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
    //subWindow->resize(200,100);
    subWindow->showMaximized();



    CPUWidget *cpuWin = new CPUWidget();
    Disassembly *dias = new Disassembly(0);

    cpuWin->getTopLeftWidget()->addWidget(dias);


    subWindow->setWidget(cpuWin);

    //Add subWindow to Main QMdiArea here
    mdiArea->addSubWindow(subWindow);


    setCentralWidget(mdiArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

