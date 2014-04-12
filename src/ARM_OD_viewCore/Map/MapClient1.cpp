#include <Logger.h>

#include "Map/MapStyleManager.h"
#include "Map/MapClient1.h"

MapClient1::MapClient1(PwGisWidget* pwWidget, Station* station, QObject* parent)
	: QObject( parent )
	, m_mux( QMutex::Recursive )
	, m_niippLayerId( 0 )
	, m_layersCounter( 0 )
{
	m_styleManager = new MapStyleManager( pwWidget->mapProvider()->styleFactory() );

	m_factory = new MapFeature::FeaturesFactory( pwWidget->mapProvider(), m_styleManager );

	m_niippPoint = m_factory->createNiippPoint();
	m_niippLayerName = tr("NIIPP");

	m_circleRadius = 0;
	m_circleChanged = false;
	m_sliceChanged = false;

	m_pwWidget = pwWidget;
	m_mapBounds = m_pwWidget->mapProvider()->mapBounds();
	m_station = station;
	m_mainLatitude = m_station->latitude;
	m_mainLongitude = m_station->longitude;
	m_lastCoord = new QMap<int, PwGisPointList* >;

	m_updateTimer = new QTimer();
	connect( m_updateTimer, SIGNAL( timeout() ), this, SLOT( updatePoints() ) );

	connect( this, SIGNAL( friendBplaAdded( int,QByteArray ) ),
			 this, SLOT( setCurrentPoint( int,QByteArray ) ) );

	connect( this, SIGNAL( enemyBplaAdded( int,QByteArray ) ),
			 this, SLOT( setCurrentPoint( int,QByteArray ) ) );

	connect( this, SIGNAL( interceptionAdded( int, int ) ),
			 this, SLOT( addPerehvatData( int, int ) ) );

	connect( this, SIGNAL( interceptionRemoved( int, int ) ),
			 this, SLOT( removeInterceptionData( int, int ) ) );

	m_uiTimer = new QTimer( this );
	connect( m_uiTimer, SIGNAL( timeout() ), this, SLOT( updateCircle() ) );
	m_uiTimer->setInterval( 100 );
	m_uiTimer->start();

	m_uiTimerSlice = new QTimer( this );
	connect( m_uiTimerSlice, SIGNAL( timeout() ), this, SLOT( updateSlice() ) );
	m_uiTimerSlice->setInterval( 100 );

	m_layerManager = m_pwWidget->mapProvider()->layerManager();

	m_perehvat = new ZInterception( this );
	QThread* thread = new QThread;
	connect( m_perehvat, SIGNAL( finished() ), thread, SLOT( quit() ) );
	connect( m_perehvat, SIGNAL( finished() ), m_perehvat, SLOT( deleteLater() ) );
	connect( thread, SIGNAL( finished() ), thread, SLOT( deleteLater() ) );
	m_perehvat->moveToThread( thread );
	thread->start();

	connect( this, SIGNAL( interceptionPointAdded( int, int, QPointF, float, float, int, float, float) ),
		this, SLOT( addInterceptionPointData( int, int, QPointF, float, float, int, float, float ) ) );
}

MapClient1::~MapClient1()
{
	removeAis();
}

void MapClient1::setNiippController( INiiPPController* niippController )
{
	m_mapNiippController.insert( niippController->getId(),
		niippController );
}

void MapClient1::showLayer( int index, bool state )
{
	if ( m_mapLayers.contains( index ) ) {
		m_layerManager->setLayerVisibility( m_mapLayers.value( index ), state );
	}
}

/// set point from another thread
void MapClient1::addFriendBpla( int id, QByteArray data )
{
	QMutexLocker lock( &m_mux );

	emit friendBplaAdded( id, data );
}

/// set point from another thread
void MapClient1::addEnemyBpla( int id, QByteArray data )
{
	QMutexLocker lock( &m_mux );

	emit enemyBplaAdded( id, data );
}

/// set cur point
void MapClient1::setCurrentPoint( int id, QByteArray data )
{
	QMutexLocker lock( &m_mux );

	m_mapCurPoints.insert( id, data );
}

