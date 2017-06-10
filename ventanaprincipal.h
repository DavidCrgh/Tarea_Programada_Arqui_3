#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QtWidgets>
#include <QObject>

#include "ventanagenerar.h"
#include "datosmatriz.h"
#include "hilogenerar.h"

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

    void multiplicarEscalar();
    void multiplicarMatrices();

    void on_botonGenerar_clicked();

    void recibirDatosMatriz();

private:
    Ui::VentanaPrincipal *ui;
    VentanaGenerar* ventanaGenerar;
    //HiloGenerar* hiloGenerar;
};

#endif // VENTANAPRINCIPAL_H
