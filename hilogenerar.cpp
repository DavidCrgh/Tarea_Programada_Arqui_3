#include "hilogenerar.h"

HiloGenerar::HiloGenerar(DatosMatriz* pDatosMatriz)
{
    datosMatriz = pDatosMatriz;
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
}

void HiloGenerar::run(){
    //QFile archivo(datosMatriz->nombreArchivo + ".txt");

    /*if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QDataStream out(&archivo);*/

    for(int i = 0; i < datosMatriz->numeroFilas; i++){
        for(int j = 0; j < datosMatriz->numeroColumnas; j++){
            qDebug("%d,%d %d", i, j, generarAleatorio(datosMatriz->signos));
            //out << (qint32) generarAleatorio();
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