void MapClient1::addAis( QMap<int, QVector<QString> > vec )
{
	QMutexLocker lock( &m_mux );

	emit aisAdded( vec );
}

void MapClient1::updateNiippPowerSector( int id, double radius, double bis, QByteArray ba )
{
	QMutexLocker lock( &m_mux );

	emit sectorUpdated( id, radius, bis, ba );
}

void MapClient1::updateNiippPowerCicle( int id, double radius, QByteArray ba )
{
	QMutexLocker lock( &m_mux );

	emit cicleUpdated( id, radius, ba );
}

void MapClient1::updatePeleng( int id, int idPost, double lat, double lon, double direction )
{
	QMutexLocker lock( &m_mux );

	emit pelengUpdated( id, idPost, lat, lon, direction );
}

void MapClient1::addInterception(int blaId, int bplaId )
{
	emit interceptionAdded( blaId, bplaId );
}

void MapClient1::removeInterception(int blaId, int bplaId )
{
	emit interceptionRemoved( blaId, bplaId );
}

void MapClient1::addPerehvatPoint(int blaId, int bplaId, QPointF coord,
	float hgt, float radius, int time, float intcCourse, float intcSpeed)
{
	emit interceptionPointAdded( blaId, bplaId, coord, hgt, radius,
		time, intcCourse, intcSpeed );
}

void MapClient1::removePointUvoda()
{
	m_pwWidget->removeMarker( "NIIPMarker" );
}

/// set central control point
void MapClient1::setPoint()
{
	//define style
	//5 - Grid
	PwGisStyle* gridStyle = m_pwWidget->createStyle( "Grid" );
	gridStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	gridStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	gridStyle->setProperty( PwGisStyle::fillColor, "black" );
	gridStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	gridStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	gridStyle->setProperty( PwGisStyle::strokeColor, "black" );
	gridStyle->setProperty( PwGisStyle::layer, m_mapLayers.value(5) );
	gridStyle->apply();
	showLayer(5, false);

	//6 - Control_points
	PwGisStyle* pointsStyle = m_pwWidget->createStyle( "Control_points" );
	pointsStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	pointsStyle->setProperty( PwGisStyle::mapFontSize, "10pt" );
	pointsStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pointsStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pointsStyle->setProperty( PwGisStyle::strokeColor, "black" );
	pointsStyle->setProperty( PwGisStyle::layer, m_mapLayers.value(6) );
	pointsStyle->apply();

	QString mapObjectsSettingFile = QCoreApplication::applicationDirPath();
	mapObjectsSettingFile.append( "/Map/map_objects.ini" );

	/// read settings for generated (positions)
	readStationsFromFile( mapObjectsSettingFile );
}

void MapClient1::addMarkerLayer( int id, QString name )
{
	QString layerId = QString( "layer_%1" ).arg( ++m_layersCounter );
	m_layerManager->addMarkerLayer( layerId, name );
	m_mapLayers.insert( id, layerId );
}

/// set map centred to point
void MapClient1::centerMap()
{
	m_mapBounds->setMapCenter( m_mainLongitude, m_mainLatitude );
}

/// set justify map
void MapClient1::justifyMap()
{
	int h = m_pwWidget->maximumHeight();
	int w = m_pwWidget->maximumWidth();
	m_mapBounds->zoomMapTo( 0, 0, w, h );
}

void MapClient1::updatePoints()
{
	QMutexLocker lock( &m_mux );

	QMap<int, QByteArray>::iterator it;
	for( it = m_mapCurPoints.begin(); it != m_mapCurPoints.end(); ++it ) {
		int id = it.key();
		QString s = QString::number( id );
		s = s.left(2);
		if ( id > 100 ) {
			QDataStream ds( &it.value(), QIODevice::ReadOnly );
			QPointF point;
			ds >> point;
			double alt;
			ds >> alt;
			double speed;
			ds >> speed;
			double course;
			ds >> course;
			int state;
			ds >> state;
			setPointBla( id, point, alt, speed, course, state );
		}
		else if( id >= 50 ) {
			setPointEvil( id, it.value() );
		}
		else if( id > 0 ) {
			QDataStream ds1( &it.value(), QIODevice::ReadOnly );
			QPointF point;
			ds1 >> point;
			setPointEvilPeleng( id, point );
		}

		if ( m_mapBattle.contains( id ) ) {
			int per_id = id;
			int target_id = m_mapBattle.value( per_id );
			QByteArray target_ba = m_mapCurPoints.value( target_id );
			QByteArray per_ba = m_mapCurPoints.value( per_id );
			if ( !( target_ba.size() == 0 || per_ba.size() == 0 ) ) {
				m_perehvat->set(per_id, target_id, per_ba, target_ba);
			}
		}
	}

}

