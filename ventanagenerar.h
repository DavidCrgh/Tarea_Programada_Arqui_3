#ifndef VENTANAGENERAR_H
#define VENTANAGENERAR_H

#include <QWidget>
#include <QObject>

#include "datosmatriz.h"

namespace Ui {
class VentanaGenerar;
}

class VentanaGenerar : public QWidget
{
    Q_OBJECT

public:
    DatosMatriz* datosMatriz;

    explicit VentanaGenerar(QWidget *parent = 0);
    ~VentanaGenerar();

    void iniciarEdicion();

signals:
    void signalGenerar();

private slots:
    void on_botonGenerar_clicked();

    void on_botonCancelar_clicked();

private:
    Ui::VentanaGenerar *ui;
};

#endif // VENTANAGENERAR_H
