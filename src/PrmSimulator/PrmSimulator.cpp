#include <math.h>
#include <time.h>

#include <QDateTime>
#include <QDataStream>
#include <QStringList>

#include "PrmSimulator.h"

PrmSimulator::PrmSimulator(const uint& port, QObject *parent )
	: RpcServerBase( parent )
	, m_port( port )
{
	qsrand( time(0) );

	centerLat = 60 + (double)qrand() / RAND_MAX * 0.2;
	centerLon = 30 + (double)qrand() / RAND_MAX;
	alt = 1500 + qrand() % 1000;

	mode = 1;
	zone = 1;
	bearing = 1;
	angle = 0;

	connect( &m_updateTimer, SIGNAL(timeout()), SLOT(update()) );
	m_updateTimer.start( 100 );
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

void PrmSimulator::update()
{
	// calculate new position of BPLA
	angle++;
	double radius = 0.005 + (double)qrand() / RAND_MAX * 0.005;
	double lon = cos((180 - angle) * M_PI / 180) * radius + centerLon;
	double lat = sin((180 - angle) * M_PI / 180) * radius + centerLat;

	path << QPointF( lat, lon );

	// limit path points
	if (path.size() > 50) {
		path.remove( 0, path.size() - 50 );
	}

	// prepare data to send
	QByteArray data;
	QDataStream stream( &data, QIODevice::WriteOnly );

	stream << QTime();
	stream << mode;
	stream << path.last();
	stream << path;
	stream << (double) (500 + qrand() % 1000);
	stream << (double) alt;
	stream << (double) bearing;

	call( RPC_SLOT_SERVER_SEND_BPLA_DEF, QVariant( data ) );
}
