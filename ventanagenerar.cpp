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
    emit signalGenerar();
}

void VentanaGenerar::on_botonCancelar_clicked()
{
    this->close();
}
