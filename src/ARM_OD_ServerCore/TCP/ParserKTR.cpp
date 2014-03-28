#include "ParserKTR.h"
#include <QDebug>

ParserKTR::ParserKTR(int id, IRouter *router)
{
    _id = id;
    _latitude = 0;
    _longitude = 0;
    _altitude = 0;
    _router = router;
}

ParserKTR::~ParserKTR()
{
    qDebug() << "Terminated parser";
}

/// self distroy
void ParserKTR::distruct()
{
    delete this;
}

void ParserKTR::parsing(QByteArray data)
{
    char f1 = data.at(0);
    char f2 = data.at(1);
    QString r1 = "A";
    QString r2 = "G";
    _latitude = 0;
    _longitude = 0;

    if(!(f1 == r1.toAscii().at(0) && f2 == r2.toAscii().at(0)))
        return;

    long latValue = 0;
    long lonValue = 0;
    long deg = 0;
    long min = 0;
    long minParts = 0;

    qDebug() << "parse data from KTR";
    for(int i = 0+2; i < 3+2; i++)
    {
        latValue <<= 8;
        latValue |= ((long)data.at(i)) & 255;
        lonValue <<= 8;
        lonValue |= ((long)data.at(i+3)) & 255;
    }

    deg = latValue / 100000;
    min = latValue - (deg * 100000);
    min = min / 1000;
    minParts = latValue - (deg * 100000) - (min * 1000);
    double lat = (double) minParts / (double) 1000;
    lat += min;
    lat /= 60;
    lat += deg;

    deg = lonValue / 100000;
    min = lonValue - (deg * 100000);
    min = min / 1000;
    minParts = lonValue - (deg * 100000) - (min * 1000);
    double lon = (double) minParts / (double) 1000;
    lon += min;
    lon /= 60;
    lon += deg;

    _latitude = (int) (lat * 100000);
    _latitude = lat;//_latitude / 100000;
    _longitude = (int) (lon * 100000);
    _longitude = lon;//_longitude / 100000;

    ///alt
    _altitude = (double)((((long)data.at(2+6)) << 8) | ((long)(data.at(2+7)) & 255));
    QPointF point;
    point.setX(/*(qreal)*/_latitude/*/100000*/);
    point.setY(/*(qreal)*/_longitude/*/100000*/);
    IMapClient *m_client = _router->get_map_controller()->get_map_client(1);
    if(m_client)
    {
        m_client->addBLA(_id, point, _altitude);
    }
}

void ParserKTR::_send_bla_points(QPointF point, double alt)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << point;
    ds << alt;

    QSharedPointer<IMessage> msg(new Message(_id, KTR_BLA, ba));
    _subscriber->data_ready(KTR_BLA, msg);
}

void ParserKTR::_send_correlation(quint32 point1, quint32 point2, QVector<QPointF> vec)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << point1;
    ds << point2;
    ds << vec;

    QSharedPointer<IMessage> msg(new Message(point1, FLAKON_CORRELATION, ba));
    _subscriber->data_ready(FLAKON_CORRELATION, msg);
}
