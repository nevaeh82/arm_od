#include <math.h>
#include <time.h>

#include <QPointF>
#include <QDateTime>
#include <QDataStream>
#include <QStringList>

#include "PrmSimulator.h"

#define _USE_MATH_DEFINES
#include "math.h"

#define UPDATE_INTERVAL 500
#define ANGLE_STEP		0.5

PrmSimulator::PrmSimulator(const uint& port, QObject *parent )
	: RpcServerBase( parent )
	, m_port( port )
{
	qsrand( time(0) );

	centerLat = 42.511183 + (double)qrand() / RAND_MAX * 0.2;
	centerLon = 41.6905 + (double)qrand() / RAND_MAX;
	alt = 1500 + qrand() % 1000;

	mode = 1;
	zone = 1;
	bearing = 1;
	angle = 0;

	qsrand(QTime::currentTime().msec());
	frequency = (double) (-1 + qrand() % 100);

	connect( &m_updateTimer, SIGNAL(timeout()), SLOT(update()) );
	m_updateTimer.start( UPDATE_INTERVAL );
}

PrmSimulator::~PrmSimulator()
{
	stop();
}

bool PrmSimulator::start( quint16 port, QHostAddress ipAddress )
{
	Q_UNUSED( ipAddress );

	if ( port != 0 ) {
		m_port = port;
	}

	return RpcServerBase::start( m_port );
}

UAVPositionDataEnemy PrmSimulator::encodeBplaData(const QDateTime& time)
{
	double radius = 0.005 + (double)qrand() / RAND_MAX * 0.0001;
	double lon = cos((180 - angle) * M_PI / 180) * radius + centerLon;
	double lat = sin((180 - angle) * M_PI / 180) * radius + centerLat;

	double speed = (double) (1 + qrand() % 10);

	UAVPositionDataEnemy uav;

	uav.altitude = alt;
	uav.altitudeStdDev = alt - 5 + qrand() % 10;
	uav.course = angle;
	uav.frequency = frequency;
	uav.latLon = QPointF( lat, lon );
	uav.latLonStdDev = QPointF( (double)(qrand() % 100) / 100 - 0.5, (double)(qrand() % 100) / 100 - 0.5 );
	uav.speed = speed;
	uav.state = 1;
	uav.time = time;

	return uav;
}

void PrmSimulator::sendUavsData()
{
	// generate data source type
	int functionRand = qrand() % 3;

	QString function;
	switch (functionRand) {
		case 0:
			function = RPC_SLOT_SERVER_SEND_BPLA_DEF;
			break;

		case 1:
			function = RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO;
			break;

		case 2:
			function = RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE;
			break;
	}

	// update base telemetry parameters
	angle += ANGLE_STEP;

	// encode BPLA data
	QList<UAVPositionDataEnemy> list;

    QDateTime time = QDateTime::currentDateTime();

	if (functionRand == 2) {
		list << encodeBplaData(); // it's for UAV_SOLVER_SINGLE_1_SOURCE
		list << encodeBplaData(); // it's for UAV_SOLVER_SINGLE_2_SOURCE
	} else {
		int tailLength = 100;

		angle -= tailLength * ANGLE_STEP;
		for ( int i = 0; i < tailLength; i++ ) {
			list << encodeBplaData( time.addMSecs( (i - tailLength + 1) * UPDATE_INTERVAL ) );
			angle += ANGLE_STEP;
		}
	}

	QByteArray data;
	QDataStream stream( &data, QIODevice::WriteOnly );
	stream << list;

	m_serverPeer->call( function, QVariant( data ) );
}

void PrmSimulator::sendHyperbolas()
{
	// send hyperbolas data
	QByteArray dataToSend;

	int size = 1 + qrand() % 10;
	QTime time = QTime::currentTime();
	QList<QVector<QPointF>> list;

	for( int i=0; i < size; i++) {
		QVector<QPointF> tmpHyperb;
		tmpHyperb.append(QPointF(60, 30 + i*10));
		tmpHyperb.append(QPointF(61, 31 + i*10));
		tmpHyperb.append(QPointF(62, 32 + i*10));
		tmpHyperb.append(QPointF(63, 31 + i*10));
		tmpHyperb.append(QPointF(64, 31 + i*10));

		list << tmpHyperb;
	}

	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << (double) 1830;
	dataStream << time;
	dataStream << list;

	m_serverPeer->call( RPC_SLOT_SERVER_SEND_HYPERBOLA, QVariant( dataToSend ) );
}

void PrmSimulator::update()
{
	sendUavsData();
	sendHyperbolas();
}
