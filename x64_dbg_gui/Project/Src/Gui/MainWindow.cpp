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

    // Setup the command bar
    mCmdLineEdit = new CommandLineEdit(ui->cmdBar);
    ui->cmdBar->addWidget(new QLabel("Command "));
    ui->cmdBar->addWidget(mCmdLineEdit);


    // Setup Signals/Slots
    connect(ui->actionStepOver, SIGNAL(triggered()), mCpuWin, SLOT(stepOverSlot()));
    connect(mCmdLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(ui->actionStepOver,SIGNAL(triggered()),this,SLOT(execStepOver()));
    connect(ui->actionStepInto,SIGNAL(triggered()),this,SLOT(execStepInto()));
    connect(ui->actionCommand,SIGNAL(triggered()),this,SLOT(setFocusToCommandBar()));

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::executeCommand()
{
    QString wCmd = mCmdLineEdit->text();

    Bridge::getBridge()->execCmd(reinterpret_cast<const char*>(wCmd.toAscii().data()));

    mCmdLineEdit->addCmdToHistory(wCmd);
    mCmdLineEdit->setText("");
}




void MainWindow::execStepOver()
{
    Bridge::getBridge()->execCmd("sto");
}

void MainWindow::execStepInto()
{
    Bridge::getBridge()->execCmd("sti");
}

void MainWindow::setFocusToCommandBar()
{
    mCmdLineEdit->setFocusToCmd();
}

