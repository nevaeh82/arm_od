#ifndef BASETCPCLIENT_H
#define BASETCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QVariant>

#include "Templates/BaseSubject.h"
#include "Interfaces/Tcp/ITcpReceiver.h"
#include "Interfaces/Tcp/ITcpClient.h"

#include "Logger/Logger.h"

namespace TCP {
typedef enum TcpConnectionState
{
	Disconnected = 0,
	Connected = 1,
	Unknown = -1
}TcpConnectionState;
}

class BaseTcpClient : public QIODevice, public ITcpClient, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

protected:
	QString m_host;
	quint32 m_port;

	QTcpSocket* m_tcpSocket;
	qint32 m_reconnectTimer;
	quint32 m_reconnectTimeout;

public:
	explicit BaseTcpClient(QObject* parent = NULL, const bool isReadRequired = true);
	virtual ~BaseTcpClient();

	virtual void connectToHostAsync(const QString& host, const quint32 port);
	virtual void disconnectFromHostAsync();

	virtual bool connectToHostSync(const QString& host, const quint32 port);
	virtual bool disconnectFromHostSync();

	virtual bool isConnected();

	virtual QString getHost();
	virtual QObject* asQObject();
	virtual void setReconnectInterval(const uint timeMsec);

	virtual bool open(OpenMode mode);
	virtual void close();
	virtual qint64 bytesAvailable() const;
        
	virtual qint64 readData(char* data, qint64 maxlen);
	virtual qint64 writeData(const char* data, qint64 len);
	void timerEvent(QTimerEvent*);

private slots:
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketReadyRead();
	void onSocketDisplayError(QAbstractSocket::SocketError socketError);

	void connectToHostInternalSlot(QString host, quint32 port);
	void disconnectFromHostInternalSlot();
	void writeDataInternalSlot(const QByteArray& data);

	void onSocketReconnectSlot();
    void killTimerSlot();

signals:
	void writeDataInternalSignal(const QByteArray& data);

	void signalConnectedToHost(int status);

    void signalKillTimer();

};

#endif // BASETCPCLIENT_H
