#include "AtlantSimulator.h"

AtlantSimulator::AtlantSimulator(IRouter* router, double lat, double lon, int sourceid):
    _id(-1)
{
    _router = router;
    _subscriber = _router->get_subscriber();

    _lat = lat;
    _lon = lon;
    _sourceid = sourceid;

    connect(this, SIGNAL(signalData(QSharedPointer<IMessageOld>)), this, SLOT(_slot_simulation(QSharedPointer<IMessageOld>)));
}

AtlantSimulator::~AtlantSimulator()
{
    emit signalFinished();
}

void AtlantSimulator::set_id(int id)
{
    _id = id;
}

void AtlantSimulator::set_type(int type)
{
    _type = type;
}

int AtlantSimulator::get_id()
{
    return _id;
}

int AtlantSimulator::get_type()
{
    return _type;
}

void AtlantSimulator::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    emit signalData(msg_ptr);
}


void AtlantSimulator::_slot_simulation(QSharedPointer<IMessageOld> msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds1(*dd);
    QPointF point;
    double  alt;


    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);


    A_Dir_Ans_msg msg;
    msg.requestId = _sourceid;
    msg.sourceId = _sourceid;
    msg.dateTime = 0;
    msg.post = QString("Atlant_%1").arg(_id);
    msg.postLatitude = _lat;
    msg.postLongitude = _lon;
    msg.postHeight = 0;
    msg.frequency = 0;
    msg.widht = 0;

    ds1 >> point;
    ds1 >> alt;

    double d_lon;
    double d_lat;
    double angle;
    d_lon =  point.x() - _lon;
    d_lat =  point.y() - _lat;
    angle = atan2(d_lat,d_lon)*180/3.14;

    if ( angle < 0 ) {
        angle = 360 + angle;
    }

    qDebug() << d_lon << d_lat << angle;

//    angle *= -1;
//    angle -= 90;


    msg.direction = angle;
    msg.angle = 0;
    msg.level = 0;
    msg.quality = 0;
    msg.motionType = 0;
    msg.motionConfidence = 0;


    ds << msg.requestId;
    ds << msg.sourceId;
    ds << msg.dateTime;
    ds << msg.post;
    ds << msg.postLatitude;
    ds << msg.postLongitude;
    ds << msg.postHeight;
    ds << msg.frequency;
    ds << msg.widht;
    ds << msg.direction;
    ds << msg.angle;
    ds << msg.level;
    ds << msg.quality;
    ds << msg.motionType;
    ds << msg.motionConfidence;

//    ds << _sourceid;
//    ds << _sourceid;
//    ds << QDateTime::currentDateTime();
//    ds << QString("Atlant_%1").arg(_id);
//    ds << _lat;
//    ds << _lon;
//    ds << 0;
//    ds << 0;
//    ds << 0;



////    switch(type1)
////    {
////    case KTR_BLA:


//        ds << angle;
//        ds << 0;
//        ds << 0;
//        ds << 0;
//        ds << 0;
//        ds << 0;

        QSharedPointer<IMessageOld> msg1(new MessageOld(_id, ARM_R_SERVER_ATLANT_DIRECTION, ba));
        _subscriber->data_ready(ARM_R_SERVER_ATLANT_DIRECTION, msg1);

//        break;


//    default:
//        break;
//    }
}
