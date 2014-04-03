#include <QPointF>
#include <QDebug>

#include "NIIPPParser.h"

NIIPPParser::NIIPPParser(int id, int type)
{
    _id = id;
    _type = type;
}

NIIPPParser::~NIIPPParser()
{
    qDebug() << "Terminated parser";
}

/// self distroy
void NIIPPParser::distruct()
{
delete this;
}

void NIIPPParser::set_router(IRouter *router)
{
    _router = router;
    _subscriber = _router->get_subscriber();
//    _queue = router->get_queue();
}

void NIIPPParser::parsing(QByteArray data)
{
    QString str(data);
//    qDebug() << str;

    QStringList list = str.split(',');
    QDateTime dt;
    dt.fromString(list[0], "ddMMyy");
//    qDebug() << dt;
    QTime time;
    time.fromString(list[1], "HHmmss");
//    qDebug() << time;

    int mode = list[2].toInt();
//    qDebug() << mode;

    QPointF point;
    QString lat_str = list[3];
    QString lat_sec_str = lat_str.right(2);
    int sec_int = lat_sec_str.toInt();
    double sec = (double)sec_int/3600;

    QString lat_min_str_temp = lat_str.right(4);
    QString lat_min_str = lat_min_str_temp.left(2);
    int min_int = lat_min_str.toInt();
    double min = (double)min_int/60;

    QString lat_grad = lat_str.left(lat_str.length() - 4);
    int grad_int = lat_grad.toInt();
    double grad = (double)grad_int + min + sec;

    point.setX(grad);

    QString NS = list[4];

    QString lon_str = list[5];
    QString lon_sec_str = lon_str.right(2);
    sec_int = lon_sec_str.toInt();
    sec = (double)sec_int/3600;

    QString lon_min_str_temp = lon_str.right(4);
    QString lon_min_str = lon_min_str_temp.left(2);
    min_int = lon_min_str.toInt();
    min = (double)min_int/60;

    QString lon_grad = lon_str.left(lon_str.length() - 4);
    grad_int = lon_grad.toInt();
    grad = (double)grad_int + min + sec;
    point.setY(grad);

    QString EW = list[6];

    int alt = list[7].toInt();

    int zone = list[8].toInt();

    int course = list[9].toInt();

    int angle = list[10].toInt();

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << _id;
    ds << dt;
    ds << time;
    ds << mode;
    ds << point;
    ds << NS;
    ds << EW;
    ds << alt;
    ds << zone;
    ds << course;
    ds << angle;

	QSharedPointer<IMessageOld> msg(new MessageOld(_id, NIIPP_ANSWER, ba));
    _subscriber->data_ready(NIIPP_ANSWER, msg);
    qDebug() << "MMMMMMMMMMMMMMMMMMMMMM" << course << angle << mode;

//    if(mode == 01)
//    {
//        _router->get_map_controller()->get_map_client(1)->updateNiippPowerCicle(0);
//    }
//    if(mode == 10)
//    {
//        _router->get_map_controller()->get_map_client(1)->updateNiippPowerSector(0, /*_zone[zone]**/1000, course);
//    }
}
