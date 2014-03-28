﻿#include "MapClient1.h"


MapClient1::MapClient1( PwGisWidget* pwwidget, TabsProperty* property,
	IDBManager* dbBla, IDBManager* dbEvil ) :
	m_mux( QMutex::Recursive ),
	m_niippLayerId( 0 ),
	m_layersCounter( 0 )
{
	m_niippLayerName = "NIIPP";

	m_pointUvodaNiipp.setX( 0 );
	m_pointUvodaNiipp.setY( 0 );

	m_dbBla = dbBla;
	m_dbEvil = dbEvil;

	m_circleRadius = 0;
	m_circleChanged = false;
	m_sliceChanged = false;

	m_pwwidget = pwwidget;
	m_mapBounds = m_pwwidget->mapProvider()->mapBounds();
	m_property = property;
	m_mainLatitude = m_property->get_latitude();
	m_mainLongitude = m_property->get_longitude();
	m_lastCoord = new QMap<int, PwGisPointList* >;


	//define style
	this->addMarkerLayer( 0, QObject::tr( "ОП" ) );//0 - stations
	this->addMarkerLayer( 1, QObject::tr( "БПЛА" ) ); //1 - BPLA profile
	this->addMarkerLayer( 2, QObject::tr( "БЛА" ) ); //2 - BLA profile
	this->addMarkerLayer( 3, QObject::tr( "Атлант" ) ); //3 - Pelengators
	this->addMarkerLayer( 4, QObject::tr( "Атлант Цель" ) ); //4 - PelengatorsPoint
	this->addMarkerLayer( 5, QObject::tr( "Сетка" ) ); //5 - Grid
	this->addMarkerLayer( 6, QObject::tr( "Контрольные точки" ) ); //6 - Control_points
	this->addMarkerLayer( 7, QObject::tr( "Точка перехвата" ) ); //7 - Perehvat
	this->addMarkerLayer( 8, QObject::tr( "Гражданские суда" ) ); //8 - AIS - civil plane
	this->addMarkerLayer( 9, QObject::tr( "Точки увода" ) ); //9 - NIIPPPoint
	this->addMarkerLayer( 10, QObject::tr( "СПИП ДД" ) ); //10 - NIIPP
	//map features
	m_AisFeature = new AisFeature( m_pwwidget, m_mapLayers.value(8) );
	m_NiippFeature = new NiippFeature( m_pwwidget, m_mapLayers.value(10),
		m_mapLayers.value(9) );
	m_PelengatorFeature = new PelengatorFeature( m_pwwidget, m_lastCoord,
		m_mapLayers.value(3), m_mapLayers.value(4) );
	m_PerehvatFeature = new PerehvatFeature( m_pwwidget, m_mapLayers.value(7) );
	m_BlaFeature = new BlaFeature( m_pwwidget, m_lastCoord, m_mapLayers.value(2) );
	m_BplaFeature = new BplaFeature( m_pwwidget, m_mapLayers.value(1) );
	m_StationFeature = new StationFeature( m_pwwidget, m_mapLayers.value(0) );

	QString niippLayerId = m_niippLayerName + QString::number( m_niippLayerId );
	addNiippLayer( niippLayerId );


	m_updateTimer = new QTimer();

	connect( this, SIGNAL( signalAddBla( int,QByteArray ) ),
		this, SLOT( setCurrentPoint( int,QByteArray ) ) );

	connect( this, SIGNAL( signalAddEvil( int,QByteArray ) ),
		this, SLOT( setCurrentPoint( int,QByteArray ) ) );

	connect( this, SIGNAL( signalAddAis( QMap<int, QVector<QString> >) ),
		this, SLOT( setAisData( QMap<int, QVector<QString> > ) ) );

	connect( m_updateTimer, SIGNAL( timeout() ),
		this, SLOT( updatePoints() ) );

	connect( this, SIGNAL( signalUpdatePeleng( int, int, double, double, double ) ),
		this, SLOT( updatePelengData( int, int, double, double, double ) ) );

	connect( this, SIGNAL( signalUpdateSector( int, double, double, QByteArray ) ),
		this, SLOT( updateSector( int, double, double, QByteArray ) ) );

	connect( this, SIGNAL( signalUpdateCicle( int, double, QByteArray ) ),
		this, SLOT( updateCicle( int, double, QByteArray ) ) );

	connect( this, SIGNAL( signalAddPerehvat( int, int ) ),
		this, SLOT( addPerehvatData( int, int ) ) );

	connect( this, SIGNAL( signalRemovePerehvat( int, int ) ),
		this, SLOT( removePerehvatData( int, int ) ) );


	m_TimerSimulator = new QTimer();
	connect( m_TimerSimulator, SIGNAL( timeout() ),
		this, SLOT( updateSimulator() ) );

	m_uiTimer = new QTimer( this );
	connect( m_uiTimer, SIGNAL( timeout() ), this, SLOT( updateCircle() ) );
	m_uiTimer->setInterval( 100 );
	m_uiTimer->start();

	m_uiTimerSlice = new QTimer( this );
	connect( m_uiTimerSlice, SIGNAL( timeout() ), this, SLOT( updateSlice() ) );
	m_uiTimerSlice->setInterval( 100 );

	QObject::connect( &m_pwwidget->mapProvider()->mapManager()->events(),
		SIGNAL( mapReady() ), this, SLOT( onMapReady() ) );

	m_layerManager = m_pwwidget->mapProvider()->layerManager();

	m_perehvat = new ZInterception( this );
	QThread* thread = new QThread;
	connect( m_perehvat, SIGNAL( finished() ),
		thread, SLOT( quit() ) );
	connect( m_perehvat, SIGNAL( finished() ),
		m_perehvat, SLOT( deleteLater() ) );
	connect( thread, SIGNAL( finished() ),
		thread, SLOT( deleteLater() ) );
	m_perehvat->moveToThread( thread );
	thread->start();
	connect( this, SIGNAL( signalAddPerehvatPoint( int, int, QPointF, float, float, int, float, float) ),
		this, SLOT( addPerehvatPointData( int, int, QPointF, float, float, int, float, float ) ) );
}

