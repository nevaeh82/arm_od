#include <math.h>
#include <time.h>

#include <QRegExp>

#include "KtrSimulator.h"

#define  CMD_LIST_BPLA					1
#define  CMD_REGISTER_LISTENER			2

#define  CMD_MIN_LENGTH					22

KtrSimulator::KtrSimulator(const uint& port, const uint& bplaCount,
						   QObject *parent )
	: QObject( parent )
	, m_port( port )
	, m_server( NULL )
	, m_baseAngle( 0 )
{
	// init map of commands regular expressions
	m_commandsRegExps.insert( CMD_LIST_BPLA, QRegExp( "user link command ctrl" ) );
	m_commandsRegExps.insert( CMD_REGISTER_LISTENER, QRegExp( "user link command b(\\d+)d(\\d+)" ) );

	// init base simulation data for each BPLA
	qsrand( time(0) );
	for (uint i=1; i <= bplaCount; i++) {
		uint device = 1;
		uint angle = qrand() % 360;
		double lat = 60 + (double)qrand() / RAND_MAX * 0.2;
		double lon = 30 + (double)qrand() / RAND_MAX;
		double alt = 1500 + qrand() % 1000;
		double radiusX = 0.005 + (double)qrand() / RAND_MAX * 0.01;
		double radiusY = 0.005 + (double)qrand() / RAND_MAX * 0.01;

		m_bplaList.insert( 100 + i, BplaSimulatorNode( device, lat, lon, alt, angle,
													   radiusX, radiusY ) );
	}

	// start update timer
	connect( &m_updateTimer, SIGNAL(timeout()), SLOT(update()) );
	m_updateTimer.start( 100 );

	connect(&m_uavListUpdateTimer, SIGNAL(timeout()), SLOT(updateUavList()));
	m_uavListUpdateTimer.start(1000);
}

KtrSimulator::~KtrSimulator()
{
	stop();
}

bool KtrSimulator::start()
{
	if (m_server) return true;

	m_server = new QTcpServer( this );

	connect( m_server, SIGNAL(newConnection()), this, SLOT(newConnection()) );

	// start server
	if (!m_server->listen( QHostAddress::Any, m_port )) {
		qCritical() << QString::fromUtf8( "Unable to start the server: %1." )
					 .arg( m_server->errorString() );

		stop();

		return false;
	}

	return true;
}

void KtrSimulator::stop()
{
	if (m_server == NULL) return;

	m_server->close();
	delete m_server;
	m_server = NULL;
}

void KtrSimulator::newConnection()
{
	while (m_server->hasPendingConnections()) {
		QTcpSocket *socket = m_server->nextPendingConnection();

		connect( socket, SIGNAL(readyRead()), SLOT(readyRead()) );
		connect( socket, SIGNAL(disconnected()), SLOT(disconnected()) );

		QByteArray *buffer = new QByteArray();
		m_buffers.insert( socket, buffer );
	}
}

void KtrSimulator::disconnected()
{
	QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
	QByteArray *buffer = m_buffers.value(socket);

	m_buffers.remove(socket);
	m_bplaConnections.remove(socket);
	m_ktrConnections.remove(socket);

	socket->deleteLater();
	delete buffer;
}

bool KtrSimulator::writeAndFlush( QTcpSocket* socket, const QByteArray& data )
{
	return socket->write( data ) != -1
			&& socket->waitForBytesWritten()
			&& socket->flush();
}

void KtrSimulator::readyRead()
{
	QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
	QByteArray *buffer = m_buffers.value(socket);

	while (socket->bytesAvailable() > 0) {
		buffer->append(socket->readAll());

		// while data length is enouth for read the shortest
		// command ("user link command ctrl" - 22 bytes), let's do it
		while (buffer->count() >= CMD_MIN_LENGTH) {
			QRegExp cmdRegExp;

			// execute command LIST BPLA: user link command ctrl
			cmdRegExp = m_commandsRegExps.value( CMD_LIST_BPLA );
			if (cmdRegExp.indexIn( *buffer ) == 0) {
				buffer->remove( 0, cmdRegExp.matchedLength() );
				qDebug() << "Requested list of BPLA";

				m_ktrConnections << socket;
			}

			// execute REGISTER BPLA LISTENER: user link command bXXdYY
			cmdRegExp = m_commandsRegExps.value( CMD_REGISTER_LISTENER );
			if (cmdRegExp.indexIn( *buffer ) == 0) {
				int id = 0, device = 0;
				id = cmdRegExp.cap(1).toInt();
				device = cmdRegExp.cap(2).toInt();

				buffer->remove( 0, cmdRegExp.matchedLength() );
				qDebug() << QString( "Registered connection for BPLA %1, device = %2" )
							.arg( id ).arg( device ).toStdString().c_str();

				if (!m_bplaList.contains( id )) {
					qDebug() << QString( "BPLA with ID %1 not eists" )
								.arg( id ).toStdString().c_str();
				}

				// remember id and device of requested BPLA
				socket->setProperty( "bpla_id", id );
				socket->setProperty( "bpla_device", device );

				m_bplaConnections << socket;
			}
		}
	}

	if (buffer->length()) {
		qDebug() << "Not processed from client: " << *buffer;
	}
}

long KtrSimulator::encodeAngle( double angle ) {
	int deg = (int)angle;
	double frac = ( angle - (double)deg ) * 60;
	int min = (int)frac;
	int sec = ( frac - (double)min ) * 1000;

	long result = deg * 100000 + min * 1000 + sec;
	return result;
}

