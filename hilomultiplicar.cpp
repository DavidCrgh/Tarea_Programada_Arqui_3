#include "hilomultiplicar.h"

HiloMultiplicar::HiloMultiplicar(QString matrizA, QString matrizB, int inicio, int fin)
{
    this->matrizA = matrizA;
    this->matrizB = matrizB;
    this->inicio = inicio;
    this->fin = fin;
}

int HiloMultiplicar::pasarR2toR(int i, int j, int numColumnas){
    return i * numColumnas + j;
}

QList<int> HiloMultiplicar::pasarRtoR2(int i, int numColumnas){
    QList<int> par;
    par.append(i / numColumnas);
    par.append(i % numColumnas);

    return par;
}

void HiloMultiplicar::run(){
    //Bytes consumidos por atributos del hilo
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
    mutex.unlock();

    int filasA;
    int columnasA;

    int filasB;
    int columnasB;

    mutex.lock();
    QDataStream inA(&archivoA);
    QDataStream inB(&archivoB);
    QDataStream outResultado(&archivoResultado);

    inA >> filasA;
    inA >> columnasA;

    inB >> filasB;
    inB >> columnasB;
    mutex.unlock();

    int entradaA;
    int entradaB;

    int entradaResultado;
    QList<int> parEntradaActual;

    emit triggerActualizarMemoria(9 * sizeof (int)
                                  + sizeof (QMutex)
                                  + (sizeof(QFile) + sizeof (QDataStream)) * 3);
    //Nueve ints consumidos metodo run + los objetos de IO y concurrencia

    archivoResultado.seek((2 + inicio) * 4);

    for(int i = inicio; i <= fin; i++){
        parEntradaActual = pasarRtoR2(i, columnasB); // i : at(0) j : at(1)

        entradaResultado = 0;
        for(int k = 0; k < columnasA; k++){
            mutex.lock();
            archivoA.seek((2 + pasarR2toR(parEntradaActual.at(0), k, columnasA)) * 4);
            archivoB.seek((2 + pasarR2toR(k, parEntradaActual.at(1), columnasB)) * 4);

            inA >> entradaA;
            inB >> entradaB;
            mutex.unlock();

            entradaResultado += entradaA * entradaB;
        }
        mutex.lock();
        outResultado << (qint32) entradaResultado;
        mutex.unlock();

        emit triggerIncrementarEntradas();
    }
    //Cuando termina el hilo se limpia la memoria usada
    emit triggerActualizarMemoria(-(matrizA.size() + matrizB.size() + 2 * 4));
    emit triggerActualizarMemoria(-(9 * sizeof (int)
                                    + sizeof (QMutex)
                                    + (sizeof(QFile) + sizeof (QDataStream)) * 3));
}