MapClient1::~MapClient1()
{
	delete m_AisFeature;
	delete m_NiippFeature;
	delete m_PelengatorFeature;
	delete m_PerehvatFeature;
	delete m_BlaFeature;
	delete m_BplaFeature;
	delete m_StationFeature;
}

void MapClient1::setNiippController( INiiPPController* niippController )
{
	m_mapNiippController.insert( niippController->get_id(),
		niippController );
}

void MapClient1::showLayer( int index, bool state )
{
	if ( m_mapLayers.contains( index ) ) {
		m_layerManager->setLayerVisibility( m_mapLayers.value( index ), state );
	}
}

/// set point from another thread
void MapClient1::addBLA( int id, QByteArray data )
{
	QMutexLocker lock( &m_mux );

	emit signalAddBla( id, data );
}

/// set point from another thread
void MapClient1::addEvil( int id, QByteArray data )
{
	QMutexLocker lock( &m_mux );

	emit signalAddEvil( id, data );
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

	emit signalAddAis( vec );
}

void MapClient1::updateNiippPowerSector( int id, double radius, double bis, QByteArray ba )
{
	QMutexLocker lock( &m_mux );

	emit signalUpdateSector( id, radius, bis, ba );
}

void MapClient1::updateNiippPowerCicle( int id, double radius, QByteArray ba )
{
	QMutexLocker lock( &m_mux );

	emit signalUpdateCicle( id, radius, ba );
}

void MapClient1::updatePeleng( int id, int idPost, double lat, double lon, double direction )
{
	QMutexLocker lock( &m_mux );

	emit signalUpdatePeleng( id, idPost, lat, lon, direction );
}

void MapClient1::addPerehvat(int blaId, int bplaId )
{
	emit signalAddPerehvat( blaId, bplaId );
}

void MapClient1::removePerehvat(int blaId, int bplaId )
{
	emit signalRemovePerehvat( blaId, bplaId );
}

void MapClient1::addPerehvatPoint(int blaId, int bplaId, QPointF coord,
	float hgt, float radius, int time, float intcCourse, float intcSpeed)
{
	emit signalAddPerehvatPoint( blaId, bplaId, coord, hgt, radius,
		time, intcCourse, intcSpeed );
}

void MapClient1::removePointUvoda()
{
	m_pwwidget->removeMarker( "NIIPMarker" );
}

/// set central control point
void MapClient1::setPoint()
{
	//define style
	//5 - Grid
	PwGisStyle* gridStyle = m_pwwidget->createStyle( "Grid" );
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
	PwGisStyle* pointsStyle = m_pwwidget->createStyle( "Control_points" );
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
	m_StationFeature->readFromFile( mapObjectsSettingFile );
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
	int h = m_pwwidget->maximumHeight();
	int w = m_pwwidget->maximumWidth();
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
	m_pwwidget->mapProvider()->layerManager()->addVectorLayer( id, m_niippLayerName );
}

