#include <QVariant>

#include <Logger.h>

#include "Map/MapStyleManager.h"
#include "Map/MapClient1.h"

#define clearObjectsList(type, map) foreach( type* item, map ) { delete item; } map.clear();

MapClient1::MapClient1(PwGisWidget* pwWidget, Station* station, QObject* parent)
	: QObject( parent )
	, m_mux( QMutex::Recursive )
{
	m_styleManager = new MapStyleManager( pwWidget->mapProvider()->styleFactory() );

	m_factory = new MapFeature::FeaturesFactory( pwWidget->mapProvider(), m_styleManager );

	m_niippPoint = m_factory->createNiippPoint();

	m_circleRadius = 0;
	m_circleChanged = false;

	m_pwWidget = pwWidget;
	m_bounds = m_pwWidget->mapProvider()->mapBounds();
	m_station = station;

	/// \todo I don't know what is there for these timers, maybe it was some code for demo?
	m_uiTimer = new QTimer( this );
	connect( m_uiTimer, SIGNAL( timeout() ), this, SLOT( updateCircle() ) );
	m_uiTimer->setInterval( 100 );
	//m_uiTimer->start();

	m_interception = new ZInterception( this );
	QThread* thread = new QThread;
	connect( m_interception, SIGNAL( finished() ), thread, SLOT( quit() ) );
	connect( m_interception, SIGNAL( finished() ), m_interception, SLOT( deleteLater() ) );
	connect( thread, SIGNAL( finished() ), thread, SLOT( deleteLater() ) );
	m_interception->moveToThread( thread );
	thread->start();

	connect( &m_bplaRedrawTimer, SIGNAL( timeout() ), SLOT( redrawAllBpla()) );
	m_bplaRedrawTimer.start( 1000 );

	connect( this, SIGNAL(friendBplaAdded(UavInfo)),
			 this, SLOT(addFriendBplaInternal(UavInfo)) );

	connect( this, SIGNAL(enemyBplaAdded(UavInfo)),
			 this, SLOT(addEnemyBplaInternal(UavInfo)) );

	connect( this, SIGNAL(bplaRemoved(Uav)),
			 this, SLOT(removeBplaInternal(Uav)) );

	connect( this, SIGNAL(niippPowerZoneUpdated(Niipp)),
			 this, SLOT(addNiipInternal(Niipp)) );

	connect( this, SIGNAL(interceptionAdded(int, int)),
			 this, SLOT(addPerehvatData(int, int)) );

	connect( this, SIGNAL(interceptionRemoved(int, int)),
			 this, SLOT(removeInterceptionData(int, int)) );

	connect( this, SIGNAL(interceptionPointAdded(int, int, QPointF, float, float, int, float, float)),
			 this, SLOT(addInterceptionPointData(int, int, QPointF, float, float, int, float, float)) );
}

MapClient1::~MapClient1()
{
	delete m_niippPoint;
	delete m_factory;
	delete m_styleManager;

	removeAll();
}

