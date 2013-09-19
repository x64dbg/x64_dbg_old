#include "MainWindow.h"
#include <QApplication>
#include <QtGui>
#include "CPUWidget.h"

#include "MapViewOfMem.h"
#include "QBeaEngine.h"
#include "Disassembly.h"
//#include "StdTable.h"
#include "HexDump.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    MainWindow w;
    w.show();
*/

    //CPUSubWindow cpuWin;
    //cpuWin.show();

    //CPUWidget cpuWin;
    //cpuWin.show();


    //QBeaEngine disasm;

/*
    HexDump hexDump(0);
    hexDump.callVirtual();
    hexDump.resize(QSize(600,600));
    hexDump.show();
*/

    Disassembly d(0);
    d.resize(QSize(600,600));
    d.show();




/*
    StdTable table(0);
    table.resize(QSize(600,600));
    table.show();
*/
/*
    MapViewOfMem mem("AsmCode.bin");
    DisassemblyView dis(mem,0);
    dis.resize(QSize(600,600));
    dis.show();
    */

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
