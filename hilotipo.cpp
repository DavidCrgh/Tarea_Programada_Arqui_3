#include "hilotipo.h"


HiloTipo::HiloTipo(QString matrizA, QString mensajeTipo, bool esCuadrada){
    this->matrizA = matrizA;
    this->mensajeTipo = mensajeTipo;
    this->esCuadrada = esCuadrada;
}

void HiloTipo::run(){
    emit triggerActualizarMemoria(matrizA.size() + sizeof (bool));
    QFile archivoA(matrizA);

    if(!archivoA.open(QIODevice::ReadWrite)){
        return;
    }

    QDataStream inA(&archivoA);

    int filas;
    int columnas;

    int escalar;

    int entrada;

    bool esNula = true;
    bool esTriangularSuperior = esCuadrada;
    bool esTriangularInferior = esCuadrada;
    bool esDiagonal = esCuadrada;
    bool esEscalar = esCuadrada;
    bool esIdentidad = esCuadrada;

    int seguir;

    emit triggerActualizarMemoria(4 * sizeof (int)
                                  + 6 * sizeof (bool)
                                  + sizeof (QFile)
                                  + sizeof (QDataStream));

    inA >> filas;
    inA >> columnas;

    inA >> escalar;

    archivoA.seek(8); //Entrada 0,0 de la matriz

    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            inA >> entrada;
            //Nulidad
            if(entrada != 0){
                esNula &= false;
            }
            //Triangularidad Superior
            if(i > j && entrada != 0){
                esTriangularSuperior &= false;
            }
            //TriangularidadInferior
            if(i < j && entrada != 0){
                esTriangularInferior &= false;
            }
            //Diagonalidad no se revisa durante iteracion
            //Escalar
            if(i == j && entrada != escalar){
                esEscalar &= false;
            }
            //Identidad no se revisa durante iteracion
            emit triggerIncrementarEntradas();
            seguir = esNula | esTriangularSuperior | esTriangularInferior | esEscalar;
            if(!seguir){
                break;
            }
        }
        if(!seguir){
            break;
        }
    }
    esDiagonal = esTriangularSuperior & esTriangularInferior;
    esEscalar &= esDiagonal;
    esIdentidad = (esEscalar) & (escalar == 1);

    if(esNula){
        mensajeTipo += "    * Matriz Nula\n";
    }
    if(esTriangularSuperior){
        mensajeTipo += "    * Matriz Triangular Superior\n";
    }
    if(esTriangularInferior){
        mensajeTipo += "    * Matriz Triangular Inferior\n";
    }
    if(esDiagonal){
        mensajeTipo += "    * Matriz Diagonal\n";
    }
    if(esEscalar){
        mensajeTipo += "    * Matriz Escalar\n";
    }
    if(esIdentidad){
        mensajeTipo += "    * Matriz Identidad\n";
    }
    emit triggerActualizarMemoria(-(matrizA.size() + sizeof (bool)));
    emit triggerActualizarMemoria(-(4 * sizeof (int)
                                  + 6 * sizeof (bool)
                                  + sizeof (QFile)
                                  + sizeof (QDataStream)));
    emit finalizarRevision(mensajeTipo);
}
