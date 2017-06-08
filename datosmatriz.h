#ifndef DATOSMATRIZ_H
#define DATOSMATRIZ_H

#include <QString>

class DatosMatriz
{
public:
    //Atributos
    QString nombreArchivo;
    int numeroFilas;
    int numeroColumnas;
    char signos; //P: Positivos, N: negativos, A: ambos

    //Constructores
    DatosMatriz();

};

#endif // DATOSMATRIZ_H
