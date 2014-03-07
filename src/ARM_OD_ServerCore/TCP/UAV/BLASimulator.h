#ifndef BLASIMULATOR_H
#define BLASIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QDataStream>
#include <QByteArray>
#include <math.h>

#include "../../Common/Router.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"


////��������� ��� �������� ������
//struct DataFromRadioLocation
//{
//    //������ ���������
//    QVector<QPointF> coordLatLon;
//    //������ �����
//    QVector<double> heigh;
//    //������ ��� ���������
//    QVector<double> coordStdDev;
//    //������ ������� ��������� ���������
//    QVector<QTime> timeHMSMs;
//    //������ �������
//    QVector<double> airspeed;
//    //������ ��� ��������
//    QVector<double> airSpeedStdDev;
//    //������ �������� ������
//    QVector<double> relativeBearing;
//    //������ ��������� ��������
//    QVector<State> StateMassive_;
//};

class BLASimulator : public QObject
{
    Q_OBJECT
public:
    BLASimulator(IRouter* router, double radius, bool red);
    ~BLASimulator();

public:
    void start();
    void stop();

private slots:
    void _slot_simulation();

private:
    int             _id;
    QTimer*         _timer;
    IRouter*        _router;
    ISubscriber*    _subscriber;

    double          _radius;
    bool            _red;
};

#endif // BLASIMULATOR_H
