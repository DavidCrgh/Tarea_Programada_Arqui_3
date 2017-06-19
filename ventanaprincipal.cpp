#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
    ventanaGenerar = new VentanaGenerar();
    connect(ui->seleccionadorOperacion, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cambiarOperacion(int)));
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
    pathResultado = "/home/davidcr/Desktop/Resultado.mtz";
    numeroHilos = 1;

    escalar = NULL;

    entradasAOperar = 0;
    entradasOperadas = 0;

    memoriaConsumida = 0;
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

void VentanaPrincipal::cambiarOperacion(int index)
{
    clearLayout(ui->contenedorControles);
    ui->botonOperar->disconnect();

    QVBoxLayout* contenedorVertical = new QVBoxLayout();
    QLabel* labelOperando = new QLabel("");
    QPushButton* botonAbrirB = new QPushButton("");
    QLineEdit* entradaEscalar = new QLineEdit;
    entradaEscalar->setValidator(new QIntValidator(this));

    ui->spinBox->setEnabled(true);

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
        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(sumarMatrices()));
        break;
    case 1: //Escalar
        labelOperando->setText("Escalar");

        contenedorVertical->addWidget(labelOperando);
        contenedorVertical->addWidget(entradaEscalar);

        ui->contenedorControles->addLayout(contenedorVertical);
        ui->botonOperar->setText("Multiplicar");

        escalar = entradaEscalar;
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

        connect(botonAbrirB, SIGNAL(clicked(bool)),
                this, SLOT(abrirArchivoMatrizB()));
        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(multiplicarMatrices()));
        break;
    case 3: //Transpuesta
        ui->botonOperar->setText("Transponer");
        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(transponerMatriz()));
        break;
    case 4: //Tipo
        ui->spinBox->setEnabled(false);
        ui->botonOperar->setText("Tipo Matriz");
        connect(ui->botonOperar, SIGNAL(clicked(bool)),
                this, SLOT(determinarTipo()));
        break;
    }
}

void VentanaPrincipal::sumarMatrices(){
    if(!(filasA == filasB) && !(columnasA == columnasB)){
        QMessageBox mensajeError;
        mensajeError.setText(
                    "Error: \n Las matrices deben ser de dimensiones iguales");
        mensajeError.exec();
        return;
    }

    if(QFile::exists(pathResultado)){
        QFile::remove(pathResultado);
    }
    QFile::copy(pathMatrizA, pathResultado);

    long totalEntradas = (filasA * columnasA);
    int entradasPorHilo = totalEntradas / numeroHilos;
    int entradasResiduo = totalEntradas - (entradasPorHilo * numeroHilos);

    entradasAOperar = totalEntradas;
    entradasOperadas = 0;

    int indice = 0;
    HiloSumar* hiloSumar;

    ui->botonOperar->setEnabled(false);
    ui->botonVerResultado->setEnabled(false);
    gettimeofday(&inicioOperacion, NULL);

    while(true){
        if(indice + entradasPorHilo + entradasResiduo == totalEntradas){
            hiloSumar = new HiloSumar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo + entradasResiduo - 1);
            connect(hiloSumar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloSumar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloSumar->start();
            break;
        } else {
            hiloSumar = new HiloSumar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo - 1);
            connect(hiloSumar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloSumar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloSumar->start();
        }

        indice += entradasPorHilo;
    }
}

