#include "NIIPPController.h"

NIIPPController::NIIPPController(IRouter *router):
    _id(101),
    _ip("192.168.10.50"),
    _port(6340),
    _type(-1)
{
    _router = router;
    _subscriber = _router->get_subscriber();
}

NIIPPController::~NIIPPController()
{
}

void NIIPPController::setController(QString ip, quint16 port)
{
    _parser = new NIIPPParser(_id, _type);
    _parser->set_router(_router);

    TCPClientOld *client = _router->get_tcp_controller()->get_client(_ip, _port, _id);
    if(client)
        client->slotDestroy();

    _ip = ip;
    _port = port;

    qDebug() << _router->get_tcp_controller()->add_connection(_ip, _port, _id);

    TCPClientOld *client1 = _router->get_tcp_controller()->get_client(_ip, _port, _id);
    client1->set_parser(_parser);

    connect(this, SIGNAL(signalSendToEncodeBpla(QByteArray*)), this, SLOT(_slot_encode(QByteArray*)));
    _subscriber->add_subscription(NIIPP_BPLA, this);

}


void NIIPPController::set_id(int id)
{
    _id = id;

}

void NIIPPController::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    _slotGetData(msg_ptr);
}

int NIIPPController::get_id()
{
    return _id;
}

void NIIPPController::set_type(int type)
{
    _type = type;
}

int NIIPPController::get_type()
{
    return _type;
}

void NIIPPController::_slotGetData(rpc_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
	IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);

    switch(type1)
    {
    case NIIPP_BPLA:
        if(id == _id)
        {
            emit signalSendToEncodeBpla(dd);
        }
        break;
    default:
        break;
    }
}