void MapClient1::init()
{
	m_mapLayers.clear();
	removeAll();

	// create marker layers
	this->addMarkerLayer( 0, "layer_0_OP", tr( "OP" ) );//0 - stations
	this->addMarkerLayer( 1, "layer_1_UAV_enemy", tr( "UAV_enemy" ) ); //1 - BPLA profile
	this->addMarkerLayer( 2, "layer_2_UAV", tr( "UAV" ) ); //2 - BLA profile
	this->addMarkerLayer( 3, "layer_3_Atlant", tr( "Atlant" ) ); //3 - Pelengators
	this->addMarkerLayer( 4, "layer_4_Atlant_target", tr( "Atlant target" ) ); //4 - PelengatorsPoint
	this->addMarkerLayer( 5, "layer_5_Grid", tr( "Grid" ) ); //5 - Grid
	this->addMarkerLayer( 6, "layer_6_Checkpoints", tr( "Checkpoints" ) ); //6 - Control_points
	this->addMarkerLayer( 7, "layer_7_Interception_point", tr( "Interception point" ) ); //7 - Perehvat
	this->addMarkerLayer( 8, "layer_8_Civil_ships", tr( "Civil ships" ) ); //8 - AIS - civil plane
	this->addMarkerLayer( 9, "layer_9_Diversion_points", tr( "Diversion points" ) ); //9 - NIIPPPoint
	this->addMarkerLayer( 10, "layer_10_SPIP_DD", tr( "SPIP DD" ) ); //10 - NIIPP

	showLayer( 5, false );

	// create styles for features
	m_styleManager->createStationStyle( m_mapLayers.value(0) )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(1) )->apply();
	m_styleManager->createFriendBplaStyle( m_mapLayers.value(2) )->apply();
	m_styleManager->createFriendBplaSliceStyle( m_mapLayers.value(2) )->apply();
	m_styleManager->createPelengatorStyle( m_mapLayers.value(3) )->apply();
	m_styleManager->createPelengatorPointStyle( m_mapLayers.value(4) )->apply();
	m_styleManager->createGridStyle( m_mapLayers.value(5) )->apply();
	m_styleManager->createCheckPointStyle( m_mapLayers.value(6) )->apply();
	m_styleManager->createInterceptionStyle( m_mapLayers.value(7) )->apply();
	m_styleManager->createAisStyle( m_mapLayers.value(8) )->apply();
	m_styleManager->createNiippPointStyle( m_mapLayers.value(9) )->apply();
	m_styleManager->createNiippStyle( m_mapLayers.value(10) )->apply();

	//addNiippLayer
	m_pwWidget->mapProvider()->layerManager()->addVectorLayer( "layer_11_NIIPP", tr("NIIPP") );

	connect( this, SIGNAL( aisAdded( QMap<int, QVector<QString> >) ),
			 this, SLOT( setAisData( QMap<int, QVector<QString> > ) ) );

	connect( this, SIGNAL( pelengUpdated( int, int, double, double, double ) ),
			 this, SLOT( addPeleng( int, int, double, double, double ) ) );
}

void MapClient1::showLayer( int index, bool state )
{
	if ( m_mapLayers.contains( index ) ) {
		m_pwWidget->mapProvider()->layerManager()->setLayerVisibility( m_mapLayers.value( index ), state );
	}
}

void MapClient1::addFriendBpla(const UavInfo& uav)
{
	emit friendBplaAdded( uav );
}

void MapClient1::addEnemyBpla(const UavInfo& uav)
{
	emit enemyBplaAdded( uav );
}

void MapClient1::removeBpla(const Uav& uav)
{
	emit bplaRemoved( uav );
}

void MapClient1::addAis( QMap<int, QVector<QString> > vec )
{
	emit aisAdded( vec );
}

void MapClient1::updatePeleng( int id, int idPost, double lat, double lon, double direction )
{
	emit pelengUpdated( id, idPost, lat, lon, direction );
}

void MapClient1::removeAll()
{
	clearObjectsList( MapFeature::Ais, m_aisList );
	clearObjectsList( MapFeature::FriendBpla, m_friendBplaList );
	clearObjectsList( MapFeature::EnemyBpla, m_enemyBplaList );
	clearObjectsList( MapFeature::Niipp, m_niippList );
	clearObjectsList( MapFeature::Pelengator, m_pelengatorList );
	clearObjectsList( MapFeature::PelengatorPoint, m_pelengatorPointsList );
	clearObjectsList( MapFeature::Interception, m_interceptionList );
	clearObjectsList( MapFeature::Station, m_stationList );
	clearObjectsList( MapFeature::CheckPoint, m_checkPointsList );
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

void MapClient1::removeNiippPoint()
{
	delete m_niippPoint;
}

void MapClient1::readCheckPointsFromFile(QString fileName)
{
	QSettings settings( fileName, QSettings::IniFormat );
	settings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	QStringList childKeys = settings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		settings.beginGroup( childKey );

		QVariant name = settings.value( "Name" );
		QVariant lat = settings.value( "Latitude" );
		QVariant lon = settings.value( "Longitude" );

		if( name.isNull() || !name.canConvert( QVariant::String ) ) continue;
		if( lat.isNull() || !lat.canConvert( QVariant::Double ) ) continue;
		if( lon.isNull() || !lon.canConvert( QVariant::Double ) ) continue;

		MapFeature::CheckPoint *p = m_factory->createCheckPoint(
					name.toString(), QPointF( lon.toDouble(), lat.toDouble() ) );

		p->updateMap();
		m_checkPointsList << p;

		settings.endGroup();
	}
}

