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

#include "IRPC.h"
#include "Common/IMessage.h"
#include "Common/CommandMessage.h"

#include "Station.h"
#include "Tabs/Tree/DBManager.h"
#include "Tabs/ITabMap.h"
#include "MapInterface/IMapController.h"

#include "RadiolocationInterface.h"

#include "Rpc/RpcDefines.h"
#include "Rpc/RpcClientBase.h"

#include "UAVDefines.h"

#include "Interfaces/IDbBlaManager.h"
#include "Tabs/DbBla/Defines.h"

class RPCClient : public RpcClientBase
{
    Q_OBJECT
private:
	IMessageOld*        m_commandMsg;
	IDbBlaManager*		m_dbBlaManager;
	IDBManager*         m_dbManagerTarget;
	Station	*			m_station;
	ITabMap*            m_parentTab;
	ITabManager*        m_tabManager;
	IMapController*     m_mapController;

	QMap<int, int>      m_mapPelengEvilIds;
	int                 m_pelengEvilIds;
	int                 m_rdsEvilIds;

	bool                m_solverAuto;

public:
	RPCClient(Station *station, IDbBlaManager *db_manager, IDBManager *db_manager_target,
			  IMapController* map_controller, ITabMap *parent_tab, ITabManager* tab_manager, QObject *parent = NULL);
    ~RPCClient();

	bool start(quint16 port, QHostAddress ipAddress);
	void setCommand(IMessageOld* msg);

private:
	void formCommand(IMessageOld *msg);
	void setSolverAuto(QByteArray ba);
	void sendNiippBpla(QByteArray data);
	void sendDataToSovler(QByteArray data);
	void setSolverClear(QByteArray data);

	void sendBplaPoints(QByteArray data);

public slots:
    ///rpc_server
	void rpcSendBlaPoints(QByteArray data);
	void rpcSlotServerSendAisData(QByteArray data);
	void rpcSendBplaPoints(QByteArray data);
	void rpcSendBplaPointsAuto(QByteArray data);
	void rpcSendNiippData(QByteArray data);

	void rpcSlotServerSendAtlantDirection(QByteArray data);
	void rpcSlotServerSendAtlantPosition(QByteArray data);

private slots:
	void slotRCPConnetion();
	void slotSetCommand(IMessageOld* msg);

signals:
	void signalSetCommand(IMessageOld *msg);

	///RPC signals
	void signalSetClientId(int id);
	void signalSetNIIPPBPLA(QByteArray data);
	void signalSetSolverData(QByteArray data);
	void signalSetSolverDataClear(QByteArray data);

};

#endif // RPCCLIENT_H
