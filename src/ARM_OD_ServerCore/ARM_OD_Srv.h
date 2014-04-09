#ifndef ARM_OD_SRV_H
#define ARM_OD_SRV_H

#include <QObject>
#include <QTextCodec>

#include <QMetaType>

#include <QPointF>

#include <SettingsManager/RpcSettingsManager.h>

#include "RPC/RPCServer.h"
#include "RPC/RpcConfigReader.h"

#include "Common/Router.h"
#include "Common/Subscriber.h"


#include "Common/Simulators/AtlantSimulator.h"

#include "RPC/Client/RPCClient_R.h"

#include "TCP/TcpManager.h"
#include "TCP/TcpSettingsManager.h"

typedef QSharedPointer<IMessageOld> rpc_msg;
typedef QVector<QPointF>         rpc_send_points_vector;
typedef QPointF                  rpc_QPointF;
typedef QByteArray               rpc_send_ba_data;

class ARM_OD_Srv : public QObject
{
    Q_OBJECT

private:
	TcpManager* m_tcpManager;

public:
    ARM_OD_Srv();
    ~ARM_OD_Srv();

public:

private:
    RPCServer*      _rpc_server;
    Router*         _router;
	Subscriber*     _subscriber_up;

	RPCClient_R*    _rpc_client1;
	RpcConfigReader* m_rpcConfigReader;

signals:
    void signalStartRPC();
    void signalStopRPC();
    void signalFinishRPC();

    void closeALL();

};

Q_DECLARE_METATYPE(rpc_msg)
Q_DECLARE_METATYPE(rpc_QPointF)
Q_DECLARE_METATYPE(rpc_send_ba_data)

#endif // ARM_OD_SRV_H