void MapClient1::addNiippLayer( QString id )
{
	m_pwWidget->mapProvider()->layerManager()->addVectorLayer( id, m_niippLayerName );
}

void MapClient1::addPerehvatData( int bla_id, int bpla_id )
{
	m_mapBattle.insert( bla_id, bpla_id );
}

//for timer
void MapClient1::updateCircle()
{
	if ( !m_circleChanged ) {
		return;
	}
	m_circleChanged = false;

	m_pwWidget->addClassicPolygon( "c1",  30.531368, 60.074592,
		m_circleRadius*1000, 40, 0, "ОП1 Гроза", "", "yellow selectAndDrag" );
}

void MapClient1::updateSlice()
{
	if ( !m_sliceChanged ) {
		return;
	}
}

void MapClient1::init()
{
	// create marker layers
	this->addMarkerLayer( 0, tr( "OP" ) );//0 - stations
	this->addMarkerLayer( 1, tr( "UAV_enemy" ) ); //1 - BPLA profile
	this->addMarkerLayer( 2, tr( "UAV" ) ); //2 - BLA profile
	this->addMarkerLayer( 3, tr( "Atlant" ) ); //3 - Pelengators
	this->addMarkerLayer( 4, tr( "Atlant target" ) ); //4 - PelengatorsPoint
	this->addMarkerLayer( 5, tr( "Grid" ) ); //5 - Grid
	this->addMarkerLayer( 6, tr( "Checkpoints" ) ); //6 - Control_points
	this->addMarkerLayer( 7, tr( "Interception point" ) ); //7 - Perehvat
	this->addMarkerLayer( 8, tr( "Civil ships" ) ); //8 - AIS - civil plane
	this->addMarkerLayer( 9, tr( "Diversion points" ) ); //9 - NIIPPPoint
	this->addMarkerLayer( 10, tr( "SPIP DD" ) ); //10 - NIIPP

	// create styles for features
	m_styleManager->createStationStyle( m_mapLayers.value(0) )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(1) )->apply();
	m_styleManager->createFriendBplaStyle( m_mapLayers.value(2) )->apply();
	m_styleManager->createPelengatorStyle( m_mapLayers.value(3) )->apply();
	m_styleManager->createPelengatorPointStyle( m_mapLayers.value(4) )->apply();
	m_styleManager->createInterceptionStyle( m_mapLayers.value(7) )->apply();
	m_styleManager->createAisStyle( m_mapLayers.value(8) )->apply();
	m_styleManager->createNiippPointStyle( m_mapLayers.value(9) )->apply();
	m_styleManager->createNiippStyle( m_mapLayers.value(10) )->apply();

	QString niippLayerId = m_niippLayerName + QString::number( m_niippLayerId );
	addNiippLayer( niippLayerId );

	foreach( IMapObjectInfo* value, m_mapObjects ) {
		dynamic_cast<Sector*>(value)->updateMap();
	}

	m_updateTimer->start( 1000 );

	connect( this, SIGNAL( aisAdded( QMap<int, QVector<QString> >) ),
			 this, SLOT( setAisData( QMap<int, QVector<QString> > ) ) );

	connect( this, SIGNAL( pelengUpdated( int, int, double, double, double ) ),
			 this, SLOT( addPeleng( int, int, double, double, double ) ) );

	connect( this, SIGNAL( sectorUpdated( int, double, double, QByteArray ) ),
			 this, SLOT( addNiippDirected( int, double, double, QByteArray ) ) );

	connect( this, SIGNAL( cicleUpdated( int, double, QByteArray ) ),
			 this, SLOT( addNiippNotDirected( int, double, QByteArray ) ) );

	connect( m_pwWidget, SIGNAL( mapClicked( double, double ) ),
			 this, SLOT( mapMouseClicked( double, double ) ) );
}

