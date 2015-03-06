#ifndef RPCCLIENT_R_H
#define RPCCLIENT_R_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>
#include <QDataStream>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include <SettingsManager/RpcSettingsManager.h>

#include "IRPC_R.h"
#include "../../Common/IMessage.h"
#include "../../Common/IRouter.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"

#include "../../Common/IClient.h"

#include "Rpc/RpcDefines.h"
#include <Rpc/RpcRoutedClient.h>

#include "UAVDefines.h"

#include "TCP/TcpDefines.h"

class RPCClient_R : public RpcRoutedClient, public IClient
{
	Q_OBJECT
private:
	IMessageOld* m_commandMsg;
	int m_id;
	IRouter* m_router;
	ISubscriber* m_subscriber;

	int m_type;

public:
	RPCClient_R(IRouter* router, QObject *parent = 0);
	virtual ~RPCClient_R();

public:
	void setId(int id);
	int getId();
	void setType(int type);
	int getType();
	void sendData(QSharedPointer<IMessageOld> msg_ptr);

	bool start(quint16 port, QHostAddress address);

	void setCommand(IMessageOld* msg);
	void pushMsg(QByteArray msg);

	void sendDataByRpc(const QString& signalType, const QByteArray& data);

private slots:
	void slotSetCommand(IMessageOld* msg);
	void slotPushMsg(QByteArray msg);
	void slotGetData(QSharedPointer<IMessageOld> msg_ptr);
	void registerRoute();

private:
	void formCommand(IMessageOld *msg);
	QByteArray encodeToEnemyUav(const QByteArray& data, bool singleMode = false);

private slots:
	void slotRCPConnetion();

signals:
	void signalSetCommand(IMessageOld *msg);
	void signalPushMsh(QByteArray* data);

	void signalSendData(QSharedPointer<IMessageOld> msg_ptr);

	///RPC signals
signals:
	void signalSetClientId(int id);
	void signalSetSolver(QByteArray data);
	void signalSetSolverClear(QByteArray data);

public slots:

	///rpc_server
	void rpcSlotServerSendBplaDef(QByteArray ba);
	void rpcSlotServerSendBplaDefAuto(QByteArray ba);
	void rpcSlotServerSendBplaDefSingle(QByteArray ba);
	void rpcSlotServerSendHyperbola(QByteArray ba);

	void rpcSlotServerAtlantDirection(QByteArray ba);
	void rpcSlotServerAtlantPosition(QByteArray ba);

};

#endif // RPCCLIENT_R_H
