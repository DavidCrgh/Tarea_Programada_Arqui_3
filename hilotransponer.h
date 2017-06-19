#ifndef HILOTRANSPONER_H
#define HILOTRANSPONER_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QDataStream>

class HiloTransponer : public QThread
{
    Q_OBJECT
public:
    QString matrizA;
    int inicio;
    int fin;

    HiloTransponer(QString matrizA, int inicio, int fin);
    void run();
    int pasarR2toR(int i, int j, int numColumnas);
    QList<int> pasarRtoR2(int i, int numColumnas);

signals:
    void triggerIncrementarEntradas();
    void triggerActualizarMemoria(int);
};

#endif // HILOTRANSPONER_H
