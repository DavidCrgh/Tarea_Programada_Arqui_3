#ifndef HILOMULTIPLICAR_H
#define HILOMULTIPLICAR_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QDataStream>

#include <QList>

class HiloMultiplicar : public QThread
{
    Q_OBJECT
public:
    QString matrizA;
    QString matrizB;
    int inicio;
    int fin;

    HiloMultiplicar(QString matrizA, QString matrizB, int inicio, int fin);

    void run();
    int pasarR2toR(int i, int j, int numColumnas);
    QList<int> pasarRtoR2(int i, int numColumnas);

signals:
    void triggerIncrementarEntradas();
    void triggerActualizarMemoria(int);
};

#endif // HILOMULTIPLICAR_H
