#ifndef HILOESCALAR_H
#define HILOESCALAR_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QDataStream>

class HiloEscalar : public QThread
{
    Q_OBJECT
public:
    QString matrizA;
    int escalar;
    int inicio;
    int fin;
    HiloEscalar(QString matrizA, int escalar, int inicio, int fin);

    void run();

signals:
    void triggerIncrementarEntradas();
};

#endif // HILOESCALAR_H
