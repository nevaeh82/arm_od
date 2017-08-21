#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include <QMutex>
#include <QMutexLocker>

#include "IRPC.h"
#include "Common/IMessage.h"
#include "Common/CommandMessage.h"

#include "Station.h"
#include "Tabs/ITabMap.h"
#include "Map/IMapController.h"


#include "Rpc/RpcDefines.h"
#include "Rpc/RpcClientBase.h"

#include "zavCommon/UAVDefines.h"

#include "Interfaces/IDbUavManager.h"
#include "Tabs/ITabManager.h"

class RPCClient : public RpcClientBase
{
	Q_OBJECT
private:
	IMessageOld*        m_commandMsg;

	Station*			m_station;

	bool                m_solverAuto;

public:
	explicit RPCClient(QObject* parent = NULL);
	virtual ~RPCClient();

	bool start(quint16 port, QHostAddress ipAddress);
	void setCommand(IMessageOld* msg);
	void setStation(Station* station);

private:
	void formCommand(IMessageOld *msg);
	void setSolverAuto(QByteArray ba);
	void sendNiippBpla(QByteArray data);
	void sendNiippConnectionStatus(QByteArray data);
	void sendDataToSovler(QByteArray data);
	void setSolverClear(QByteArray data);
	void setAdsb(QByteArray data);

public slots:
	///rpc_server
	void rpcSendBlaPoints(QByteArray data);
	void rpcSlotServerSendAisData(QByteArray data);
	void rpcSendBplaPoints(QByteArray data);
	void rpcSendBplaPoints1(QByteArray data);
	void rpcSendMapPing(QByteArray data);
    void rpcSendNiipStatus(QByteArray data);
	void rpcSendBplaPointsAuto(QByteArray data);
	void rpcSendBplaPointsSingle(QByteArray data);
	void rpcSendHyperbola(QByteArray data);
	void rpcSendARMRConnection(bool b);

	void rpcSendNiippData(QByteArray data);
	void rpcSendNiippConnectionStatus(QByteArray data);

	void rpcSlotServerSendAtlantDirection(QByteArray data);
	void rpcSlotServerSendAtlantPosition(QByteArray data);

	void rpcSlotServerSendAdbsData(QByteArray data);

private slots:
	void slotRCPConnetion();
	void slotSetCommand(IMessageOld* msg);

	void startInternalSlot(quint16 port, QString ipAddress);

signals:
	void signalSetCommand(IMessageOld *msg);

	///RPC signals
	void signalSetClientId(int id);
	void signalSetNIIPPBPLA(QByteArray data);
	void signalGetNIIPPStatus(QByteArray data);
	void signalSetSolverData(QByteArray data);
	void signalSetSolverDataClear(QByteArray data);
	void signalSetAdsb(QByteArray data);

	void startInternalSignal(quint16 port, QString ipAddress);

};

#endif // RPCCLIENT_H
