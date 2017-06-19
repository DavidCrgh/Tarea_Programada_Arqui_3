#ifndef HILOTIPO_H
#define HILOTIPO_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QDataStream>

#include <QString>

class HiloTipo : public QThread
{
    Q_OBJECT
public:
    QString matrizA;
    QString mensajeTipo;
    bool esCuadrada;

    HiloTipo(QString matrizA, QString mensajeTipo, bool esCuadrada);

    void run();

signals:
    void triggerIncrementarEntradas();
    void finalizarRevision(QString);
    void triggerActualizarMemoria(int);
};

#endif // HILOTIPO_H