void MapClient1::setPoint()
{
	QString mapObjectsSettingFile = QCoreApplication::applicationDirPath();
	mapObjectsSettingFile.append( "/Map/map_objects.ini" );

	// read settings for generated (positions)
	readStationsFromFile( mapObjectsSettingFile );

	QString mapPointsSettingFile = QCoreApplication::applicationDirPath();
	mapPointsSettingFile.append( "/Map/map_points.ini" );

	readCheckPointsFromFile( mapPointsSettingFile );
}

void MapClient1::addMarkerLayer( int id, const QString& layerId,
	const QString& name )
{
	m_pwWidget->mapProvider()->layerManager()->addMarkerLayer( layerId, name );
	m_mapLayers.insert( id, layerId );
}

/// set map centred to point
void MapClient1::centerMap()
{
	m_bounds->setMapCenter( m_station->longitude, m_station->latitude );
}

void MapClient1::justifyMap()
{
	int h = m_pwWidget->maximumHeight();
	int w = m_pwWidget->maximumWidth();
	m_bounds->zoomMapTo( 0, 0, w, h );
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

void MapClient1::addNiippPoint( const QPointF& point )
{
	if( !m_niippPoint ) {
		m_niippPoint = m_factory->createNiippPoint( );
	}

	m_niippPoint->setPosition( point );
	m_niippPoint->updateMap();
}

void MapClient1::updateNiippPowerZone(const Niipp& niipp)
{
	emit niippPowerZoneUpdated( niipp );
}

void MapClient1::removeInterceptionData( int friendBplaId, int enemyBplaId )
{
	Q_UNUSED( enemyBplaId );

	m_mapBattle.remove( friendBplaId );
}

void MapClient1::addFriendBplaInternal(const UavInfo& uav)
{
	MapFeature::FriendBpla* bpla = m_friendBplaList.value( uav.uavId, NULL );

	switch( uav.sourceType ) {
		// if we get data about UAV from slices source...
		case UavSlicesSource:
			// and UAV exists, update slice track
			if( bpla != NULL ) {
				bpla->setSlice( QPointF( uav.lon, uav.lat ) );
			}

			return;

		case UavAutopilotSource:
			if( bpla != NULL ) {
				// update, if BPLA already added and position changed
				bpla->update( uav );
			} else {
				// else create new one
				bpla = m_factory->createFriendBpla( uav );
				m_friendBplaList.insert( uav.uavId, bpla );
			}

			return;
	}
}

void MapClient1::addEnemyBplaInternal(const UavInfo& uav)
{
	MapFeature::EnemyBpla* bpla = m_enemyBplaList.value( uav.uavId, NULL );

	if( bpla != NULL ) {
		bpla->update( uav );
	} else {
		bpla = m_factory->createEnemyBpla( uav );
		m_enemyBplaList.insert( uav.uavId, bpla );
	}
}

void MapClient1::removeBplaInternal(const Uav& uav)
{
	if( m_friendBplaList.contains( uav.uavId ) ) {
		delete m_friendBplaList.take( uav.uavId );
	}

	if( m_enemyBplaList.contains( uav.uavId ) ) {
		delete m_enemyBplaList.take( uav.uavId );
	}
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
	clearObjectsList( MapFeature::Ais, m_aisList );

	m_aisList = newAisList;
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

void MapClient1::addPeleng(int id, int idPost, double lat, double lon, double direction)
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

void MapClient1::addNiipInternal( const Niipp& niipp )
{
	// add or update NIIPP
	MapFeature::Niipp* niippFeature = m_niippList.value( niipp.getId(), NULL );
	if( niippFeature != NULL ) {
		niippFeature->update( niipp );
	} else {
		niippFeature = m_factory->createNiipp( niipp );
		niippFeature->updateMap();

		m_niippList.insert( niipp.getId(), niippFeature );
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

void MapClient1::redrawAllBpla()
{
	foreach( MapFeature::FriendBpla* bpla, m_friendBplaList ) {
		bpla->updateMap();
	}

	foreach( MapFeature::EnemyBpla* bpla, m_enemyBplaList ) {
		bpla->updateMap();
	}
}