/// get coordinates
void MapClient1::mapMouseClicked( double lon, double lat )
{
	if( !m_niippPoint ) {
		m_niippPoint = m_factory->createNiippPoint( );
	}

	QPointF position( lon, lat );
	m_niippPoint->setPosition( position );
	m_niippPoint->updateMap();

	m_mapNiippController.value( 100 )->setPoint( position );
	m_mapNiippController.value( 101 )->setPoint( position );
}

void MapClient1::removeInterceptionData( int friendBplaId, int enemyBplaId )
{
	Q_UNUSED( enemyBplaId );

	m_mapBattle.remove( friendBplaId );
}


// --- draw features on the map ---

//add BLA
/// set point in this thread
void MapClient1::setPointBla( int id, QPointF point, double alt, double speed,
	double course, int state )
{
	Q_UNUSED( alt );
	Q_UNUSED( speed );
	Q_UNUSED( course );
	Q_UNUSED( state );

	QMutexLocker lock( &m_mux );

	point = QPointF(point.y(), point.x());

	MapFeature::FriendBpla* bpla = m_friendBplaList.value( id, NULL );
	if( bpla != NULL ) {
		// update, if BPLA already added and position changed
		if( bpla->position() == point ) return;

		bpla->setPosition( point );
	} else {
		// else create new one
		bpla = m_factory->createFriendBpla( id, point );
		m_friendBplaList.insert( id, bpla );
	}

	bpla->updateMap();
}

//add BPLA - Evil
/// set point in this thread
void MapClient1::setPointEvil( int id, QByteArray data )
{
	QDataStream ds( &data, QIODevice::ReadOnly );
	int time;
	ds >> time;

	int state;
	ds >> state;

	QPointF sko;
	ds >> sko;

	QVector<QPointF> track;
	ds >> track;

	double speed;
	ds >> speed;

	double alt;
	ds >> alt;

	double bearing;
	ds >> bearing;

	MapFeature::EnemyBpla* bpla = m_enemyBplaList.value( id, NULL );
	if( bpla != NULL ) {
		bpla->update( alt, sko, track );
	} else {
		bpla = m_factory->createEnemyBpla( id, sko, track, alt );
		m_enemyBplaList.insert( id, bpla );
		bpla->updateMap();
	}

	if ( !track.size() ) return;

	m_mapNiippController.value( 100 )->sendEnemyBpla( track.at( track.size()-1 ),
		m_niippPoint->position(), alt, bearing );
	m_mapNiippController.value( 101 )->sendEnemyBpla( track.at( track.size()-1 ),
		m_niippPoint->position(), alt, bearing );
}

//add AIS
void MapClient1::setAisData( QMap<int, QVector<QString> > data )
{
	QMap<QString, MapFeature::Ais*> newAisList;

	QMap<int, QVector<QString> >::iterator it;
	for( it = data.begin(); it != data.end(); ++it ) {
		bool doubleOK;

		QVector<QString> vec = it.value();

		double lon = vec.at(1).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong longitude (%1) for AIS %2" ).arg( vec.at(1), vec.at(0) ) );
			continue;
		}

		double lat = vec.at(2).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong latitude (%1) for AIS %2" ).arg( vec.at(2), vec.at(0) ) );
			continue;
		}

		double course = vec.at(5).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong course (%1) for AIS %2" ).arg( vec.at(5), vec.at(0) ) );
			continue;
		}

		QString name = vec.at(0);
		QPointF position( lon, lat );

		// try to pass or update this AIS, if it exists
		MapFeature::Ais* ais = m_aisList.value( name, NULL );
		if( ais != NULL ) {
			m_aisList.remove( name );
			newAisList.insert( name, ais );

			if( ais->name() == name && ais->position() == position && ais->course() == course ) {
				continue;
			}

			// if new data is not same, update it
			ais->setName( name );
			ais->setPosition( position );
			ais->setCourse( course );
			ais->updateMap();
			continue;
		}

		ais = m_factory->createAis( name, position, course );
		newAisList.insert( name, ais );
	}

	// now remove remains AIS from old list
	removeAis();

	m_aisList = newAisList;
}

