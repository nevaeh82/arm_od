#ifndef BPLASIMULATOR_H
#define BPLASIMULATOR_H

#include <QObject>
#include <QTimer>
#include <qmath.h>

#include "../Tabs/DbBla/Defines.h"

class BplaSimulator : public QObject
{
    Q_OBJECT

public:
    BplaSimulator(QObject* parent = NULL);
    ~BplaSimulator();
    void startSim();
    void stopSim();

private:
    QTimer* m_timer;
    UavInfo uav;
    double m_lat;
    double m_lon;
    double df;
    double r;

private slots:
    void slotNewPoint();

signals:
    void signalGenNewPoint(double, double);
};

#endif // BPLASIMULATOR_H
