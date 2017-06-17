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

    connect(ui->botonOperar, SIGNAL(clicked(bool)),
            this, SLOT(sumarMatrices()));

    pathMatrizA = "";
    pathMatrizB = "";
    numeroHilos = 1;

    entradasAOperar = 0;
    entradasOperadas = 0;
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

void VentanaPrincipal::sumarMatrices(){
    if(QFile::exists("/home/davidcr/Desktop/Resultado.mtz")){
        QFile::remove("/home/davidcr/Desktop/Resultado.mtz");
    }
    QFile::copy(pathMatrizA, "/home/davidcr/Desktop/Resultado.mtz");

    long totalEntradas = (filasA * columnasA);
    int entradasPorHilo = totalEntradas / numeroHilos;
    int entradasResiduo = totalEntradas - (entradasPorHilo * numeroHilos);

    entradasAOperar = totalEntradas;
    entradasOperadas = 0;

    int indice = 0;
    HiloSumar* hiloSumar;

    /*
     *  LARGE_INTEGER tiempo_in,tiempo_fin;
        double secs;
        QueryPerformanceCounter(&tiempo_in);
        multiplicarMatrices(orden, matrizA, matrizB, matrizAB);
        QueryPerformanceCounter(&tiempo_fin);
        secs = calcularTiempo(&tiempo_fin, &tiempo_in);
        std::stringstream str;
        str << fixed << setprecision( 20 ) << secs;
        std::cout<<str.str()<<endl;
        string tiem = "El tiempo fue de : "+str.str()+" s";*/

    gettimeofday(&inicioOperacion, NULL);

    while(true){
        if(indice + entradasPorHilo + entradasResiduo == totalEntradas){
            hiloSumar = new HiloSumar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo + entradasResiduo - 1);
            connect(hiloSumar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            hiloSumar->start();
            break;
        } else {
            hiloSumar = new HiloSumar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo - 1);
            connect(hiloSumar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            hiloSumar->start();
        }

        indice += entradasPorHilo;
    }
}

void VentanaPrincipal::multiplicarEscalar(){
    qDebug("Hola mundo!");
}

void VentanaPrincipal::multiplicarMatrices(){
    qDebug("!odnum aloH");
}

void VentanaPrincipal::pintarMatriz(QString matriz, int n, int m){
    QFile archivo(matriz);

    if(!archivo.open(QIODevice::ReadWrite)){
        return; //Si falla en abrir no se hace nada
    }

    QDataStream in(&archivo);

    archivo.seek(8); //Buscar byte donde empieza matriz

    QVBoxLayout* layoutTabla = new QVBoxLayout();
    QTableWidget* tabla = new QTableWidget(n,m);

    delete ui->scrollAreaWidgetContents->layout();

    int datoActual;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            in >> datoActual;
            tabla->setItem(i, j,
                           new QTableWidgetItem(QString::number(datoActual)));
        }
    }

    layoutTabla->addWidget(tabla);
    ui->scrollAreaWidgetContents->setLayout(layoutTabla);
}

void VentanaPrincipal::abrirArchivoMatrizA(){
    pathMatrizA = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz A"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
    if(pathMatrizA != ""){
        QFile archivo(pathMatrizA);

        if(archivo.open(QIODevice::ReadOnly)){
            QDataStream in(&archivo);

            in >> filasA;
            in >> columnasA;

            ui->labelDimensionesA->setText("A: "
                                           + QString::number(filasA)
                                           + " x "
                                           + QString::number(columnasA));
            pintarMatriz(pathMatrizA, filasA, columnasA);
        }
    }
}

void VentanaPrincipal::abrirArchivoMatrizB(){
    pathMatrizB = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz B"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
    if(pathMatrizB != ""){
        QFile archivo(pathMatrizB);

        if(archivo.open(QIODevice::ReadOnly)){
            QDataStream in(&archivo);

            in >> filasB;
            in >> columnasB;

            ui->labelDimensionesB->setText("B: "
                                           + QString::number(filasB)
                                           + " x "
                                           + QString::number(columnasB));
            pintarMatriz(pathMatrizB, filasB, columnasB);
        }
    }
}

void VentanaPrincipal::on_botonGenerar_clicked()
{
    ventanaGenerar->show();
}

void VentanaPrincipal::recibirDatosMatriz(){
    DatosMatriz* datosMatriz = ventanaGenerar->datosMatriz;
    qDebug("Datos recibidos");
    HiloGenerar* hilo = new HiloGenerar(datosMatriz);
    hilo->start();
}

void VentanaPrincipal::mostrarTiempo(){
    double tiempo = ((finOperacion.tv_sec - inicioOperacion.tv_sec) * 1000) //Segundos
            + (finOperacion.tv_usec / 1000 - inicioOperacion.tv_usec / 1000); //Microsegundos
    ui->labelTiempo->setText("Tiempo de Ejecución: "
                             + QString::number(tiempo / pow(10, 3))
                             + "s");
}

void VentanaPrincipal::incrementarEntradas(){
    entradasOperadas++;

    if(entradasOperadas == entradasAOperar){
        //Terminar de contar;
        gettimeofday(&finOperacion, NULL);
        mostrarTiempo();
        qDebug("Operacion terminada");
    }
}

void VentanaPrincipal::on_spinBox_valueChanged(int arg1)
{
    numeroHilos = arg1;
}

void VentanaPrincipal::on_botonVerA_clicked()
{
    if(pathMatrizA != ""){
        pintarMatriz(pathMatrizA, filasA, columnasA);
    }

}

void VentanaPrincipal::on_botonVerB_clicked()
{
    if(pathMatrizB != ""){
        pintarMatriz(pathMatrizB, filasB, columnasB);
    }
}
