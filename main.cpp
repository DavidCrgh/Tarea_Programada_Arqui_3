#include "ventanaprincipal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VentanaPrincipal w;
    w.show();

    /*struct timeval start, end;
    gettimeofday(&start,NULL);
    for(int i =0; i < 100000; i++){
        qDebug("Hola mundo!");
    }
    gettimeofday(&end, NULL);

    qDebug("Listo");*/

    return a.exec();
}
