#ifndef BLASIMULATOR_H
#define BLASIMULATOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

class BlaSimulator : public QObject
{
	Q_OBJECT

private:
	const unsigned int _port;
	quint8 _blaCount;
	QTcpServer* _server;

	// buffers to store data for each socket until block has completely received
	QHash<QTcpSocket*, QByteArray*> buffers;

	/**
	 * @brief Convert decimal degrees to degrees, minutes and seconds
	 * @param angle
	 * @return Number in format DDMMSSS, where DD - degrees, MM - minutes, SSS - seconds
	 */
	long encodeAngle( double angle );

public:
	explicit BlaSimulator( const unsigned int port, QObject *parent = 0 );
	~BlaSimulator();

	bool start();
	void stop();

		bool decodeCommandGetBLA(QByteArray *buffer, int& device, int& id);
		QByteArray encodeBlaList();
		QByteArray encodePlaPosition(const unsigned int& id);
		QByteArray encodeBlaAtitude(const unsigned int& id);
protected slots:
	void newConnection();
	void readyRead();
	void disconnected();
	bool writeAndFlush( QTcpSocket *socket, const QByteArray& data );
//	void processData( const QByteArray& data );

signals:
	void dataRecieved( const QByteArray& data );
};

#endif // BLASIMULATOR_H
