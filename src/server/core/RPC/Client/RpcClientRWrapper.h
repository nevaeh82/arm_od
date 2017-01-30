#ifndef RPCCLIENTRWRAPPER_H
#define RPCCLIENTRWRAPPER_H

#include <QObject>
#include "RPCClient_R.h"

class RpcClientRWrapper : public QObject, public IClient
{
	Q_OBJECT
private:
	quint16 m_port;
	QHostAddress m_address;
	RPCClient_R* m_rpcClient;
	IRouter* m_router;

public:
	explicit RpcClientRWrapper(QObject *parent = 0);
	void init(quint16, QHostAddress&, IRouter*);
	void setCommand(IMessageOld* msg);

	void setId(int id);
	int getId();
	void setType(int type);
	int getType();
	void sendData(QSharedPointer<IMessageOld> msg_ptr);

	void sendDataByRpc(const QString& signalType, const QByteArray& data);

signals:
	void initSignal();
	void sendDataByRpcSignal(QString signalType, QByteArray data);

	void signalARMRConnect(bool);

private slots:
	void initSlot();
	void sendDataByRpcSlot(QString signalType, QByteArray data);

};

#endif // RPCCLIENTRWRAPPER_H
