#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QSharedPointer>
#include <QMetaType>
#include "../Common/IClient.h"
#include "../Common/IRouter.h"
#include "../Common/IMessage.h"
#include "../Common/ISubscriber.h"
#include "IRPC.h"

typedef QSharedPointer<IMessageOld> rpc_msg;
typedef QPointF                  rpc_QPointF;
//typedef QVector<QPointF>         rpc_send_points_vector;

class RPCClient : public QObject, public IClient
{
    Q_OBJECT
public:
    RPCClient(IRouter *router, IRPC* server);
    ~RPCClient();

public:
    virtual void set_id(int _id);
    virtual int  get_id();
    virtual void set_type(int type);
    virtual int get_type();
	virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);


private:
    int             _id;
    int             _type;
    IRouter*        _router;
    IRPC*           _server;

signals:
    void signalReadyData(rpc_msg msg_ptr);

//    /// RPC signals
//    void signalSendPoints(rpc_send_points_vector points);
//    void signalResponseModulationType(quint64 cid, QString modulation);

//    void signalSendToRPCPoints(quint64 cid, rpc_send_points_vector points);
//    void signalSendToRPCCorPoints(quint64 cid, quint32 point1, quint32 point2, rpc_send_points_vector points);

//    ///PRM
//    void signalPRMStatus(quint64, QByteArray *ba);


    ///BLA
//    void signalSendToRPCPointBLA(quint64 cid, QPointF point, double alt);
    void signalSendToRPCBLAPoints(quint64,int,rpc_QPointF,double,double,double,int);
    void signalSendToRPCAISData(quint64,QByteArray*);


    /// from ARM_R
    void signalSendToRPCBPLAPoints(quint64, QByteArray*);
    void signalSendToRPCBPLAPointsAuto(quint64, QByteArray*);


    /// from NIIPP
    void signalSendToNIIPPPoints(quint64, QByteArray*);

    /// from ARM_R atlant direction
    void signalSendToRPCAtlantDirection(quint64, QByteArray*);
    void signalSendToRPCAtlantPosition(quint64, QByteArray*);




private slots:
    void _slotGetData(rpc_msg msg_ptr);
//    void _slotTest(rpc_send_points_vector v);

};

//Q_DECLARE_METATYPE(rpc_msg)

#endif // RPCCLIENT_H