void VentanaPrincipal::multiplicarEscalar(){
    if(QFile::exists(pathResultado)){
        QFile::remove(pathResultado);
    }
    QFile::copy(pathMatrizA, pathResultado);

    long totalEntradas = (filasA * columnasA);
    int entradasPorHilo = totalEntradas / numeroHilos;
    int entradasResiduo = totalEntradas - (entradasPorHilo * numeroHilos);

    entradasAOperar = totalEntradas;
    entradasOperadas = 0;

    int indice = 0;
    HiloEscalar* hiloEscalar;

    int valorEscalar = escalar->text().toInt();

    ui->botonOperar->setEnabled(false);
    gettimeofday(&inicioOperacion, NULL);

    while(true){
        if(indice + entradasPorHilo + entradasResiduo == totalEntradas){
            hiloEscalar = new HiloEscalar(pathMatrizA, valorEscalar,
                                      indice,
                                      indice + entradasPorHilo + entradasResiduo - 1);
            connect(hiloEscalar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloEscalar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloEscalar->start();
            break;
        } else {
            hiloEscalar = new HiloEscalar(pathMatrizA, valorEscalar,
                                      indice,
                                      indice + entradasPorHilo - 1);
            connect(hiloEscalar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloEscalar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloEscalar->start();
        }

        indice += entradasPorHilo;
    }
}

void VentanaPrincipal::multiplicarMatrices(){
    if(!(columnasA == filasB)){
        QMessageBox mensajeError;
        mensajeError.setText("Error:\n"
                             "La multiplicacion de matrices requiere que el numero"
                             "de columnas del primer operando sea igual al numero "
                             "de filas del segundo.\n\n"
                             "Nota: la multiplicacion de matrices NO es conmutativa.");
        mensajeError.exec();
    }

    if(QFile::exists(pathResultado)){
        QFile::remove(pathResultado);
    }
    QFile archivoResultado(pathResultado);
    if(!archivoResultado.open(QIODevice::ReadWrite)){
        return;
    }
    QDataStream out(&archivoResultado);

    out << (qint32) filasA;
    out << (qint32) columnasB;

    archivoResultado.close();

    long totalEntradas = (filasA * columnasB);
    int entradasPorHilo = totalEntradas / numeroHilos;
    int entradasResiduo = totalEntradas - (entradasPorHilo * numeroHilos);

    entradasAOperar = totalEntradas;
    entradasOperadas = 0;

    int indice = 0;
    HiloMultiplicar* hiloMultiplicar;

    ui->botonOperar->setEnabled(false);
    ui->botonVerResultado->setEnabled(false);
    gettimeofday(&inicioOperacion, NULL);

    while(true){
        if(indice + entradasPorHilo + entradasResiduo == totalEntradas){
            hiloMultiplicar = new HiloMultiplicar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo + entradasResiduo - 1);
            connect(hiloMultiplicar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloMultiplicar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloMultiplicar->start();
            break;
        } else {
            hiloMultiplicar = new HiloMultiplicar(pathMatrizA, pathMatrizB,
                                      indice,
                                      indice + entradasPorHilo - 1);
            connect(hiloMultiplicar, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloMultiplicar, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloMultiplicar->start();
        }

        indice += entradasPorHilo;
    }
}

void VentanaPrincipal::transponerMatriz(){
    if(QFile::exists(pathResultado)){
        QFile::remove(pathResultado);
    }
    QFile archivoResultado(pathResultado);

    if(!archivoResultado.open(QIODevice::ReadWrite)){
        return;
    }

    QDataStream out(&archivoResultado);

    out << (qint32) columnasA;
    out << (qint32) filasA;

    archivoResultado.close();

    long totalEntradas = (filasA * columnasA);
    int entradasPorHilo = totalEntradas / numeroHilos;
    int entradasResiduo = totalEntradas - (entradasPorHilo * numeroHilos);

    entradasAOperar = totalEntradas;
    entradasOperadas = 0;

    int indice = 0;
    HiloTransponer* hiloTransponer;

    ui->botonOperar->setEnabled(false);
    gettimeofday(&inicioOperacion, NULL);

    while(true){
        if(indice + entradasPorHilo + entradasResiduo == totalEntradas){
            hiloTransponer = new HiloTransponer(pathMatrizA,
                                      indice,
                                      indice + entradasPorHilo + entradasResiduo - 1);
            connect(hiloTransponer, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloTransponer, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloTransponer->start();
            break;
        } else {
            hiloTransponer = new HiloTransponer(pathMatrizA,
                                      indice,
                                      indice + entradasPorHilo - 1);
            connect(hiloTransponer, SIGNAL(triggerIncrementarEntradas()),
                    this, SLOT(incrementarEntradas()));
            connect(hiloTransponer, SIGNAL(triggerActualizarMemoria(int)),
                    this, SLOT(actualizarMemoria(int)));
            hiloTransponer->start();
        }

        indice += entradasPorHilo;
    }
}

void VentanaPrincipal::determinarTipo(){
    QString mensajeTipos = "La matriz tiene las siguientes caracteristicas:\n";
    bool esCuadrada;

    entradasAOperar = (filasA * columnasA);
    entradasOperadas = 0;

    ui->botonOperar->setEnabled(false);
    gettimeofday(&inicioOperacion, NULL);

    //Fila
    if(filasA == 1){
        mensajeTipos += "   * Matriz Fila\n";
    }
    if(columnasA == 1){
        mensajeTipos += "   * Matriz Columna\n";
    }
    if(filasA != columnasA){
        mensajeTipos += "   * Matriz Rectangular\n";
        esCuadrada = false;
    } else{
        mensajeTipos += "   * Matriz Cuadrada\n";
        esCuadrada = true;
    }

    HiloTipo* hiloTipo = new HiloTipo(pathMatrizA, mensajeTipos, esCuadrada);
    connect(hiloTipo, SIGNAL(triggerIncrementarEntradas()),
            this, SLOT(incrementarEntradas()));
    connect(hiloTipo, SIGNAL(triggerActualizarMemoria(int)),
            this, SLOT(actualizarMemoria(int)));
    connect(hiloTipo, SIGNAL(finalizarRevision(QString)),
            this, SLOT(finalizarRevisionTipo(QString)));
    hiloTipo->start();
}

void VentanaPrincipal::pintarMatriz(QString matriz){
    QFile archivo(matriz);

    if(!archivo.open(QIODevice::ReadWrite)){
        return; //Si falla en abrir no se hace nada
    }

    QDataStream in(&archivo);

    int n, m;
    in >> n;
    in >> m;

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
    QString pathArchivo = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz A"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
    if(pathArchivo == pathResultado){
        QMessageBox mensajeError;
        mensajeError.setText("Error:\n"
                             "No es posible seleccionar Resultado.mtz como\n"
                             "operando. Porfavor cambie el nombre si desea\n"
                             "usarlo.");
        mensajeError.exec();
    } else if(pathArchivo != ""){
        pathMatrizA = pathArchivo;
    }

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
            pintarMatriz(pathMatrizA);
        }
    }
}

void VentanaPrincipal::abrirArchivoMatrizB(){
    QString pathArchivo = QFileDialog::getOpenFileName(this,
                                               tr("Abrir Matriz B"),
                                               "/home/davidcr/Desktop",
                                               tr("Matrices (*.mtz)"));
    if(pathArchivo == pathResultado){
        QMessageBox mensajeError;
        mensajeError.setText("Error:\n"
                             "No es posible seleccionar Resultado.mtz como\n"
                             "operando. Porfavor cambie el nombre si desea\n"
                             "usarlo.");
        mensajeError.exec();
    } else if(pathArchivo != ""){
        pathMatrizB = pathArchivo;
    }

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
            pintarMatriz(pathMatrizB);
        }
    }
}

