#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    // Memory Map View
    mMemMapView = new QMdiSubWindow();
    mMemMapView->setWindowTitle("Memory Map");
    mMemMapView->setWidget(new MemoryMapView());
    mMemMapView->hide();

    // Log View
    mLogView = new QMdiSubWindow();
    mLogView->setWindowTitle("Log");
    mLogView->setWidget(new LogView());
    mLogView->hide();


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
    mdiArea->addSubWindow(mMemMapView);
    mdiArea->addSubWindow(mLogView);


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
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(execClose()));
    connect(ui->actionMemoryMap,SIGNAL(triggered()),this,SLOT(displayMemMapWidget()));
    connect(ui->actionRun,SIGNAL(triggered()),this,SLOT(execRun()));
    connect(ui->actionRtr,SIGNAL(triggered()),this,SLOT(execRtr()));
    connect(ui->actionLog,SIGNAL(triggered()),this,SLOT(displayLogWidget()));

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
    Bridge::getBridge()->execCmd("StepOver");
}

void MainWindow::execStepInto()
{
    Bridge::getBridge()->execCmd("StepInto");
}

void MainWindow::setFocusToCommandBar()
{
    mCmdLineEdit->setFocusToCmd();
}

void MainWindow::execClose()
{
    Bridge::getBridge()->execCmd("stop");
}

void MainWindow::execRun()
{
    Bridge::getBridge()->execCmd("run");
}

void MainWindow::execRtr()
{
    Bridge::getBridge()->execCmd("rtr");
}


void MainWindow::displayMemMapWidget()
{
    mMemMapView->widget()->show();
    mMemMapView->setFocus();
}

void MainWindow::displayLogWidget()
{
    mLogView->widget()->show();
    mLogView->setFocus();
}
