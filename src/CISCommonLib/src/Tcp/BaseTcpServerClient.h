#ifndef BASETCPSERVERCLIENT_H
#define BASETCPSERVERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "Templates/BaseSubject.h"
#include "Interfaces/Tcp/ITcpServerClient.h"
#include "Interfaces/Tcp/ITcpReceiver.h"

#include "Logger/Logger.h"

class BaseTcpServerClient : public QObject, public ITcpServerClient, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

private:
	QTcpSocket* m_tcpSocket;
	QTimer* m_reconnectTimer;

public:
	explicit BaseTcpServerClient(QTcpSocket*, QObject *parent = 0);
	virtual ~BaseTcpServerClient();

	// ITcpServerClient interface
public:
	void connectToHost(const QString& host, const quint32 port);
	void disconnectFromHost();
	bool isConnected();
	void writeData(const QByteArray& data);
	QString getHost();
	QObject* asQObject();
	virtual void setReconnectInterval(const uint);

private slots:
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketReadyRead();
	void onSocketDisplayError(QAbstractSocket::SocketError socketError);

	void disconnectFromHostInternalSlot();
	void writeDataInternalSlot(const QByteArray& data);

signals:
	//void disconnectFromHostInternalSignal();
	void writeDataInternalSignal(const QByteArray& data);
};

#endif // BASETCPSERVERCLIENT_H
