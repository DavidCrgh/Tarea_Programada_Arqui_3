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
#include <QMessageBox>

#include <sys/time.h>

#include "ventanagenerar.h"
#include "datosmatriz.h"

#include "hilogenerar.h"
#include "hilosumar.h"
#include "hiloescalar.h"
#include "hilomultiplicar.h"
#include "hilotransponer.h"
#include "hilotipo.h"

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
    void cambiarOperacion(int index);

    void sumarMatrices();
    void multiplicarEscalar();
    void multiplicarMatrices();
    void transponerMatriz();
    void determinarTipo();

    void pintarMatriz(QString matriz);

    void abrirArchivoMatrizA();
    void abrirArchivoMatrizB();

    void on_botonGenerar_clicked();

    void recibirDatosMatriz();
    void incrementarEntradas();
    void actualizarMemoria(int bytes);
    void finalizarRevisionTipo(QString mensaje);

    void mostrarTiempo();

    void on_spinBox_valueChanged(int arg1);

    void on_botonVerA_clicked();

    void on_botonVerB_clicked();

    void on_botonVerResultado_clicked();

private:
    Ui::VentanaPrincipal *ui;
    VentanaGenerar* ventanaGenerar;

    QString pathMatrizA;
    QString pathMatrizB;
    QString pathResultado;
    int filasA;
    int columnasA;
    int filasB;
    int columnasB;

    int numeroHilos;

    QLineEdit* escalar;

    long entradasAOperar;
    long entradasOperadas;

    struct timeval inicioOperacion;
    struct timeval finOperacion;

    long memoriaConsumida;

    //HiloGenerar* hiloGenerar;
};

#endif // VENTANAPRINCIPAL_H
