#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "CPUWidget.h"
#include "LineEdit.h"

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
    void cmdLineEditKeyPressedSlot(int parKey);
    void execStepOver();
    void execStepInto();
    void setFocusToCommandBar();
    
private:
    Ui::MainWindow *ui;
    QMdiArea* mdiArea;
    CPUWidget* mCpuWin;

    LineEdit* mCmdLineEdit;

    QList<QString> mCmdHistory;
    int mCmdIndex;
};

#endif // MAINWINDOW_H
