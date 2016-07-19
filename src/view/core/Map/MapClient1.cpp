#include <QVariant>

#include <Logger/Logger.h>
#include <math.h>

#include "Uav/UavModel.h"
#include "Map/MapStyleManager.h"
#include "Map/MapClient1.h"

#define clearObjectsList(type, map) foreach( type* item, map ) { delete item; } map.clear();

#define HYPERBOLE_LIFE_TIME 5000

MapClient1::MapClient1(MapWidget* pwWidget, Station* station, QObject* parent)
	: QObject( parent )
	, m_mux( QMutex::Recursive )
	, m_niippPoint( 0 )
{
	m_view = pwWidget;
	m_pwWidget = pwWidget->getPwGis();

	m_styleManager = new MapStyleManager( m_pwWidget->mapProvider()->styleFactory() );
	m_objectsManager = m_pwWidget->mapProvider()->objectsManager();

	m_factory = new MapFeature::FeaturesFactory(m_view, m_pwWidget->mapProvider(), m_styleManager );

	m_niippPoint = m_factory->createNiippPoint();

	m_circleRadius = 0;
	m_circleChanged = false;

	m_bounds = m_pwWidget->mapProvider()->mapBounds();
	m_station = station;

	m_hyperboleTimer = new QTimer(this);
	connect( m_hyperboleTimer, SIGNAL(timeout()), this, SLOT(removeAllHyperbole()) );

	m_onePointTimer = new QTimer(this);
	connect( m_onePointTimer, SIGNAL(timeout()), this, SLOT(removeAllonePointTimer()) );
	m_onePointTimer->start(3000);

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

	connect( this, SIGNAL(enemyBplaAdded(UavInfo,QVector<QPointF>,QVector<QPointF>)),
			 this, SLOT(addEnemyBplaInternal(UavInfo,QVector<QPointF>,QVector<QPointF>)) );

	connect( this, SIGNAL(bplaRemoved(Uav)),
			 this, SLOT(removeBplaInternal(Uav)) );

	connect( this, SIGNAL(niippPowerZoneUpdated(Niipp)),
			 this, SLOT(addNiipInternal(Niipp)) );

	connect( this, SIGNAL(interceptionAdded(int, int, QList<UavInfo>&, QList<UavInfo>&)),
			 this, SLOT(addPerehvatData(int, int, QList<UavInfo>&, QList<UavInfo>&)) );

	connect( this, SIGNAL(interceptionRemoved(int, int)),
			 this, SLOT(removeInterceptionData(int, int)) );

	connect( this, SIGNAL(hyperboleAdded(QByteArray,QColor)),
			 this, SLOT(addHyperboleInternal(QByteArray,QColor)) );

	connect( this, SIGNAL(interceptionPointAdded(int, int, QPointF, float, float, int, float, float)),
			 this, SLOT(addInterceptionPointData(int, int, QPointF, float, float, int, float, float)) );

	connect(&m_objectsManager->events(), SIGNAL(featureClicked(QString,QString)), SLOT(onFeatureClicked(QString,QString)));
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
	addMarkerLayer( 0, "OP", tr( "OP" ) );

	addMarkerLayer( 6, "Atlant", tr( "Atlant" ) );
	addMarkerLayer( 7, "Atlant_target", tr( "Atlant target" ) );
	addMarkerLayer( 8, "Grid", tr( "Grid" ) );
	addMarkerLayer( 9, "Checkpoints", tr( "Checkpoints" ) );
	addMarkerLayer( 10, "Interception_point", tr( "Interception point" ) );
	addMarkerLayer( 11, "Civil_ships", tr( "Civil ships" ) );
	addMarkerLayer( 12, "Diversion_points", tr( "Diversion points" ) );
	addMarkerLayer( 13, "SPIP_DD", tr( "SPIP DD" ) );
	addMarkerLayer( 14, "Hyperbole", tr( "Hyperbole" ) );
	addMarkerLayer( 15, "History", tr( "History" ) );

	addMarkerLayer( 101, "UAV_enemy_track_manual", tr( "UAV_enemy_track_manual" ) );
	addMarkerLayer( 100, "UAV_enemy_manual", tr( "UAV_enemy_manual" ) );

	addMarkerLayer( 103, "UAV_enemy_track_auto", tr( "UAV_enemy_track_auto" ) );
	addMarkerLayer( 102, "UAV_enemy_auto", tr( "UAV_enemy_auto" ) );

	addMarkerLayer( 104, "UAV_enemy_single", tr( "UAV_enemy_single" ) );

	addMarkerLayer( 5, "UAV_track_KTR", tr( "UAV_track_KTR" ) );
	addMarkerLayer( 4, "UAV_track_autopilot", tr( "UAV_track_autopilot" ) );
	addMarkerLayer( 3, "UAV", tr( "UAV" ) );

	addMarkerLayer( 202, "ADSB", tr( "ADSB" ) );
	addMarkerLayer( 203, "ADSBtrack", tr( "ADSBtrack" ) );

	showLayer( 8, false );

	// create styles for features
	m_styleManager->createStationStyle( m_mapLayers.value(0) )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(100), UAV_SOLVER_MANUAL_SOURCE )->apply();
	m_styleManager->createEnemyBplaTrackStyle( m_mapLayers.value(101), UAV_SOLVER_MANUAL_SOURCE )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(102), UAV_SOLVER_AUTO_SOURCE )->apply();
	m_styleManager->createEnemyBplaTrackStyle( m_mapLayers.value(103), UAV_SOLVER_AUTO_SOURCE )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(104), UAV_SOLVER_SINGLE_1_SOURCE )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(104), UAV_SOLVER_SINGLE_2_SOURCE )->apply();

	m_styleManager->createFriendBplaStyle( m_mapLayers.value(3) )->apply();
	m_styleManager->createFriendBplaTrackStyle( m_mapLayers.value(4) )->apply();
	m_styleManager->createFriendBplaSliceStyle( m_mapLayers.value(5) )->apply();

	m_styleManager->createPelengatorStyle( m_mapLayers.value(6) )->apply();
	m_styleManager->createPelengatorPointStyle( m_mapLayers.value(7) )->apply();
	m_styleManager->createGridStyle( m_mapLayers.value(8) )->apply();
	m_styleManager->createCheckPointStyle( m_mapLayers.value(9) )->apply();
	m_styleManager->createInterceptionStyle( m_mapLayers.value(10) )->apply();
	m_styleManager->createAisStyle( m_mapLayers.value(11) )->apply();
	m_styleManager->createAdsbStyle( m_mapLayers.value(202) )->apply();
	m_styleManager->createAdsbTrackStyle( m_mapLayers.value(203) )->apply();
	m_styleManager->createNiippPointStyle( m_mapLayers.value(12) )->apply();
	m_styleManager->createNiippStyle( m_mapLayers.value(13) )->apply();
	m_styleManager->createHyperboleStyle( m_mapLayers.value(14) )->apply();
	m_styleManager->createHistoryStyle( m_mapLayers.value(15) )->apply();

	//addNiippLayer
	m_pwWidget->mapProvider()->layerManager()->addVectorLayer( "NIIPP", tr("NIIPP") );

	connect( this, SIGNAL( aisAdded( QMap<int, QVector<QString> >) ),
			 this, SLOT( setAisData( QMap<int, QVector<QString> > ) ) );

	connect( this, SIGNAL( pelengUpdated( int, int, double, double, double ) ),
			 this, SLOT( addPeleng( int, int, double, double, double ) ) );

	connect( this, SIGNAL(adsbAdded(QByteArray)), this, SLOT(setAdsbData(QByteArray)) );
}

