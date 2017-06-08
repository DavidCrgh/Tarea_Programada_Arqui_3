#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}


void clearLayout(QLayout *layout){
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        //delete item;
    }
}


void VentanaPrincipal::on_seleccionadorOperacion_currentIndexChanged(int index)
{
    clearLayout(ui->contenedorControles);

    QVBoxLayout* contenedorVertical = new QVBoxLayout();

    QLabel* labelMatrizB = new QLabel("");
    QPushButton* botonAbrirB = new QPushButton("");

    switch(index){
    case 0: // Suma
        labelMatrizB->setText("Matriz B");
        botonAbrirB->setText("Abrir Archivo B");

        contenedorVertical->addWidget(labelMatrizB);
        contenedorVertical->addWidget(botonAbrirB);

        ui->contenedorControles->addLayout(contenedorVertical);
        break;
    case 1: //Escalar
        //labelMatrizB->
        break;
    case 2: //Producto
        break;
    case 3: //Inversa
        break;
    case 4: //Tipo
        break;
    }
}


