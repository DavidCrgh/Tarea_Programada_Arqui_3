#include "hiloescalar.h"

HiloEscalar::HiloEscalar(QString matrizA, int escalar, int inicio, int fin)
{
    this->matrizA = matrizA;
    this->escalar = escalar;
    this->inicio = inicio;
    this->fin = fin;
}

void HiloEscalar::run(){
    QMutex mutex;

    mutex.lock();
    QFile archivoA(matrizA);
    QFile archivoResultado("/home/davidcr/Desktop/Resultado.mtz");
    mutex.unlock();

    mutex.lock();
    if(!archivoA.open(QIODevice::ReadWrite)
            & !archivoResultado.open(QIODevice::ReadWrite)){
        return;
    }
    archivoA.seek((2 + inicio) * 4);
    archivoResultado.seek((2 + inicio) * 4);
    mutex.unlock();

    mutex.lock();
    QDataStream inA(&archivoA);
    QDataStream outResultado(&archivoResultado);
    mutex.unlock();

    int entradaA = 0;

    int entradaResultado = 0;

    for(int i = 0; i <= (fin - inicio); i++){
        mutex.lock();
        inA >> entradaA;
        entradaResultado = entradaA * escalar; //TODO Aqui se invoca a proc ASM
        outResultado << (qint32) entradaResultado;
        mutex.unlock();
        emit triggerIncrementarEntradas();
    }
}
