#ifndef BASETCPSERVER_H
#define BASETCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "Interfaces/Tcp/ITcpServer.h"
#include "Interfaces/Tcp/ITcpReceiver.h"

#include "BaseTcpServerClient.h"

#include "Templates/BaseSubject.h"

class BaseTcpServer : public QObject, public ITcpServer, public ITcpReceiver, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

private:
	QTcpServer* m_tcpServer;
	uint m_clientsCount;
	QMap<uint, ITcpServerClient*> m_clients;

protected:
	QHostAddress m_host;
	quint16 m_port;

public:
	explicit BaseTcpServer(QObject *parent = 0);

	void start(const QHostAddress& host,  const quint16 port);
	void stop();

	bool isListening();

	bool sendData(const QByteArray& data);
	bool sendData(const uint clientId, const QByteArray& data);

	void onDataReceived(const QVariant& argument);

	ITcpServerClient* getClientById(uint);
private slots:
	void onNewConnectionSlot();

signals:
	void newClientSignal(uint clientId, ITcpServerClient*);
};

#endif // BASETCPSERVER_H
