#include "hilogenerar.h"

HiloGenerar::HiloGenerar(DatosMatriz* pDatosMatriz)
{
    datosMatriz = pDatosMatriz;
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
}

void HiloGenerar::run(){
    QString nombreArchivo = "/home/davidcr/Desktop/"
            + datosMatriz->nombreArchivo + ".mtz";

    if(QFile::exists(nombreArchivo)){
        QFile::remove(nombreArchivo);
    }

    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly)){
        return;
    }

    QDataStream salidaBytes(&archivo);


    salidaBytes << (qint32) datosMatriz->numeroFilas;
    salidaBytes << (qint32) datosMatriz->numeroColumnas;

    for(int i = 0; i < datosMatriz->numeroFilas; i++){
        for(int j = 0; j < datosMatriz->numeroColumnas; j++){
            salidaBytes << (qint32) generarAleatorio(datosMatriz->signos);
            emit triggerIncrementarEntradas();
        }
    }
}

int HiloGenerar::generarAleatorio(char signo){
    //rand() % 101 pertenece a [0,100]
    switch (signo) {
    case 'A':
        //rand() % 2 = {0 : Positivo, 1 : Negativo}
        if(rand() % 2){ //Negativo
            return (rand() % 101) * -1;
        } else{ //Positivo
            return rand() % 101;
        }
        break;
    case 'P':
        return rand() % 101;
        break;
    case 'N':
        return (rand() % 101) * -1;
        break;
    }
}
