#ifndef BPLASIMULATOR_H
#define BPLASIMULATOR_H

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

// Contains data to calculate position for one BPLA
struct BplaSimulatorNode {
	BplaSimulatorNode( const uint& device = 0,
					   const double& centerLat = 0, const double& centerLon = 0,
					   const double& alt = 0, const double& angle = 0,
					   const double& radiusX = 0.01, const double& radiusY  = 0.01 )
	{
		this->device = device;
		this->centerLat = centerLat;
		this->centerLon = centerLon;
		this->alt = alt;
		this->angle = angle;
		this->radiusX = radiusX;
		this->radiusY = radiusY;
	}

	// device number, which should emulate for current BPLA
	uint device;

	// center of ellipsiod track of BPLA
	double centerLat;
	double centerLon;

	// X and Y radiuses of ellipsoid track
	double radiusX;
	double radiusY;

	double alt;
	double angle;
};

class KtrSimulator : public QObject
{
	Q_OBJECT

private:
	const uint m_port;
	QTcpServer* m_server;

	// buffers to store data for each socket until block has completely received
	QHash<QTcpSocket*, QByteArray*> m_buffers;

	// map which links BPLA id with it emulation data
	QMap<uint, BplaSimulatorNode> m_bplaList;

	// set of connected sockets
	QSet<QTcpSocket*> m_bplaConnections;
	QSet<QTcpSocket*> m_ktrConnections;

	QTimer m_updateTimer;
	QTimer m_uavListUpdateTimer;

	// base angle for calculate current position of every BPLA
	double m_baseAngle;

	// map of commands regular expressions
	QMap<int, QRegExp> m_commandsRegExps;

	/**
	 * @brief Convert decimal degrees to degrees, minutes and thousandth
	 * @param angle
	 * @return Number in format DDMMTTT, where DD - degrees, MM - minutes, TTT - thousandth of minutes
	 */
	long encodeAngle( double angle );

	/**
	 * @brief Rotates point to specified angle
	 * @param cx X center of rotation
	 * @param cy Y center of rotation
	 * @param angle Angle to rotate
	 * @param x horizontal coordinate of input point
	 * @param y vertical coordinate of input point
	 */
	void rotatePoint( const double& cx, const double& cy,
					  const double& angle, double& x, double& y );

public:
	explicit KtrSimulator( const uint& port, const uint& bplaCount = 5,
						   QObject *parent = 0 );
	~KtrSimulator();

	bool start();
	void stop();

	QByteArray encodeBplaList();
	QByteArray encodePosition(const uint& id);
	QByteArray encodeAtitude(const uint& id);

protected slots:
	void update();
	void updateUavList();
	void newConnection();
	void readyRead();
	void disconnected();
	bool writeAndFlush( QTcpSocket *socket, const QByteArray& data );

signals:
	void dataRecieved( const QByteArray& data );
};

#endif // BPLASIMULATOR_H
