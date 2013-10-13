#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "CPUWidget.h"
#include "CommandLineEdit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void executeCommand();
    void execStepOver();
    void execStepInto();
    void setFocusToCommandBar();
    
private:
    Ui::MainWindow *ui;
    QMdiArea* mdiArea;
    CPUWidget* mCpuWin;

    CommandLineEdit* mCmdLineEdit;
};

#endif // MAINWINDOW_H
