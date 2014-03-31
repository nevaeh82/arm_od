#include "BLASimulator.h"
//#define _USE_MATH_DEFINES

BLASimulator::BLASimulator(IRouter* router, double radius, bool red) :
    _id(777),
    _radius(radius),
    _red(red)
{
    _router = router;
    _subscriber = _router->get_subscriber();
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(_slot_simulation()));
}

BLASimulator::~BLASimulator()
{
    delete _timer;
}

void BLASimulator::start()
{
    _timer->start(2000);
}

void BLASimulator::stop()
{
    _timer->stop();
}

void BLASimulator::_slot_simulation()
{
    static double angle = 0;

    static double alt = 1000;
    QPointF point;

    if(angle >= 360)
    {
        angle = 0;
    }

    if(angle > 100 && angle < 200)
    {
        alt = 1500;
    }
    else
    {
        alt = 1000;
    }

    double lon = cos((180 - angle) * M_PI / 180) * _radius + 41.6905;
    double lat = sin((180 - angle) * M_PI / 180) * _radius + 42.511183;
    point.setY(lon);
    point.setX(lat);
    angle += 5;


    if(_red)
    {
        QByteArray *ba = new QByteArray();
        QDataStream ds(ba, QIODevice::ReadWrite);
        ds << point;
        ds << alt;

        QSharedPointer<IMessageOld> msg(new MessageOld(_id, KTR_BLA, ba));
        _subscriber->data_ready(KTR_BLA, msg);
    }
    else
    {
        QVector<QPointF> points;
        points.append(point);
        QVector<QPointF> stdPoints;
        QPointF stdPoint;
        stdPoint.setX(999);
        stdPoint.setY(999);
        stdPoints.append(stdPoint);
        QByteArray *ba = new QByteArray();
        QDataStream ds(ba, QIODevice::ReadWrite);
        ds << 0;//aData.timeHMSMs.at(aLastItem);
        ds << 1/*aData.StateMassive_.at(aLastItem)*/;
        ds << stdPoint;
        ds << points;//aData.coordLatLon.at(aLastItem);
        double speed = 100;
        ds << speed;//aData.airspeed.at(aLastItem);
        double alt = 1000;
        ds << alt;//aData.heigh.at(aLastItem);
        double bearing = 250;
        ds << bearing;//aData.relativeBearing.at(aLastItem);


//        ds << aData.timeHMSMs.at(aLastItem);
//        ds << 1/*aData.StateMassive_.at(aLastItem)*/;
//        ds << aData.latLonStdDev.at(aLastItem);
//        ds << aData.coordLatLon;
//        ds << aData.airspeed.at(aLastItem);
//        ds << aData.heigh.at(aLastItem);
//        ds << aData.relativeBearing.at(aLastItem);

        QSharedPointer<IMessageOld> msg(new MessageOld(_id, ARM_R_SERVER_BPLA_COORDS, ba));
        _subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS, msg);
    }
}
