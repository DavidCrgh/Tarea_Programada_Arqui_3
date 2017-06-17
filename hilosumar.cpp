#include "hilosumar.h"

HiloSumar::HiloSumar(QString matrizA, QString matrizB, int inicio, int fin)
{
    this->matrizA = matrizA;
    this->matrizB = matrizB;
    this->inicio = inicio;
    this->fin = fin;
}

void HiloSumar::run(){
    QMutex mutex;

    mutex.lock();
    QFile archivoA(matrizA);
    QFile archivoB(matrizB);
    QFile archivoResultado("/home/davidcr/Desktop/Resultado.mtz");
    mutex.unlock();

    mutex.lock();
    if(!archivoA.open(QIODevice::ReadWrite)
            & !archivoB.open(QIODevice::ReadWrite)
            & !archivoResultado.open(QIODevice::ReadWrite)){
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

    for(int i = 0; i <= (fin - inicio); i++){
        mutex.lock();
        inA >> entradaA;
        inB >> entradaB;
        entradaResultado = entradaA + entradaB;
        outResultado << (qint32) entradaResultado;
        mutex.unlock();
        emit triggerIncrementarEntradas();
    }
}
