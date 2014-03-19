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

	/// RPC signals
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
};

#endif // RPCCLIENT_H
