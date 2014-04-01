#include "RPCClient.h"
#include <QDebug>

RPCClient::RPCClient(IRouter* router, IRPC* server):
    _id(0),
    _type(0)
{
//    qRegisterMetaType<rpc_msg> ("rpc_msg");
    _router = router;
    _server = server;

    connect(this, SIGNAL(signalReadyData(rpc_msg)), this, SLOT(_slotGetData(rpc_msg)));
}

RPCClient::~RPCClient()
{
}

void RPCClient::set_id(int id)
{
    _id = id;
}

void RPCClient::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    _slotGetData(msg_ptr);
}

int RPCClient::get_id()
{
    return _id;
}

void RPCClient::set_type(int type)
{
    _type = type;
}

int RPCClient::get_type()
{
    return _type;
}

void RPCClient::_slotGetData(rpc_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
	IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);
    quint64 cid = _server->get_client_id(this);

    QPointF point;
    double  alt;
    switch(type1)
    {
    case KTR_BLA:
        ds >> point;
        ds >> alt;
        emit signalSendToRPCBLAPoints(cid, id, point, alt, 0, 0, 0);
        break;
    case ARM_R_SERVER_BPLA_COORDS:
//        ds >> id_temp;
        emit signalSendToRPCBPLAPoints(cid, dd);
        break;
    case ARM_R_SERVER_BPLA_COORDS_AUTO:
        emit signalSendToRPCBPLAPointsAuto(cid, dd);
    case NIIPP_ANSWER:
        emit signalSendToNIIPPPoints(cid, dd);
        break;
    case ARM_R_SERVER_ATLANT_DIRECTION:
        emit signalSendToRPCAtlantDirection(cid, dd);
        break;
    case ARM_R_SERVER_ATLANT_POSITION:
        emit signalSendToRPCAtlantPosition(cid, dd);
        break;

    case AIS_DATA:
        emit signalSendToRPCAISData(cid, dd);
        break;



    default:
        break;
    }
}
