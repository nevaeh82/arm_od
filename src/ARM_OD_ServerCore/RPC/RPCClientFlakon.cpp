#include "RPCClientFlakon.h"
#include <QDebug>

RPCClientFlakon::RPCClientFlakon(IRouter* router, IRPC* server):
    _id(0),
    _type(0)
{
    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
    _router = router;
    _server = server;

    connect(this, SIGNAL(signalReadyData(rpc_flakon_msg)), this, SLOT(_slotGetData(rpc_flakon_msg)));    
//    connect(this, SIGNAL(signalSendPoints(rpc_send_points_vector)), this, SLOT(_slotTest(rpc_send_points_vector)));
}

RPCClientFlakon::~RPCClientFlakon()
{
}

void RPCClientFlakon::set_id(int id)
{
    _id = id;
}

void RPCClientFlakon::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    _slotGetData(msg_ptr);
//    emit signalReadyData(msg_ptr);
}

int RPCClientFlakon::get_id()
{
    return _id;
}

void RPCClientFlakon::set_type(int type)
{
    _type = type;
}

int RPCClientFlakon::get_type()
{
    return _type;
}

void RPCClientFlakon::_slotGetData(rpc_flakon_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
	IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!called this " << _id;
    quint64 cid = _server->get_client_id(this);
    QVector<QPointF> points;
    QString str;
    quint32 point1, point2;
    quint16 freq;
    quint8 filter;
    quint8 att1;
    switch(type1)
    {
    case FLAKON_FFT:
        ds >> points;
        emit signalSendToRPCPoints(cid, points);
        break;
    case FLAKON_CORRELATION:
        ds >> point1;
        ds >> point2;
        ds >> points;
        emit signalSendToRPCCorPoints(cid, point1, point2, points);
        break;
    case FLAKON_SIGNAL_TYPE:
        ds >> str;
        emit signalResponseModulationType(cid, str);
        break;
    case PRM_STATUS:
//        ds >> freq;
        emit signalPRMStatus(cid, dd);
        break;
    default:
        break;
    }


    //emit signalSendPoints(points);
   // delete f;
}

void RPCClientFlakon::_slotTest(rpc_send_points_vector v)
{

    for(int i = 0; i < v.size(); i++)
    {
        qDebug() << v.at(i).x() << v.at(i).y();
    }
}
