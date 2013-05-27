#include "MainWindow.h"
#include <QApplication>
#include <QtGui>
#include "CPUWidget.h"
#include "ColumnWidget.h"
#include "CustomTableView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //CPUSubWindow cpuWin;
    //cpuWin.show();

    //CPUWidget cpuWin;
    //cpuWin.show();

    ColumnWidget cw(5);
    cw.show();

    QWidget w;
    CustomTableView cust(&w);
    w.show();


/*
    QWidget fenetre;
    fenetre.setFixedSize(300, 150);

    // Création du bouton, ayant pour parent la "fenêtre"
    QPushButton bouton("Pimp mon bouton !", &fenetre);
    // Personnalisation du bouton
    bouton.setFont(QFont("Comic Sans MS", 14));
    bouton.setCursor(Qt::PointingHandCursor);
    bouton.setIcon(QIcon("smile.png"));

    // Affichage de la fenêtre
    fenetre.show();
*/

    return a.exec();
}
