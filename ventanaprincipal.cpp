#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
    ventanaGenerar = new VentanaGenerar();
    connect(ventanaGenerar, SIGNAL(signalGenerar()),
            this, SLOT(recibirDatosMatriz()));

    connect(ui->botonAbrirA, SIGNAL(clicked(bool)),
            this, SLOT(abrirArchivoMatrizA()));
    connect(ui->botonAbrirB, SIGNAL(clicked(bool)),
            this, SLOT(abrirArchivoMatrizB()));

    pathMatrizA = "";
    pathMatrizB = "";
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
    ui->botonOperar->disconnect();

    QVBoxLayout* contenedorVertical = new QVBoxLayout();

    QLabel* labelOperando = new QLabel("");

    QPushButton* botonAbrirB = new QPushButton("");

    QLineEdit* entradaEscalar = new QLineEdit;
    entradaEscalar->setValidator(new QIntValidator(this));

    switch(index){
    case 0: // Suma
        labelOperando->setText("Matriz B");
        botonAbrirB->setText("Abrir Archivo B");

        contenedorVertical->addWidget(labelOperando);
        contenedorVertical->addWidget(botonAbrirB);

        ui->contenedorControles->addLayout(contenedorVertical);
        ui->botonOperar->setText("Sumar");

        connect(botonAbrirB, SIGNAL(clicked(bool)),
                this, SLOT(abrirArchivoMatrizB()));
        break;
    case 1: //Escalar
        labelOperando->setText("Escalar");

        contenedorVertical->addWidget(labelOperando);
        contenedorVertical->addWidget(entradaEscalar);

        ui->contenedorControles->addLayout(contenedorVertical);
        ui->botonOperar->setText("Multiplicar");

        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(multiplicarEscalar()));
        break;
    case 2: //Producto
        labelOperando->setText("Matriz B");
        botonAbrirB->setText("Abrir Archivo B");

        contenedorVertical->addWidget(labelOperando);
        contenedorVertical->addWidget(botonAbrirB);

        ui->contenedorControles->addLayout(contenedorVertical);
        ui->botonOperar->setText("Multiplicar");

        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(multiplicarMatrices()));
        break;
    case 3: //Inversa
        ui->botonOperar->setText("Invertir");
        break;
    case 4: //Tipo
        ui->botonOperar->setText("Tipo Matriz");
        break;
    }
}

void VentanaPrincipal::multiplicarEscalar(){
    qDebug("Hola mundo!");
}

void VentanaPrincipal::multiplicarMatrices(){
    qDebug("!odnum aloH");
}

void VentanaPrincipal::abrirArchivoMatrizA(){
    pathMatrizA = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz A"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
}

void VentanaPrincipal::abrirArchivoMatrizB(){
    pathMatrizB = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz B"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
}

void VentanaPrincipal::on_botonGenerar_clicked()
{
    ventanaGenerar->show();
}

void VentanaPrincipal::recibirDatosMatriz(){
    DatosMatriz* datosMatriz = ventanaGenerar->datosMatriz;
    qDebug("Datos recibidos");
    HiloGenerar* hilo = new HiloGenerar(datosMatriz);
    hilo->run();
}
