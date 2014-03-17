#ifndef BASETCPCLIENT_H
#define BASETCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QVariant>

#include <PwLogger/PwLogger.h>

#include "BaseSubject.h"
#include "../Interfaces/ITcpReceiver.h"
#include "../Interfaces/ITcpClient.h"

class BaseTcpClient : public QObject, public ITcpClient, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

private:
	QString m_host;
	quint32 m_port;

	QTcpSocket* m_tcpSocket;
	QTimer* m_reconnectTimer;

	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpClient(QObject* parent = NULL);
	virtual ~BaseTcpClient();

	// ITCPClient interface
public:
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual void writeData(const QByteArray& data);
	virtual QString getHost();
	virtual QObject* asQObject();

private slots:
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketReadyRead();
	void onSocketDisplayError(QAbstractSocket::SocketError socketError);

	void connectToHostInternalSlot(const QString& host, const quint32& port);
	void disconnectFromHostInternalSlot();
	void writeDataInternalSlot(const QByteArray& data);

signals:
	void connectToHostInternalSignal(const QString& host, const quint32& port);
	void disconnectFromHostInternalSignal();
	void writeDataInternalSignal(const QByteArray& data);
};

#endif // BASETCPCLIENT_H