void MapClient1::addHyperboles(const QByteArray& data, const QColor color)
{
	emit hyperboleAdded(data, color);
}

void MapClient1::addHyperboleInternal(const QByteArray& data, const QColor color )
{
	QDataStream ds(data);

	m_hyperboleTimer->start(HYPERBOLE_LIFE_TIME);

	double frequency;
	ds >> frequency;

	QTime time;
	ds >> time;

	QList<QVector<QPointF>> list;
	ds >> list;

	// hide no needed hyporboles
	if ( m_hyperboleList.count() > list.count() ) {
		for ( int i = list.count(); i < m_hyperboleList.count(); i++ ) {
			m_hyperboleList[i]->removeFromMap();
		}
	}

	for ( int i = 0; i < list.count(); ++i ) {
		if( i < m_hyperboleList.count() ) {
			m_hyperboleList[i]->updatePath( list[i], time );
		} else {
			MapFeature::Hyperbole* hyperbole;
			hyperbole = m_factory->createHyperbole( list[i], time, color );
			m_hyperboleList << hyperbole;
		}
	}
}

void MapClient1::removeAllHyperbole()
{
	for ( int i = 0; i < m_hyperboleList.count(); i++ ) {
		m_hyperboleList[i]->removeFromMap();
	}
}

void MapClient1::removeAllonePointTimer()
{
		foreach (MapFeature::EnemyBpla* bla, m_enemyBplaList) {
			if( bla->getStyleName() == MapStyleManager::getEnemyBplaStyleName(104) ) {
				bla->removeFromMap();
				m_enemyBplaList.remove(m_enemyBplaList.key(bla));
			}
		}
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

void MapClient1::addEnemyBpla(const UavInfo& uav,
							  const QVector<QPointF>& tail, const QVector<QPointF>& tailStdDev)
{
	emit enemyBplaAdded( uav, tail, tailStdDev );
}

void MapClient1::removeBpla(const Uav& uav)
{
	emit bplaRemoved( uav );
}

void MapClient1::addAis( QMap<int, QVector<QString> > vec )
{
	emit aisAdded( vec );
}

void MapClient1::addAdsb(QByteArray data)
{
	emit adsbAdded(data);
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
	clearObjectsList( MapFeature::Hyperbole, m_hyperboleList );
}

void MapClient1::addInterception(int blaId, int bplaId, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList )
{
	emit interceptionAdded( blaId, bplaId, blaInfoList, bplaInfoList );
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
	if(m_niippPoint != 0) {
		delete m_niippPoint;
	}

	m_niippPoint = 0;
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

void MapClient1::addPerehvatData( int bla_id, int bpla_id, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList )
{
	m_mapBattle.insert( bla_id, bpla_id );
	QByteArray target;
	QByteArray per;
	QDataStream ds1(&per, QIODevice::WriteOnly);
	QDataStream ds(&target, QIODevice::WriteOnly);
	UavInfo tempInfo;

	tempInfo = blaInfoList.last();
	QPointF point1;
	point1.setY(tempInfo.lat);
	point1.setX(tempInfo.lon);
	ds1 << point1;
	double alt1 = tempInfo.alt;
	ds1 << alt1;
	double speed1 = tempInfo.speed;
	ds1 << speed1;
	double course1 = tempInfo.yaw;
	ds1 << course1;
	//?
	int state1 = 1; //set by default. Don't need in interception counting
	ds1 << state1;

	tempInfo = bplaInfoList.last();
	int time = 1; //set by default
	ds << time;
	int state = 1; //set by default
	ds << state;
	QPointF sko;
	sko.setY(tempInfo.lat);
	sko.setX(tempInfo.lon);
	ds << sko;
	QVector<QPointF> track;
	track.append(sko);
	ds << track;
	double speed = tempInfo.speed;
	ds << speed;
	double alt = tempInfo.alt;
	ds << alt;
	double bearing = tempInfo.yaw;
	ds << bearing;

	m_interception->set(bla_id, bpla_id, per, target);
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
	m_mapBattle.remove( friendBplaId );

	//removing interception map feature area from map
	QString interceptionKey = QString( "%1-%2" ).arg( friendBplaId ).arg( enemyBplaId );
	if(m_interceptionList.contains(interceptionKey)) {
		delete m_interceptionList.take( interceptionKey );
	}
}

void MapClient1::addFriendBplaInternal(const UavInfo& uav)
{
	QString id = UavModel::getExtendedId( uav );
	MapFeature::FriendBpla* bpla = m_friendBplaList.value( id, NULL );

	m_uavKnownSources << uav.source;

	switch( uav.source ) {
		// if we get data about UAV from slices source...
		case UAV_SLICES_SOURCE:
			// and UAV exists, update slice track
			if( bpla != NULL ) {
				bpla->setSlice( QPointF( uav.lon, uav.lat ) );
			} else {
				// else create new one
				bpla = m_factory->createFriendBpla( uav );
				m_friendBplaList.insert( id, bpla );
			}

			return;

		case UAV_AUTOPILOT_SOURCE:
			if( bpla != NULL ) {
				// update, if BPLA already added and position changed
				bpla->update( uav );
			} else {
				// else create new one
				bpla = m_factory->createFriendBpla( uav );
				m_friendBplaList.insert( id, bpla );
			}

			return;
	}
}

void MapClient1::addEnemyBplaInternal(const UavInfo& uav,
									  const QVector<QPointF>& tail,
									  const QVector<QPointF>& tailStdDev)
{
	QString id = UavModel::getExtendedId( uav );
	MapFeature::EnemyBpla* bpla = m_enemyBplaList.value( id, NULL );

	m_uavKnownSources << uav.source;

	if( bpla != NULL ) {
		bpla->update( uav );
	} else {
		bpla = m_factory->createEnemyBpla( uav );
		m_enemyBplaList.insert( id, bpla );
	}

	if( !uav.historical ) {
		bpla->setTail( tail, tailStdDev );
	}

	bpla->updateMap();
}

void MapClient1::removeBplaInternal(const Uav& uav)
{
	foreach(uint source, m_uavKnownSources) {
		QString id = UavModel::getExtendedId( uav, source );

		if( m_friendBplaList.contains( id ) ) {
			delete m_friendBplaList.take( id );
		}

		if( m_enemyBplaList.contains( id ) ) {
			delete m_enemyBplaList.take( id );
		}
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

void MapClient1::setAdsbData(QByteArray data)
{
	ADSBData::Packet packet;
	packet.ParseFromArray( data.data(), data.size() );

	if(packet.board_size() <= 0) {
		return;
	}

	foreach (ADSBData::Packet_Board var, packet.board()) {

		QString id;
		double lon;
		double lat;
		double yaw = 0;

		id = QString::fromStdString(var.id());
		lon = var.lon();
		lat = var.lat();

		if(var.has_yaw()) {
			yaw = var.yaw();
		}


		// add or update pelengator
		MapFeature::ADSBPlaneFeature* p = m_adsbList.value( id, NULL );
		if( p != NULL ) {
			if(lon > 180 || lon < -180) {
				return;
			}
			if(lat > 90 || lat < -90) {
				return;
			}


			if( lat != lat || lon != lon) {
				return;
			}

			p->update(QPointF(lon, lat), yaw);
			p->updateMap();
		} else {
			if(lon > 180 || lon < -180) {
				return;
			}
			if(lat > 90 || lat < -90) {
				return;
			}

			if( lat != lat ||
					lon != lon) {
				return;
			}

			p = m_factory->createAdsbPlane( id, QPointF(lon, lat) );
			p->updateMap();

			m_adsbList.insert( id, p );

			connect(p, SIGNAL(onFeatureRemove(QString)), this, SLOT(removeAdsb(QString)));
		}

	}
}

void MapClient1::removeAdsb(QString id)
{
	MapFeature::ADSBPlaneFeature* p = m_adsbList.value(id);

	if(p) {
		delete p;
	}
	m_adsbList.remove(id);
}

void MapClient1::onFeatureClicked(QString id, QString type)
{
	foreach (MapFeature::ADSBPlaneFeature* feature, m_adsbList) {
		QString tt = feature->getMapId();
		if(feature->getMapId() == id) {
			feature->onClicked();
			break;
		}
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

//	foreach( MapFeature::EnemyBpla* bpla, m_enemyBplaList ) {
//			bpla->updateMap();
//	}
}
