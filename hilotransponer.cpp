#include "hilotransponer.h"

HiloTransponer::HiloTransponer(QString matrizA, int inicio, int fin){
    this->matrizA = matrizA;
    this->inicio = inicio;
    this->fin = fin;
}

int HiloTransponer::pasarR2toR(int i, int j, int numColumnas){
    return i * numColumnas + j;
}

QList<int> HiloTransponer::pasarRtoR2(int i, int numColumnas){
    QList<int> par;
    par.append(i / numColumnas);
    par.append(i % numColumnas);

    return par;
}

void HiloTransponer::run(){
    emit triggerActualizarMemoria(matrizA.size() + 2 * sizeof (int));
    QMutex mutex;

    mutex.lock();
    QFile archivoA(matrizA);
    QFile archivoResultado("/home/davidcr/Desktop/Resultado.mtz");
    mutex.unlock();

    mutex.lock();
    if(!archivoA.open(QIODevice::ReadWrite)
            | !archivoResultado.open(QIODevice::ReadWrite)){
        return;
    }
    mutex.unlock();

    int filas;
    int columnas;

    mutex.lock();
    QDataStream inA(&archivoA);
    QDataStream outResultado(&archivoResultado);

    inA >> filas;
    inA >> columnas;

    archivoA.seek((2 + inicio) * 4);
    archivoResultado.seek((2 + inicio) * 4);
    mutex.unlock();

    QList<int> parActual;
    int entradaResultado;

    emit triggerActualizarMemoria(5 * sizeof (int)
                                  + 2 * sizeof (QFile)
                                  + 2 * sizeof (QDataStream)
                                  + sizeof(QMutex));

    for(int i = inicio; i <= fin; i++){
        mutex.lock();
        parActual = pasarRtoR2(i, filas);
        archivoA.seek((2 + pasarR2toR(parActual.at(1), parActual.at(0), columnas)) * 4);
        inA >> entradaResultado;
        outResultado << (qint32) entradaResultado;
        mutex.unlock();
        emit triggerIncrementarEntradas();
    }

    archivoA.close();
    archivoResultado.close();
    emit triggerActualizarMemoria(-(matrizA.size() + 2 * sizeof (int)));
    emit triggerActualizarMemoria(-(5 * sizeof (int)
                                  + 2 * sizeof (QFile)
                                  + 2 * sizeof (QDataStream)
                                  + sizeof(QMutex)));
}