QByteArray KtrSimulator::encodeBplaList()
{
	QByteArray data;
	quint8 count = m_bplaList.count();

	data.append( QString("control link") );
	data.append( (char*) &count, 1 );
	data.append( QString( 1, ' ' ) );

	foreach (int i, m_bplaList.keys()) {
		quint16 id = i;
		data.append( (char*) &id, 2 );
		data.append( QString( 22, ' ' ) );
	}

	return data;
}

QByteArray KtrSimulator::encodeKtrPosition(const uint& id)
{
	if (!m_bplaList.contains( id )) {
		return QByteArray();
	}

	BplaSimulatorNode node = m_bplaList.value( id );

	double radiusX = node.radiusX - 0.00005 + (double)qrand() / RAND_MAX * 0.0001;
	double radiusY = node.radiusY - 0.00005 + (double)qrand() / RAND_MAX * 0.0001;

	double lon = cos((180 - m_baseAngle) * M_PI / 180) * radiusX + node.centerLon;
	double lat = sin((180 - m_baseAngle) * M_PI / 180) * radiusY + node.centerLat;

	rotatePoint( node.centerLon, node.centerLat, node.angle, lon, lat );

//	int deg = (int)angle;
//	double frac = ( angle - (double)deg ) * 60;
//	int min = (int)frac;
//	int sec = ( frac - (double)min ) * 60;

	QString str = QString( "$KTPGA,OBJ_ID=%1,1X,P0={%2*%3'%4\"|%5*%6'%7\"},END" )
			.arg( id )
			.arg( (int) lat )
			.arg( (int) (( lat - (int) lat ) * 60) )
			.arg( ((( lat - (int) lat ) * 60) - (int) (( lat - (int) lat ) * 60)) * 60, 0, 'f', 2 )
			.arg( (int) lon )
			.arg( (int) (( lon - (int) lon ) * 60) )
			.arg( ((( lon - (int) lon ) * 60) - (int) (( lon - (int) lon ) * 60)) * 60, 0, 'f', 2 );

	return str.toAscii();
}

QByteArray KtrSimulator::encodeAutopilotPosition(const uint& id )
{
	QByteArray data;

	if (!m_bplaList.contains( id )) {
		return data;
	}

	BplaSimulatorNode node = m_bplaList.value( id );

	double radiusX = node.radiusX - 0.00005 + (double)qrand() / RAND_MAX * 0.0001;
	double radiusY = node.radiusY - 0.00005 + (double)qrand() / RAND_MAX * 0.0001;

	double lon = cos((180 - m_baseAngle) * M_PI / 180) * radiusX + node.centerLon;
	double lat = sin((180 - m_baseAngle) * M_PI / 180) * radiusY + node.centerLat;

	rotatePoint( node.centerLon, node.centerLat, node.angle, lon, lat );

	long lonEncoded = encodeAngle( lon );
	long latEncoded = encodeAngle( lat );

	data.append( "AG" );
	data.append( ((char*) &latEncoded)[2] );
	data.append( ((char*) &latEncoded)[1] );
	data.append( ((char*) &latEncoded)[0] );
	data.append( ((char*) &lonEncoded)[2] );
	data.append( ((char*) &lonEncoded)[1] );
	data.append( ((char*) &lonEncoded)[0] );

	return data;
}

QByteArray KtrSimulator::encodeAtitude(const uint& id)
{
	QByteArray data;

	if (!m_bplaList.contains( id )) {
		return data;
	}

	BplaSimulatorNode node = m_bplaList.value( id );

	quint16 altitude = node.alt - 10 + qrand() % 20;
	data.append( "AN000000" );
	data.append( ((char*) &altitude)[1] );
	data.append( ((char*) &altitude)[0] );

	return data;
}

void KtrSimulator::update()
{
	// increase base angle
	m_baseAngle += 0.25;

	if (m_baseAngle > 360) {
		m_baseAngle -= 360;
	}

	// send updated coordinates to each client
	foreach (QTcpSocket *socket, m_bplaConnections) {
		QVariant id = socket->property( "bpla_id" );
		QVariant device = socket->property( "bpla_device" );

		if (!id.isValid() || !device.isValid()) {
			continue;
		}

		if (!m_bplaList.contains( id.toInt() )) {
			continue;
		}

		BplaSimulatorNode node = m_bplaList[ id.toInt() ];

		if (device.toInt() != node.device) {
			continue;
		}

//		if (node.device == 622) {
			QByteArray data;

			data = encodeKtrPosition( id.toInt() );
			writeAndFlush( socket, data );

			data = encodeAutopilotPosition( id.toInt() );
			writeAndFlush( socket, data );

			data = encodeAtitude( id.toInt() );
			writeAndFlush( socket, data );
//		}
	}
}

void KtrSimulator::updateUavList()
{
	//Send uav list to each client
	foreach (QTcpSocket* socket, m_ktrConnections) {
		QByteArray data = encodeBplaList();
		writeAndFlush( socket, data );
	}
}

void KtrSimulator::rotatePoint( const double& cx, const double& cy,
								 const double& angle, double& x, double& y )
{
	double s = sin(angle);
	double c = cos(angle);

	// translate point back to origin:
	x -= cx;
	y -= cy;

	// rotate point
	double xnew = x * c - y * s;
	double ynew = x * s + y * c;

	// translate point back:
	x = xnew + cx;
	y = ynew + cy;
}
