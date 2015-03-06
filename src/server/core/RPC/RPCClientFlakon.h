#ifndef RPCCLIENTFLAKON_H
#define RPCCLIENTFLAKON_H

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

typedef QSharedPointer<IMessageOld> rpc_flakon_msg;
typedef QVector<QPointF>         rpc_send_points_vector;

class RPCClientFlakon : public QObject, public IClient
{
    Q_OBJECT
public:
    RPCClientFlakon(IRouter *router, IRPC* server);
    ~RPCClientFlakon();

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
    void signalReadyData(rpc_flakon_msg msg_ptr);

    /// RPC signals
    void signalSendPoints(rpc_send_points_vector points);
    void signalResponseModulationType(quint64 cid, QString modulation);

    void signalSendToRPCPoints(quint64 cid, rpc_send_points_vector points);
    void signalSendToRPCCorPoints(quint64 cid, quint32 point1, quint32 point2, rpc_send_points_vector points);

    ///PRM
    void signalPRMStatus(quint64, QByteArray *ba);



private slots:
    void _slotGetData(rpc_flakon_msg msg_ptr);
    void _slotTest(rpc_send_points_vector v);

};

//Q_DECLARE_METATYPE(rpc_flakon_msg)

#endif // RPCCLIENTFLAKON_H
