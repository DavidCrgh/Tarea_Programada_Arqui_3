#include "hilogenerar.h"

HiloGenerar::HiloGenerar(DatosMatriz* pDatosMatriz)
{
    datosMatriz = pDatosMatriz;
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
}

void HiloGenerar::run(){
    //QFile archivo("/home/davidcr/Desktop/mat");
    QFile archivo("/home/davidcr/Desktop/" + datosMatriz->nombreArchivo + ".mtz");

    if (!archivo.open(QIODevice::WriteOnly)){
        return;
    }

    QDataStream salidaBytes(&archivo);


    salidaBytes << (qint32) datosMatriz->numeroFilas;
    salidaBytes << (qint32) datosMatriz->numeroColumnas;
    //int d;
    for(int i = 0; i < datosMatriz->numeroFilas; i++){
        for(int j = 0; j < datosMatriz->numeroColumnas; j++){
            //d = generarAleatorio(datosMatriz->signos);
            //qDebug("(%d,%d) : %d",i,j,d);
            salidaBytes << (qint32) generarAleatorio(datosMatriz->signos);
        }
    }

    /*archivo.seek(3999996); //
    QDataStream in(archivo.read(4)); //LEER POSICION ESPECIFICA

    int dato;

    in >> dato;

    qDebug("%d",dato);*/
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