void MapClient1::removeAis()
{
	QMap<QString, MapFeature::Ais*>::iterator i;
	for( i = m_aisList.begin(); i != m_aisList.end(); ++i ) {
		delete i.value();
	}
}

void MapClient1::readStationsFromFile(QString fileName)
{
	QTextCodec* codec = QTextCodec::codecForName( "UTF-8" );
	QSettings stationSettings( fileName, QSettings::IniFormat );

	stationSettings.setIniCodec( codec );

	QString name;
	double lat;
	double lon;
	QPointF position;

	QStringList childKeys = stationSettings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		stationSettings.beginGroup( childKey );

		name = stationSettings.value( "Name", 0 ).toString();
		lat = stationSettings.value( "Latitude", "0" ).toDouble();
		lon = stationSettings.value( "Longitude", "0" ).toDouble();

		position = QPointF( lon, lat );

		if( name != tr( "Ingur" ) ) {
			MapFeature::Station* station = m_stationList.value( name, NULL );

			if( station = NULL ) {
				station->setPosition( position );
			} else {
				station = m_factory->createStation( name, position );
				m_stationList.insert( name, station );
			}

			station->updateMap();
		}

		stationSettings.endGroup();
	}
}

void MapClient1::addPeleng( int id, int idPost, double lat, double lon, double direction )
{
	Q_UNUSED( id );
	QPointF position( lon, lat );

	// add or update pelengator
	MapFeature::Pelengator* p = m_pelengatorList.value( idPost, NULL );
	if( p != NULL ) {
		p->update( position, direction );
	} else {
		p = m_factory->createPelengator( idPost, position, direction );
		p->updateMap();

		m_pelengatorList.insert( idPost, p );
	}
}

void MapClient1::setPointEvilPeleng( int id, QPointF point )
{
	// add or update pelengator point
	MapFeature::PelengatorPoint* p = m_pelengatorPointsList.value( id, NULL );
	if( p != NULL ) {
		p->setPosition( point );
	} else {
		QString name = tr("UAV_enemy Atlant(#") + QString::number(id) + ")\\n";
		p = m_factory->createPelengatorPoint( name, point );
		m_pelengatorPointsList.insert( id, p );
	}

	p->updateMap();
}

void MapClient1::addNiippDirected( int id, double radius, double angle, QByteArray data )
{
	addNiip( id, MapFeature::Niipp::Directed, radius, data, angle );
}

void MapClient1::addNiippNotDirected( int id, double radius, QByteArray data )
{
	addNiip( id, MapFeature::Niipp::NotDirected, radius, data );
}

void MapClient1::addNiip(int id, MapFeature::Niipp::Mode mode, double radius, QByteArray data,
						 double angle)
{
	// get position from bytes
	QDataStream stream( data );

	QString name;
	stream >> name;

	QPointF position;
	stream >> position;

	// add or update NIIPP
	MapFeature::Niipp* niipp = m_niippList.value( id, NULL );
	if( niipp != NULL ) {
		niipp->update( position, mode, radius, angle );
	} else {
		niipp = m_factory->createNiipp( id, position, mode, radius, angle );
		niipp->updateMap();

		m_niippList.insert( id, niipp );
	}
}

void MapClient1::addInterceptionPointData( int friendBplaId, int enemyBplaId, QPointF position,
	float height, float radius, int time, float course, float speed )
{
	Q_UNUSED( time );

	QString key = QString( "%1-%2" ).arg( friendBplaId ).arg( enemyBplaId );

	MapFeature::Interception* intc = m_interceptionList.value( key, NULL );
	if( intc != NULL ) {
		intc->update( position, height, radius, course, speed );
	} else {
		intc = m_factory->createInterception( friendBplaId, enemyBplaId, position, height, radius, course,
											  speed );
		intc->updateMap();
		m_interceptionList.insert( key, intc );
	}
}
