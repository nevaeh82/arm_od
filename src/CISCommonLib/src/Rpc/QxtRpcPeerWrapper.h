#ifndef QXTRPCPEERWRAPPER_H
#define QXTRPCPEERWRAPPER_H

#include <QxtRPCPeer>

#include "Rpc/RpcPeerAbstract.h"


/// Wrapper for QxtRpcPeer, that needs to used in our RPC implementations (RpcServerBase)
class QxtRpcPeerWrapper : public RpcPeerAbstract
{
	Q_OBJECT

private:
	QxtRPCPeer *m_peer;

public:
	QxtRpcPeerWrapper(QObject* parent = 0);
	virtual ~QxtRpcPeerWrapper();

	bool isServer() const;
	bool isClient() const;

	void connect(QHostAddress addr, int port = 80);
	void connect(QString addr, int port = 80);

	bool listen(QHostAddress iface = QHostAddress::Any, int port = 80);
	void stopListening();

	QList<quint64> clients() const;
	QIODevice* device() const;
	void setDevice(QIODevice* dev);
	QIODevice* takeDevice();

	QxtAbstractSignalSerializer* serializer() const;
	void setSerializer(QxtAbstractSignalSerializer* serializer);

	QxtAbstractConnectionManager* connectionManager() const;
	void setConnectionManager(QxtAbstractConnectionManager* manager);

	bool attachSignal(QObject* sender, const char* signal,
							  const QString& rpcFunction = QString());

	bool attachSlot(const QString& rpcFunction, QObject* recv, const char* slot,
							Qt::ConnectionType type = Qt::AutoConnection);

	void detachSignals(QObject* obj);
	void detachSlots(QObject* obj);
	void detachObject(QObject* obj);

public Q_SLOTS:
	void disconnectClient(quint64 id);
	void disconnectServer();
	void disconnectAll();

	void call(QString fn,
					  const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
					  const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
					  const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
					  const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());

	void call(QList<quint64> ids, QString fn,
					  const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
					  const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
					  const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
					  const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());

	void call(quint64 id, QString fn,
					  const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
					  const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
					  const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
					  const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());

	void callExcept(quint64 id, QString fn,
							const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
							const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
							const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
							const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());

	void detachSender();
};

#endif // QXTRPCPEERWRAPPER_H
