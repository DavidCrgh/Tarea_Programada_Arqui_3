#ifndef HILOSUMAR_H
#define HILOSUMAR_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QDataStream>

class HiloSumar : public QThread
{
    Q_OBJECT
public:
    QString matrizA;
    QString matrizB;
    int inicio;
    int fin;

    HiloSumar(QString matrizA, QString matrizB, int inicio, int fin);

    void run();

signals:
    void triggerIncrementarEntradas();
};

#endif // HILOSUMAR_H
