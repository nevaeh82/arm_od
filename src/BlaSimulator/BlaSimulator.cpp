#include <math.h>

#include "DMS.h"
#include "BlaSimulator.h"

BlaSimulator::BlaSimulator(const unsigned int port, QObject *parent )
	: QObject( parent )
	, _port( port )
	, _server( NULL )
	, _blaCount( 5 )
{}

BlaSimulator::~BlaSimulator()
{
	stop();
}

bool BlaSimulator::start()
{
	if (_server) return true;

	_server = new QTcpServer( this );

	connect( _server, SIGNAL(newConnection()), this, SLOT(newConnection()) );

	// start server
	if (!_server->listen( QHostAddress::Any, _port )) {
		qCritical() << QString::fromUtf8( "Unable to start the server: %1." )
					 .arg( _server->errorString() );

		stop();

		return false;
	}

	return true;
}

void BlaSimulator::stop()
{
	if (_server == NULL) return;

	_server->close();
	delete _server;
	_server = NULL;
}

void BlaSimulator::newConnection()
{
	while (_server->hasPendingConnections()) {
		QTcpSocket *socket = _server->nextPendingConnection();

		connect( socket, SIGNAL(readyRead()), SLOT(readyRead()) );
		connect( socket, SIGNAL(disconnected()), SLOT(disconnected()) );

		QByteArray *buffer = new QByteArray();
		buffers.insert( socket, buffer );
	}
}

void BlaSimulator::disconnected()
{
	QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
	QByteArray *buffer = buffers.value(socket);
	socket->deleteLater();
	delete buffer;
}

bool BlaSimulator::writeAndFlush( QTcpSocket* socket, const QByteArray& data )
{
	return socket->write( data ) != -1
			&& socket->waitForBytesWritten()
			&& socket->flush();
}

void BlaSimulator::readyRead()
{
	QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
	QByteArray *buffer = buffers.value(socket);

	while (socket->bytesAvailable() > 0) {
		buffer->append(socket->readAll());

		// while can process data, process it
		while (buffer->count() >= 22) {
			// execute GET BLA LIST: user link command ctrl
			if (buffer->left(22) == "user link command ctrl") {
				buffer->remove(0, 22);
				qDebug() << "Requested list of BLA";

				QByteArray data = encodeBlaList();
				writeAndFlush( socket, data );

			}

			// execute GET BLA DATA: user link command b00F2d0001
			if (buffer->left(19) == "user link command b") {
				int id = 0, device = 0;
				if (!decodeCommandGetBLA( buffer, device, id )) {
					continue;
				}

				qDebug() << QString( "Resuqested data for BLA %1, device = %2" )
							.arg( id ).arg( device ).toStdString().c_str();

				if (device == 622) {
					//$KTPGA,OBJ_ID=1044,2X,P0={42*38'32.14"|41*38'7.81"},P1={42*35'1.21"|41*50'53.51"},END
					QByteArray data;

					data.append( "$KTPGA,OBJ_ID=1044,2X,P0={42*38'32.14\"|41*38'7.81\"},P1={42*35'1.21\"|41*50'53.51\"},END" );

					writeAndFlush( socket, data );
				} else {
					QByteArray data;

					data = encodePlaPosition( id );
					writeAndFlush( socket, data );

					data = encodeBlaAtitude( id );
					writeAndFlush( socket, data );
				}
			}
		}
	}

	if (buffer->length()) {
		qDebug() << "Not processed from client: " << *buffer;
	}
}

long BlaSimulator::encodeAngle( double angle ) {
	int deg = (int)angle;
	double frac = angle - (double)deg;

	frac *= 60.0 ;

	int min = (int)frac;

	frac = frac - (double)min;

	double sec = qRound( frac * 600000.0 );
	sec /= 10000.0 ;

	if (sec >= 60.0) {
		min++ ;
		sec -= 60.0 ;
	}

	long result = deg * 100000 + min * 1000 + sec;

	return result;
}

bool BlaSimulator::decodeCommandGetBLA( QByteArray *buffer, int& device, int& id )
{
	QByteArray temp;

	// get requested id number
	int index = 19;
	char ch = buffer->at( index );
	while (ch >= '0' && ch <= '9') {
		temp.append( ch );

		if (index + 1 >= buffer->length()) break;
		ch = buffer->at( ++index );
	}
	id = temp.toInt();

	// validate request format
	if (index >= buffer->length() || ch != 'd') {
		return false;
	}

	// get requested device number
	temp.clear();
	index++;
	ch = buffer->at( index );
	while (ch >= '0' && ch <= '9') {
		temp.append( ch );

		if (index + 1 >= buffer->length()) break;
		ch = buffer->at( ++index );
	}
	device = temp.toInt();

	buffer->remove(0, index + 1);
	return true;
}

QByteArray BlaSimulator::encodeBlaList()
{
	QByteArray data;

	data.append( QString( 12, ' ' ) );
	data.append( (char*) &_blaCount, 1 );
	data.append( QString( 1, ' ' ) );

	for (int i = 0; i < _blaCount; i++) {
		quint16 id = i + 1;
		data.append( (char*) &id, 2 );
		data.append( QString( 22, ' ' ) );
	}

	return data;
}

QByteArray BlaSimulator::encodePlaPosition( const unsigned int& id )
{
	QByteArray data;

	long lat = encodeAngle( 30.90873456 );
	long lon = encodeAngle( 60.23847545 );

	data.append( "AG" );
	data.append( ((char*) &lat)[2] );
	data.append( ((char*) &lat)[1] );
	data.append( ((char*) &lat)[0] );
	data.append( ((char*) &lon)[2] );
	data.append( ((char*) &lon)[1] );
	data.append( ((char*) &lon)[0] );

	return data;
}

QByteArray BlaSimulator::encodeBlaAtitude(const unsigned int& id)
{
	QByteArray data;

	quint16 altitude = 4235;
	data.append( "AN000000" );
	data.append( ((char*) &altitude)[1] );
	data.append( ((char*) &altitude)[0] );

	return data;
}