void VentanaPrincipal::on_botonGenerar_clicked()
{
    ventanaGenerar->show();
}

void VentanaPrincipal::recibirDatosMatriz(){
    DatosMatriz* datosMatriz = ventanaGenerar->datosMatriz;
    entradasAOperar = datosMatriz->numeroFilas * datosMatriz->numeroColumnas;
    entradasOperadas = 0;
    gettimeofday(&inicioOperacion, NULL);
    HiloGenerar* hilo = new HiloGenerar(datosMatriz);
    connect(hilo, SIGNAL(triggerIncrementarEntradas()),
            this, SLOT(incrementarEntradas()));
    hilo->start();
}

void VentanaPrincipal::mostrarTiempo(){
    double tiempo = ((finOperacion.tv_sec - inicioOperacion.tv_sec) * 1000) //Segundos
            + (finOperacion.tv_usec / 1000 - inicioOperacion.tv_usec / 1000); //Microsegundos
    ui->labelTiempo->setText("Tiempo de Ejecución: "
                             + QString::number(tiempo)
                             + " ms");
}

void VentanaPrincipal::incrementarEntradas(){
    entradasOperadas++;

    if(entradasOperadas == entradasAOperar){
        //Terminar de contar;
        gettimeofday(&finOperacion, NULL);
        mostrarTiempo();
        ui->botonOperar->setEnabled(true);
        ui->botonVerResultado->setEnabled(true);
    }
}

void VentanaPrincipal::actualizarMemoria(int bytes){
    memoriaConsumida += bytes;
    ui->labelMemoria->setText("Memoria Consumida: "
                              + QString::number(memoriaConsumida) + " bytes");
}

void VentanaPrincipal::finalizarRevisionTipo(QString mensaje){
    entradasOperadas = entradasAOperar - 1;
    incrementarEntradas();
    QMessageBox mensajeTipos;
    mensajeTipos.setText(mensaje);
    mensajeTipos.exec();
}

void VentanaPrincipal::on_spinBox_valueChanged(int arg1)
{
    numeroHilos = arg1;
}

void VentanaPrincipal::on_botonVerA_clicked()
{
    if(pathMatrizA != ""){
        pintarMatriz(pathMatrizA);
    }
}

void VentanaPrincipal::on_botonVerB_clicked()
{
    if(pathMatrizB != ""){
        pintarMatriz(pathMatrizB);
    }
}

void VentanaPrincipal::on_botonVerResultado_clicked()
{
    if(!QFile::exists(pathResultado)){
        return;
    }

    pintarMatriz(pathResultado);
}
