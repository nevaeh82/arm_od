#include "ExchangeMainTreeItem.h"

#include <QDebug>

ExchangeMainTreeItem::ExchangeMainTreeItem(IRouter *router, QMap<int, TabsProperty *> map_settings)
{
    _router = router;
    _map_settings = map_settings;
	_id = 0;
}

ExchangeMainTreeItem::~ExchangeMainTreeItem()
{
}

/// set id of punkt
void ExchangeMainTreeItem::set_id(unsigned int id)
{
    _id = id;
}

void ExchangeMainTreeItem::push_data(QVariant station_name, QVariant name, QVariant value)
{
    unsigned int type;
    qDebug() << name.toString() << _id;
    QVariant var;
    if(name.toString() == ("Öåíòð f (ÌÃö)"))
    {
        type = 100;
        var = value.toInt();
    }
    if(name.toString() == ("Ôèëüòð (êÃö)"))
    {
        type = 101;
        var = value.toInt();
    }
    if(name.toString() == ("Ñäâèã (êÃö)"))
    {
        type = 102;
        var = value.toFloat();
    }
    if(name.toString() == ("KM"))
    {
        type = 103;
        var = value.toBool();
    }


    QByteArray ba;

    ba = _encode(type, &var.toByteArray());


    emit signalWriteData(ba);
}

QVariant ExchangeMainTreeItem::get_data()
{
    return QVariant();
}

QByteArray ExchangeMainTreeItem::_encode(unsigned int type, QByteArray *data)
{

    ZaviruhaPayloadPacketHeader payloadHeader_;
    ZaviruhaPayloadPacket       payloadPacket_;
    payloadHeader_.magic = PAYLOAD_PREAMBULE;
    payloadHeader_.id = _id;
    payloadHeader_.flags = 0;
    payloadHeader_.reserved = 0;
    if(type = 103)
    {
        payloadHeader_.length = sizeof(bool);
    }
    else
    {
        if(type != 102)
            payloadHeader_.length = sizeof(int);//data->size();
        else
            payloadHeader_.length = sizeof(float)*2;
    }
    payloadHeader_.type = type;
    payloadHeader_.timestamp = 0;
    CRCs crc;
    payloadHeader_.headerCRC = crc.crc8(reinterpret_cast<unsigned char *>(&payloadHeader_), 36);

    payloadHeader_.messageCRC = crc.crc16(reinterpret_cast<unsigned char *>(&data[0]), data->size());
    payloadPacket_.header = payloadHeader_;
    strcpy(reinterpret_cast<char *>(&payloadPacket_.data), reinterpret_cast<char *>(&data[0]));

    QByteArray ba;
    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in << payloadHeader_.magic;
    in << payloadHeader_.number;
    in << payloadHeader_.id;
    in << payloadHeader_.flags;
    in << payloadHeader_.timestamp;
    in << payloadHeader_.type;
    in << payloadHeader_.length;
    in << payloadHeader_.reserved;
    in << payloadHeader_.messageCRC;
    in << payloadHeader_.headerCRC;

    bool t;
    QString sd;
    switch(type)
    {
    case 100:
        in << data->toInt();
        break;
    case 101:
        in << data->toInt();
        break;
    case 102:
//        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        in << data->toFloat();
        qDebug() << data->toFloat();
        break;
    case 103:

        if(QString::fromAscii(data->constData()) == "true")
        {
            t = true;
        }
        else
        {
            t = false;
        }
        //t = (bool)data;
        in << t;//data;
    default:
        break;
    }
    return ba;
}
