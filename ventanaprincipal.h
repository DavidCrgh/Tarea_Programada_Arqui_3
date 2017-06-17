#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QtWidgets>
#include <QObject>
#include <QIODevice>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QList>

#include "ventanagenerar.h"
#include "datosmatriz.h"

#include "hilogenerar.h"
#include "hilosumar.h"

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

private slots:
    void on_seleccionadorOperacion_currentIndexChanged(int index);

    void sumarMatrices();
    void multiplicarEscalar();
    void multiplicarMatrices();

    void pintarMatriz(QString matriz, int n, int m);

    void abrirArchivoMatrizA();
    void abrirArchivoMatrizB();

    void on_botonGenerar_clicked();

    void recibirDatosMatriz();
    void incrementarEntradas();

    void on_spinBox_valueChanged(int arg1);

    void on_botonVerA_clicked();

    void on_botonVerB_clicked();

private:
    Ui::VentanaPrincipal *ui;
    VentanaGenerar* ventanaGenerar;
    QString pathMatrizA;
    QString pathMatrizB;
    int filasA;
    int columnasA;
    int filasB;
    int columnasB;
    int numeroHilos;

    long entradasAOperar;
    long entradasOperadas;
    //HiloGenerar* hiloGenerar;
};

#endif // VENTANAPRINCIPAL_H
