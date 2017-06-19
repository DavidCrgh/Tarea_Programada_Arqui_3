#include "hilosumar.h"

HiloSumar::HiloSumar(QString matrizA, QString matrizB, int inicio, int fin)
{
    this->matrizA = matrizA;
    this->matrizB = matrizB;
    this->inicio = inicio;
    this->fin = fin;
}

void HiloSumar::run(){
    emit triggerActualizarMemoria(matrizA.size() + matrizB.size() + 2 * 4);
    QMutex mutex;

    mutex.lock();
    QFile archivoA(matrizA);
    QFile archivoB(matrizB);
    QFile archivoResultado("/home/davidcr/Desktop/Resultado.mtz");
    mutex.unlock();

    mutex.lock();
    if(!archivoA.open(QIODevice::ReadWrite)
            || !archivoB.open(QIODevice::ReadWrite)
            || !archivoResultado.open(QIODevice::ReadWrite)){
        return;
    }
    archivoA.seek((2 + inicio) * 4);
    archivoB.seek((2 + inicio) * 4);
    archivoResultado.seek((2 + inicio) * 4);
    mutex.unlock();

    mutex.lock();
    QDataStream inA(&archivoA);
    QDataStream inB(&archivoB);
    QDataStream outResultado(&archivoResultado);
    mutex.unlock();

    int entradaA = 0;
    int entradaB = 0;

    int entradaResultado = 0;

    emit triggerActualizarMemoria(3 * sizeof(int)
                                  + 3 * (sizeof(QFile) + sizeof(QDataStream))
                                  + sizeof(QMutex));

    for(int i = 0; i <= (fin - inicio); i++){
        mutex.lock();
        inA >> entradaA;
        inB >> entradaB;
        entradaResultado = entradaA + entradaB;
        outResultado << (qint32) entradaResultado;
        emit triggerIncrementarEntradas();
        mutex.unlock();
    }
    archivoA.close();
    archivoB.close();
    archivoResultado.close();
    emit triggerActualizarMemoria(-(matrizA.size() + matrizB.size() + 2 * 4));
    emit triggerActualizarMemoria(-(3 * sizeof(int)
                                  + 3 * (sizeof(QFile) + sizeof(QDataStream))
                                  + sizeof(QMutex)));
}
