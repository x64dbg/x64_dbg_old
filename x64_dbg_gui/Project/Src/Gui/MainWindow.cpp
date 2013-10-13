#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    mCmdHistory.clear();
    mCmdIndex = -1;

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
    mCmdLineEdit = new LineEdit(ui->cmdBar);
    ui->cmdBar->addWidget(new QLabel("Command "));
    ui->cmdBar->addWidget(mCmdLineEdit);


    // Setup Signals/Slots
    connect(ui->actionStepOver, SIGNAL(triggered()), mCpuWin, SLOT(stepOverSlot()));
    connect(mCmdLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(mCmdLineEdit, SIGNAL(keyPressed(int)), this, SLOT(cmdLineEditKeyPressedSlot(int)));
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

    // Add to history
    mCmdHistory.prepend(wCmd);

    if(mCmdHistory.size() > 32)
        mCmdHistory.removeLast();

    qDebug() << wCmd << " has been added to the history !";

    Bridge::getBridge()->execCmd(reinterpret_cast<const char*>(wCmd.toAscii().data()));

    mCmdLineEdit->setText("");
    mCmdIndex = -1;
}


void MainWindow::cmdLineEditKeyPressedSlot(int parKey)
{
    if(parKey == Qt::Key_Up || parKey == Qt::Key_Down)
    {
        if(parKey == Qt::Key_Up)
            mCmdIndex++;
        else if(parKey == Qt::Key_Down)
            mCmdIndex--;

        mCmdIndex = mCmdIndex < -1 ? -1 : mCmdIndex;
        mCmdIndex = mCmdIndex > mCmdHistory.size() - 1 ? mCmdHistory.size() - 1 : mCmdIndex;

        if(mCmdIndex == -1)
        {
            mCmdLineEdit->setText("");
        }
        else
        {
            mCmdLineEdit->setText(mCmdHistory.at(mCmdIndex));
        }


    }
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
    mCmdIndex = -1;
    mCmdLineEdit->setFocus();
}

