#include "ventanagenerar.h"
#include "ui_ventanagenerar.h"

VentanaGenerar::VentanaGenerar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentanaGenerar)
{
    ui->setupUi(this);
    datosMatriz = new DatosMatriz();
}

VentanaGenerar::~VentanaGenerar()
{
    delete ui;
}


void VentanaGenerar::on_botonGenerar_clicked()
{
    datosMatriz->nombreArchivo = ui->entradaNombre->text();
    datosMatriz->numeroFilas = ui->entradaFilas->text().toInt();
    datosMatriz->numeroColumnas = ui->entradaColumnas->text().toInt();

    int indiceActual = ui->seleccionadorSignos->currentIndex();

    if(indiceActual == 0){
        datosMatriz->signos = 'P';
    } else if(indiceActual == 1){
        datosMatriz->signos = 'N';
    } else{
        datosMatriz->signos = 'A';
    }

    emit signalGenerar();
}

void VentanaGenerar::on_botonCancelar_clicked()
{
    this->close();
}
