#ifndef TCPAISCLIENT_H
#define TCPAISCLIENT_H

#include <Tcp/BaseTcpClient.h>

#include <QUrl>
#include <QDataStream>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#define TCP_STANDART_AIS_URL "http://bma.fr24.com/zones/"

class TcpAISClient : public BaseTcpClient
{
	Q_OBJECT
private:
	QNetworkAccessManager* m_networkAccessManager;

public:
	explicit TcpAISClient(QObject* parent = NULL);
	virtual ~TcpAISClient();

	// ITcpClient interface
public:
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual void writeData(const QByteArray& data);
	virtual QString getHost();
	virtual QObject* asQObject();

private slots:
	void onGetQuerySlot(QNetworkReply* reply);
	void connectToHostAISInternalSlot(const QString& host, const quint32& port);

signals:
	void connectToHostAISInternalSignal(const QString& host, const quint32& port);
};

#endif // TCPAISCLIENT_H