void NIIPPController::_slot_encode(QByteArray* data)
{
    QDataStream ds(data, QIODevice::ReadOnly);

    QDate date;
    QTime time;

    int id;

    ds >> id;

//    ds << _id;       ???
    ds >> date;
    ds >> time;

    QString comm;
    ds >> comm;

    QPointF point;
    ds >> point;
    QString NS;
    ds >> NS;

    QString EW;

    ds >> EW;
    int alt;
    ds >> alt;

    int velocity_direction = 0;
    ds >> velocity_direction;
    int zone;
    ds >> zone;

    QPointF point2;
    ds >> point2;
    QString NS2;
    ds >> NS2;
    QString EW2;
    ds >> EW2;

    QStringList list;
    list.append(date.toString("ddMMyy"));
    list.append(time.toString("HHmmss"));
    list.append(comm);

    QString lat1;
    if(point.x() == 0)
    {
        lat1 = "000000";
    }
    else
    {
        QString lat = QString::number(point.x(), 'f', 6);
        QStringList list_lat1 = lat.split('.');

        QString lat_deg = list_lat1[0];//lat.left(2);
        double lat_min1 = lat.toDouble();
        double lat_min2 = lat_deg.toDouble();
        lat_min1 = lat_min1 - lat_min2;
        double lat_min_final = lat_min1 * 60;
        int lat_min_final1 = (int)lat_min_final;
        QString lat_min = QString::number(lat_min_final1);
        if(lat_min.length() < 2)
        {
            lat_min.insert(0, "0");
        }

        double lat_sec1 = lat_min_final - (double)lat_min_final1;
        lat_sec1 = lat_sec1 * 60;
        int lat_sec2 = lat_sec1;

        QString lat_sec = QString::number(lat_sec2);
        if(lat_sec.length() < 2)
        {
            lat_sec.insert(0, "0");
        }

        lat1 = lat_deg + lat_min + lat_sec;
    }

    list.append(lat1);
    list.append(NS);


    QString lon1;
    if(point.y() == 0)
    {
        lon1 = "000000";
    }
    else
    {
        QString lon = QString::number(point.y(), 'f', 6);
        QStringList list_lon1 = lon.split('.');

        QString lon_deg = list_lon1[0];//lat.left(2);
        double lon_min1 = lon.toDouble();
        double lon_min2 = lon_deg.toDouble();
        lon_min1 = lon_min1 - lon_min2;
        double lon_min_final = lon_min1 * 60;
        int lon_min_final1 = (int)lon_min_final;
        QString lon_min = QString::number(lon_min_final1);
        if(lon_min.length() < 2)
        {
            lon_min.insert(0, "0");
        }

        double lon_sec1 = lon_min_final - (double)lon_min_final1;
        lon_sec1 = lon_sec1 * 60;
        int lon_sec2 = lon_sec1;

        QString lon_sec = QString::number(lon_sec2);
        if(lon_sec.length() < 2)
        {
            lon_sec.insert(0, "0");
        }

        lon1 = lon_deg + lon_min + lon_sec;
    }

    list.append(lon1);
    list.append(EW);

    int alt1 = (int)alt;
    int bear = (int)velocity_direction;

    list.append(QString::number(alt1));
    list.append(QString::number(bear));
    list.append(QString::number(zone));

    QString lat1_point2;
    if(point2.x() == 0)
    {
        lat1_point2 = "000000";
    }
    else
    {
        QString lat_point2 = QString::number(point.x(), 'f', 6);
        QStringList list_lat1_point2 = lat_point2.split('.');

        QString lat_deg_point2 = list_lat1_point2[0];//lat.left(2);
        double lat_min1_point2 = lat_point2.toDouble();
        double lat_min2_point2 = lat_deg_point2.toDouble();
        lat_min1_point2 = lat_min1_point2 - lat_min2_point2;
        double lat_min_final_point2 = lat_min1_point2 * 60;
        int lat_min_final1_point2 = (int)lat_min_final_point2;
        QString lat_min_point2 = QString::number(lat_min_final1_point2);
        if(lat_min_point2.length() < 2)
        {
            lat_min_point2.insert(0, "0");
        }

        double lat_sec1_point2 = lat_min_final_point2 - (double)lat_min_final1_point2;
        lat_sec1_point2 = lat_sec1_point2 * 60;
        int lat_sec2_point2 = lat_sec1_point2;

        QString lat_sec_point2 = QString::number(lat_sec2_point2);
        if(lat_sec_point2.length() < 2)
        {
            lat_sec_point2.insert(0, "0");
        }

        lat1_point2 = lat_deg_point2 + lat_min_point2 + lat_sec_point2;
    }

    list.append(lat1_point2);
    list.append(NS2);

    QString lon1_point2;
    if(point2.y() == 0)
    {
        lon1_point2 = "000000";

    }
    else
    {
        QString lon_point2 = QString::number(point.y(), 'f', 6);
        QStringList list_lon1_point2 = lon_point2.split('.');

        QString lon_deg_point2 = list_lon1_point2[0];//lat.left(2);
        double lon_min1_point2 = lon_point2.toDouble();
        double lon_min2_point2 = lon_deg_point2.toDouble();
        lon_min1_point2 = lon_min1_point2 - lon_min2_point2;
        double lon_min_final_point2 = lon_min1_point2 * 60;
        int lon_min_final1_point2 = (int)lon_min_final_point2;
        QString lon_min_point2 = QString::number(lon_min_final1_point2);
        if(lon_min_point2.length() < 2)
        {
            lon_min_point2.insert(0, "0");
        }

        double lon_sec1_point2 = lon_min_final_point2 - (double)lon_min_final1_point2;
        lon_sec1_point2 = lon_sec1_point2 * 60;
        int lon_sec2_point2 = lon_sec1_point2;

        QString lon_sec_point2 = QString::number(lon_sec2_point2);
        if(lon_sec_point2.length() < 2)
        {
            lon_sec_point2.insert(0, "0");
        }

        lon1_point2 = lon_deg_point2 + lon_min_point2 + lon_sec_point2;
    }

    list.append(lon1_point2);
    list.append(EW2);

    QByteArray ba = _encode(list);

    ITCPClientOld* cl = _router->get_tcp_controller()->get_client(_ip, _port, _id);
    if(cl != NULL)
    {
        cl->set(ba);
    }
//   ->set(ba);
}

QByteArray NIIPPController::_encode(QStringList list)
{
    QString str = list.join(",");
    QByteArray output = str.toLocal8Bit();
    quint8 crc = 0;
    for(int i = 0; i < output.length(); i++)\
    {
        crc ^= output.at(i);
    }
    str.append(",*");
    str.append(QString::number(crc));
    str.append("\\r\\n");
    qDebug() << _ip << str;
    return str.toLocal8Bit();
}
