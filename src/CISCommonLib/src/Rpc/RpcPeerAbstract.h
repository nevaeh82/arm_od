#ifndef IRPCPEER_H
#define IRPCPEER_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QIODevice>
#include <QHostAddress>

#include <QxtAbstractSignalSerializer>
#include <QxtAbstractConnectionManager>

class RpcPeerAbstract : public QObject
{
	Q_OBJECT

public:
	RpcPeerAbstract(QObject *parent = 0) : QObject(parent) {}

	virtual bool isServer() const = 0;
	virtual bool isClient() const = 0;

	virtual void connect(QHostAddress addr, int port = 80) = 0;
	virtual void connect(QString addr, int port = 80) = 0;

	virtual bool listen(QHostAddress iface = QHostAddress::Any, int port = 80) = 0;
	virtual void stopListening() = 0;

	virtual QList<quint64> clients() const = 0;
	virtual QIODevice* device() const = 0;
	virtual void setDevice(QIODevice* dev) = 0;
	virtual QIODevice* takeDevice() = 0;

	virtual QxtAbstractSignalSerializer* serializer() const = 0;
	virtual void setSerializer(QxtAbstractSignalSerializer* serializer) = 0;

	virtual QxtAbstractConnectionManager* connectionManager() const = 0;
	virtual void setConnectionManager(QxtAbstractConnectionManager* manager) = 0;

	virtual bool attachSignal(QObject* sender, const char* signal, const QString& rpcFunction = QString()) = 0;

	virtual bool attachSlot(const QString& rpcFunction, QObject* recv, const char* slot, Qt::ConnectionType type = Qt::AutoConnection) = 0;

	virtual void detachSignals(QObject* obj) = 0;
	virtual void detachSlots(QObject* obj) = 0;
	virtual void detachObject(QObject* obj) = 0;

public Q_SLOTS:
	virtual void disconnectClient(quint64 id) = 0;
	virtual void disconnectServer() = 0;
	virtual void disconnectAll() = 0;

	virtual void call(QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) = 0;
	virtual void call(QList<quint64> ids, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) = 0;
	virtual void call(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) = 0;
	virtual void callExcept(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) = 0;

	virtual void detachSender() = 0;

Q_SIGNALS:
	void clientConnected(quint64 id);
	void clientDisconnected(quint64 id);
	void connectedToServer();
	void disconnectedFromServer();
	void serverError(QAbstractSocket::SocketError);
};

#endif // IRPCPEER_H
