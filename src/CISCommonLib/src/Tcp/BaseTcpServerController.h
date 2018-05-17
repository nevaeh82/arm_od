#ifndef BASETCPSERVERCONTROLLER_H
#define BASETCPSERVERCONTROLLER_H

#include <QObject>

#include "Interfaces/Tcp/ITcpReceiver.h"
#include "Interfaces/Tcp/ITcpServerController.h"
#include "Interfaces/Tcp/ITcpListener.h"

#include "BaseTcpServer.h"
#include "BaseTcpDeviceCoder.h"

#include "Logger/Logger.h"

#define BASE_TCP_SERVER "baseTcpServer"

class BaseTcpServerController : public QObject, public ITcpServerController, public ITcpReceiver, public BaseSubject<ITcpListener>
{
	Q_OBJECT
protected:
	BaseTcpServer* m_tcpServer;
	BaseTcpDeviceCoder* m_tcpServerCoder;
	QString m_tcpServerName;
	quint32 m_deviceType;

public:
	explicit BaseTcpServerController(const QString& tcpServerName = BASE_TCP_SERVER, QObject *parent = 0);
	virtual ~BaseTcpServerController();

	void onDataReceived(const QVariant& argument);

	void createTcpServer();
	void createTcpServerCoder();

	void start(const QHostAddress& host,  const quint16 port);
	void stop();

	bool isListening();

	bool sendData(const uint clientId, const MessageSP message);
	bool sendData(const MessageSP message);

	QObject* asQObject();

private:

	QByteArray prepareDataToSend(const MessageSP message);

signals:
	
public slots:
	
};

#endif // BASETCPSERVERCONTROLLER_H