void MapClient1::addPerehvatData( int bla_id, int bpla_id )
{
	m_mapBattle.insert( bla_id, bpla_id );
}

void MapClient1::updateSimulator()
{
	static const double lats[23] = {60.075751,60.075123,60.074381,60.074096,60.073696,60.074153,60.075466,60.076722,60.078035,60.079120,60.080662,60.082089,60.082946,60.083574,60.084145,60.084659,60.085058,60.085515,60.085972,60.083345,60.079805,60.077236,60.076151};
	static const double lons[23] = {30.623432,30.626887,30.629766,30.632990,30.635984,30.638978,30.640369,30.641627,30.642663,30.643584,30.644505,30.642563,30.640475,30.638057,30.635408,30.631493,30.628614,30.625045,30.622051,30.614911,30.615372,30.616408,30.620208};

	int id6706 = 6706;
	static int i6706;
	if ( i6706 > 22 ) {
		i6706 = 0;
	}
	QPointF point( lats[i6706], lons[i6706] );
	setPointBla( id6706, point, 0, 0, 0, 0 );
	i6706++;
}

//for timer
void MapClient1::updateCircle()
{
	if ( !m_circleChanged ) {
		return;
	}
	m_circleChanged = false;

	m_pwwidget->addClassicPolygon( "c1",  30.531368, 60.074592,
		m_circleRadius*1000, 40, 0, "ОП1 Гроза", "", "yellow selectAndDrag" );
}

void MapClient1::updateSlice()
{
	if ( !m_sliceChanged ) {
		return;
	}
}

void MapClient1::onMapReady()
{
	foreach( IMapObjectInfo* value, m_mapObjects ) {
		dynamic_cast<Sector*>(value)->updateMap();
	}
	m_updateTimer->start( 1000 );

	connect( m_pwwidget, SIGNAL( mapClicked( double, double ) ),
		this, SLOT( mapMouseClicked( double, double ) ) );
}

/// get coordinates
void MapClient1::mapMouseClicked( double lon, double lat )
{
	m_NiippFeature->addPoint( lon, lat );

	m_pointUvodaNiipp.setX( lat );
	m_pointUvodaNiipp.setY( lon );
	m_mapNiippController.value( 100 )->set_point( m_pointUvodaNiipp );
	m_mapNiippController.value( 101 )->set_point( m_pointUvodaNiipp );
}

void MapClient1::removePerehvatData( int bla_id, int bpla_id )
{
	m_mapBattle.remove( bla_id );
}


// --- draw features on the map ---

//add BLA
/// set point in this thread
void MapClient1::setPointBla( int id, QPointF point, double alt, double speed,
	double course, int state )
{
	QMutexLocker lock( &m_mux );

	m_BlaFeature->add( id, point );
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

	if ( !track.size() ) {
		return;
	}

	m_BplaFeature->add( id, sko, &track, alt );

	m_mapNiippController.value( 100 )->send_evil( track.at( track.size()-1 ),
		m_pointUvodaNiipp, alt, bearing );
	m_mapNiippController.value( 101 )->send_evil( track.at( track.size()-1 ),
		m_pointUvodaNiipp, alt, bearing );
}

//add AIS
void MapClient1::setAisData( QMap<int, QVector<QString> > map1 )
{
	m_AisFeature->add( map1 );
}

void MapClient1::removeAis()
{
	m_AisFeature->remove();
}

//add Pelengators
void MapClient1::updatePelengData( int id, int idPost, double lat, double lon,
	double direction )
{
	m_PelengatorFeature->updatePeleng( idPost, lat, lon, direction );
}

//add PelengatorsPoint
void MapClient1::setPointEvilPeleng( int id, QPointF point )
{
	m_PelengatorFeature->setPointEvilPeleng( id, point );
}

//add NIIPP-slice
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void MapClient1::updateSector( int id, double radius, double bis, QByteArray ba )
{
	m_NiippFeature->updateSector( id, radius, bis, ba );
}

//add NIIPP-circle
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void MapClient1::updateCicle( int id, double radius, QByteArray ba )
{
	m_NiippFeature->updateCicle( id, radius, ba );
}

//add Perehvat
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void MapClient1::addPerehvatPointData( int blaId, int bplaId, QPointF coord,
	float hgt, float radius, int time, float intcCourse, float intcSpeed )
{
	m_PerehvatFeature->addPerehvatPointData( blaId, bplaId, coord,
		hgt, radius, intcCourse, intcSpeed );
}
