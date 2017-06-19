#ifndef HILOGENERAR_H
#define HILOGENERAR_H

#include <QObject>
#include <QThread>

#include <QIODevice>
#include <QFile>
#include <QDataStream>

#include <stdlib.h>
#include <sys/time.h>

#include "datosmatriz.h"

class HiloGenerar : public QThread
{
    Q_OBJECT
public:
    //Atributos
    DatosMatriz* datosMatriz;

    //Constructores
    HiloGenerar(DatosMatriz* pDatosMatriz);

    //Metodos
    void run();
    int generarAleatorio(char signo);

signals:
    void triggerIncrementarEntradas();
};

#endif // HILOGENERAR_H
