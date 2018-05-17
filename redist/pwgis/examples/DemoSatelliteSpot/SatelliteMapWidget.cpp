#include "SatelliteMapWidget.h"
#include "ui_SatelliteMapWidget.h"

#include <QDir>
#include <QDebug>
#include <QScopedPointer>

#include <PwGis/pwgiswidget.h>
#include <PwGis/IMapProvider.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/IStyleFactory.h>
#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/Marker.h>
#include <PwGis/PwGisPointList.h>

#include <PwGis/IMapManager.h>
#include <PwGis/ILayerManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/MapBoundsEvents.h>
#include <PwGis/IMapBounds.h>
#include <SatelliteSpot.h>

#include "SatelliteMapWidget.h"
#include <QFileDialog>

#define PROFILE_NAME   "coverageProfile"


struct LonLatListDeleter
{
	static inline void cleanup( PwGisPointList* pointer )
	{
		pointer->clear();
		delete pointer;
	}
};

typedef QScopedPointer< PwGisPointList, LonLatListDeleter > LonLatListPtr ;

SatelliteMapWidget::SatelliteMapWidget( QWidget *parent ):
	QWidget( parent ),
	ui( new Ui::SatelliteMapWidget ),
	_nextId( 0 ),
	_selectionModeEnabled( false )
{
	ui->setupUi( this );

	_styleMap[ CoverageMapType::GeoStationary ] = "CoverageGeoStationary";
	_styleMap[ CoverageMapType::LowOrbital ] = "CoverageLowOrbital";
	_styleMap[ CoverageMapType::MediumOrbital ] = "CoverageMediumOrbital";
	_styleMap[ CoverageMapType::HighOrbital ] = "CoverageHighOrbital";
	_styleMap[ CoverageMapType::SatelliteDefined ] = "CoverageSatelliteDefined";

	connect( &ui->gisWidget->mapProvider()->mapManager()->events(),
		SIGNAL( mapReady() ), this, SLOT( onMapReady() ) );

	ui->gisWidget->mapProvider()->profileManager()->registerProfile( PROFILE_NAME, ":/resources/profile" );
	ui->gisWidget->mapProvider()->mapManager()->setProfile( PROFILE_NAME );

	QString mapFile = QApplication::applicationDirPath() +
		QString( "/../../../MapSamples/GoogleCache/world/info.gc" ).replace( "/", QDir::separator() );
	if ( !QFile::exists( mapFile ) ) {
		mapFile = QApplication::applicationDirPath() +
			QString( "/../../../MapSamples/Ingit/world/World.chart" ).replace( "/", QDir::separator() );
	}
	if ( QFile::exists( mapFile ) ) {
		ui->gisWidget->mapProvider()->mapManager()->openMap( mapFile );
	}

	//ui->gisWidget->enableDebugger( true );
}

SatelliteMapWidget::~SatelliteMapWidget()
{
	ui->gisWidget->mapProvider()->mapManager()->closeMap();
	delete ui;
}

void SatelliteMapWidget::openDialogMap()
{
	ui->gisWidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ),
		QApplication::applicationDirPath(),
		tr( "Map files(*.*)" ) ), 30 );
}

void SatelliteMapWidget::onMapReady()
{
	ui->gisWidget->showTool( PwGisWidget::ToolChangeGeometry, false );

	_satelliteStyle = ui->gisWidget->mapProvider()->styleFactory()->createObjectStyle();
	//_satelliteStyle->icon()->setIconPath( QDir::currentPath() + "/resources/sattelite.png" );
	_satelliteStyle->icon()->setIconPath( "http://127.0.0.1:8080/profiles/coverageProfile/satellite.png" );
	_satelliteStyle->icon()->setIconSize( QSize( 24, 24 ) );

	_greenpointStyle = ui->gisWidget->mapProvider()->styleFactory()->createObjectStyle();
	_greenpointStyle->icon()->setIconPath( "http://127.0.0.1:8080/profiles/coverageProfile/greenpoint.png" );
	_greenpointStyle->icon()->setIconSize( QSize( 8, 8) );

	ui->gisWidget->mapProvider()->styleFactory()->createNamedStyle( "satellite_past" )
			->setProperty( PwGisStyle::strokeColor, "#404040" )
			->setProperty( PwGisStyle::strokeWidth, "1" )
			->setProperty( PwGisStyle::strokeOpacity, "0.5" )
			->setProperty( PwGisStyle::strokeDashstyle, "dot" )
			->apply();

	ui->gisWidget->mapProvider()->styleFactory()->createNamedStyle( "satellite_future" )
			->setProperty( PwGisStyle::strokeColor, "#404040" )
			->setProperty( PwGisStyle::strokeWidth, "1" )
			->setProperty( PwGisStyle::strokeOpacity, "0.7" )
			->apply();

	ui->gisWidget->executeScript( "initPolygonRingFunction()" );
	ui->gisWidget->executeScript( "initLayerSwitcher()" );

	Pw::Gis::ILayerManager* layerManager = ui->gisWidget->mapProvider()->layerManager();
	layerManager->addVectorLayer( _styleMap[ CoverageMapType::SatelliteDefined ], QString::fromUtf8( "satellite defined" ) );
	layerManager->addVectorLayer( _styleMap[ CoverageMapType::GeoStationary ], QString::fromUtf8( "geo stationary" ) );
	layerManager->addVectorLayer( _styleMap[ CoverageMapType::HighOrbital ], QString::fromUtf8( "high orbital" ) );
	layerManager->addVectorLayer( _styleMap[ CoverageMapType::MediumOrbital ], QString::fromUtf8( "medium orbital" ) );
	layerManager->addVectorLayer( _styleMap[ CoverageMapType::LowOrbital ], QString::fromUtf8( "low orbital" ) );

	emit mapReady();
}

PwGisPointList* SatelliteMapWidget::getPointList( QPolygonF geometry )
{
	PwGisPointList* list = new PwGisPointList(this);
	foreach( QPointF point, geometry) {
		list->addLonLat( new PwGisLonLat( point.x(), point.y(), this ) );
	}

	return list;
}

void SatelliteMapWidget::drawPolygonOver180( QPolygonF poly360, QPolygonF empty, QString id, CoverageMapType::Enum satelliteType )
{
	QPolygonF poly1;
	QPolygonF poly2;
	QPointF prev;

	foreach( QPointF p, poly360 ) {
		if ( p.x() > 180 ) {
			if ( prev.x() <= 180 ) {
				poly1.append( prev );
			}
			poly1.append( p );
		}
		if ( p.x() < 180 ) {
			if ( prev.x() > 180 ) {
				poly2.append( prev );
			}
			poly2.append( p );
		}
		prev = p;
	}

	//ui->gisWidget->addPath(id, getPointList(poly), "", "", "");
	addPolygon( id+"1", &poly1, &empty, "", "", _styleMap[ satelliteType ] );
	addPolygon( id+"2", &poly2, &empty, "", "", _styleMap[ satelliteType ] );
}

QPolygonF SatelliteMapWidget::crop82( QPolygonF poly )
{
	QPolygonF resampled;
	foreach( QPointF p, poly ) {
		if ( p.y() > 82 ) {
			continue;
		}
		resampled.append( p );
	}

	return resampled;
}

void SatelliteMapWidget::normalize360( QPolygonF poly, QPolygonF& poly360, QPointF &min, QPointF &max )
{
	foreach( QPointF p, poly ) {
		if ( p.x() < 0 ) {
			p.setX( p.x() + 360 );
		}

		min.setX( qMin( min.x(), p.x() ) );
		min.setY( qMin( min.y(), p.y() ) );

		max.setX( qMax( max.x(), p.x() ) );
		max.setY( qMax( max.y(), p.y() ) );

		poly360.append( p );
	}
}

void SatelliteMapWidget::addPointsPath( Geo2DList geoPoints )
{
	for( int i=0; i < geoPoints.length(); ++i ) {
		Marker* marker = ui->gisWidget->mapProvider()->objectsFactory()->createMarker();

		marker->setPosition( new PwGisLonLat( geoPoints.at( i ).Longitude, geoPoints.at( i ).Latitude ) );
		marker->setStyle( _greenpointStyle );
		marker->updateMap();

		//if ( i > 20 ) {
		//	break;
		//}
	}
}

void SatelliteMapWidget::addPolylinePath( Geo2DList geoPoints )
{
	QString id = "polyPath" + generateId();

	PwGisPointList pathPoints;
	for( int i=0; i < geoPoints.length(); ++i ) {
		pathPoints.append( new PwGisLonLat( geoPoints.at( i ).Longitude, geoPoints.at( i ).Latitude ) );
	}

	ui->gisWidget->addPath( id, &pathPoints, "", "", "" );
}

void SatelliteMapWidget::addPolygonPath( Geo2DList geoPoints )
{
	QString id = "polygonPath" + generateId();

	PwGisPointList pathPoints;
	for( int i=0; i < geoPoints.length(); ++i ) {
		pathPoints.append( new PwGisLonLat( geoPoints.at( i ).Longitude, geoPoints.at( i ).Latitude ) );
	}

	ui->gisWidget->addPolygon( id, &pathPoints, "", "", _styleMap[ CoverageMapType::SatelliteDefined ] );
}

void SatelliteMapWidget::removeAllObjects()
{
	ui->gisWidget->removeAllObjects();
}

QString SatelliteMapWidget::generateId()
{
	return QString( "id%1" ).arg( _nextId++ );
}

SatelliteMapHandle SatelliteMapWidget::showCoverageMapCalc(
		CoverageMapType::Enum satelliteType, QPointF position,
		double altitude )
{
	QString id = generateId();


	//-------------------------------------------------------------------------
	//SatelliteSpot spot;
	//QPolygonF poly = spot.getSpotPoints(position.x(), position.y(), altitude);

	//QPolygonF SatelliteSpot::getSpotPoints( double satelliteLongitude,
	//	double satelliteLatitude, double satelliteAltitude )

	//-------------------
	//static GeoPosition const observerPosition(59.5611, 29.867);
	//GeoPosition const observerPosition(position.y(), position.x());
	//GeoPosition2D observerPosition( position.x(), position.y() );
	QPolygonF poly;
	//try {
		//GeoSpot const spot(Observer(observerPosition, altitude));
		Geo3DPoint spotPosition( position.x(), position.y(), altitude );
		//SatelliteSpot const spot( spotPosition );
		SatelliteSpot spot( spotPosition );

		//static double const step = 5.0;
		//const double start = 0;
		double step = 5.0;
		double start = 0;

		for (double azimuth = start ; azimuth < 360+step+start; azimuth += step) {
			Geo2DPoint position = spot.getPoint( azimuth );
			poly.append( QPointF( position.Longitude, position.Latitude ) );
		}
	//}
	//catch (std::exception const &e) {
	//	//std::cerr << "runtime exception: " << e.what() << std::endl;
	//	return ret;
	//}
	//-------------------------------------------------------------------------



	QPolygonF empty;

	QPolygonF poly360;
	QPointF min(360, 90);
	QPointF max;

	normalize360( poly, poly360, min, max );

	if ( max.y() > 84 || (min.x() < 20 && max.x() == 360 ) ) {
		poly = crop82(poly);

		qSort( poly.begin(), poly.end(), lonMin);
		if ( min.x() < 180 && max.x() > 180  ) {

			poly360.clear();
			normalize360(poly, poly360, min, max);

			/*
			int pos = 0;
			foreach (QPointF p, poly) {
				ui->gisWidget->addPoint(id + QString("%1").arg(pos++), p.x(), p.y(), "", "", "satpoint");
			}*/
			QPolygonF poly1;
			QPolygonF poly2;
			QPointF prev;

			foreach( QPointF p, poly360) {
				if ( p.x() > 180) {
					if ( prev.x() <= 180) {
						//poly1.append(prev);
					}
					poly1.append(p);
				}
				if ( p.x() < 180) {
					if ( prev.x() > 180) {
						//poly2.append(prev);
					}
					poly2.append(p);
				}
				prev = p;
			}
			if ( poly2.first().x() < 20 && poly1.last().x() > 355) {
				poly2.push_front(poly1.last());
			}
			poly1.push_front(QPointF(poly1.first().x(), 82));
			poly1.push_back(QPointF(poly1.last().x(), 82));

			poly2.push_front(QPointF(poly2.first().x(), 82));
			poly2.push_back(QPointF(poly2.last().x(), 82));


			//ui->gisWidget->addPath(id, getPointList(poly), "", "", "");
			addPolygon( id+"1", &poly1, &empty, "", "", _styleMap[ satelliteType ] );
			addPolygon( id+"2", &poly2, &empty, "", "", _styleMap[ satelliteType ] );

			return id;
		}
		else {
			poly.push_front( QPointF( poly.first().x(), 82 ) );
			poly.push_back( QPointF( poly.last().x(), 82 ) );
			addPolygon( id+"1", &poly, &empty, "", "", _styleMap[ satelliteType ] );
		}

		return id;
	}

	if ( min.y() < -82 ) {
		QPolygonF resampled;
		foreach( QPointF p, poly ) {
			if ( p.y() <  -82 ) {
				continue;
			}
			resampled.append( p );
		}
		poly = resampled;

		qSort( poly.begin(), poly.end(), lonMin );
		poly.push_front( QPointF( poly.first().x(), -82 ) );
		poly.push_back( QPointF( poly.last().x(), -82 ) );
		addPolygon( id+"1", &poly, &empty, "", "", _styleMap[ satelliteType ] );
		return id;
	}


	if ( min.x() < 180 && max.x() > 180 ) {
		//over 180
		drawPolygonOver180( poly360, empty, id, satelliteType );
		return id;
	}


	//ui->gisWidget->addPath(id, getPointList(poly), "", "", "");
	addPolygon( id+"1", &poly, &empty, "", "", _styleMap[ satelliteType ] );
	return id;
}

void SatelliteMapWidget::addPolygon( const QString& id, QPolygonF* outerGeometry, QPolygonF* innerGeometry,
	const QString& caption, const QString& tooltip, const QString& aStyle )
{

	QStringList listOuter;
	QStringList listInner;

	listOuter.append("[");
	foreach( QPointF point, *outerGeometry) {
		listOuter.append("{ lon: ");
		listOuter.append(QString::number(point.x()));
		listOuter.append(", lat: ");
		listOuter.append(QString::number(point.y()));
		listOuter.append(" }");
		listOuter.append(",");
	}
	if(outerGeometry->size() != 0) {
		listOuter.pop_back(); //remove last " },"
	}
	listOuter.append("]");

	listInner.append("[");
	foreach( QPointF point, *innerGeometry) {
		listInner.append("{ lon: ");
		listInner.append(QString::number(point.x()));
		listInner.append(", lat: ");
		listInner.append(QString::number(point.y()));
		listInner.append(" }");
		listInner.append(",");
	}
	if(innerGeometry->size() != 0) {
		listInner.pop_back(); //remove last " },"
	}
	listInner.append("]");

	ui->gisWidget->executeScript( QString("client.addPolygonRing( \"%1\", %2, %3, \"%4\", \"%5\", \"%6\");" )
				  .arg( id )
				  .arg( listOuter.join("") )
				  .arg( listInner.join("") )
				  .arg( caption )
				  .arg( tooltip )
				  .arg( aStyle) );

	//this.addPolygonRing = function( id, pointsOuter, pointInner, caption, tooltip, styles, featureType, indPart, countParts  )
}

void SatelliteMapWidget::defusePolygon(QPolygonF& polygon)
{
	for(int i=0, j=0; i < polygon.size(); i++, j++) {
		if (i && (polygon[i-1] - polygon[i]).manhattanLength() < 2) {
			polygon.remove(i);
			i--;
		}
	}
}

SatelliteMapHandle SatelliteMapWidget::addSatellite( QString name, QString time,
	QPointF position, QPolygonF futureTrack, QPolygonF pastTrack )
{
	QString id = name;
	if ( id.isEmpty() ) {
		id = "satellite" + generateId();
	}

	Marker* marker = ui->gisWidget->mapProvider()->objectsFactory()->createMarker();

	marker->setPosition( new PwGisLonLat( ( double )position.x(), ( double )position.y() ) );
	marker->setToolTip( time );
	marker->setName( name );
	marker->setStyle( _satelliteStyle );
	marker->updateMap();

	_satellites.insert( id, marker );

	defusePolygon( futureTrack );
	defusePolygon( pastTrack );

	pastTrack.append( position );

	LonLatListPtr satFut( getPointList( futureTrack ) );
	LonLatListPtr satPast( getPointList( pastTrack ) );

	ui->gisWidget->addPath( QString( "satfut" ).append(id), satFut.data(), "", "", "satellite_future" );
	ui->gisWidget->addPath( QString( "satpast" ).append(id), satPast.data(), "", "", "satellite_past" );
	return id;
}

void SatelliteMapWidget::removeSatellite(SatelliteMapHandle handle)
{
	if ( !_satellites.contains( handle ) ) {
		return;
	}

	_satellites[ handle ]->removeFromMap();
	_satellites.remove( handle );

	ui->gisWidget->removeObject( QString( "satfut" ).append( handle ) );
	ui->gisWidget->removeObject( QString( "satpast" ).append( handle ) );
}

void SatelliteMapWidget::updateSatellitePosition( SatelliteMapHandle handle, QString time, QPointF position )
{
	if ( !_satellites.contains( handle ) ) {
		return;
	}

	Marker* s = _satellites.value( handle );
	s->setToolTip( time );
	s->setPosition( new PwGisLonLat( position.x(), position.y() ) );
	s->updateMap();
}

void SatelliteMapWidget::updateSatelliteTrack( SatelliteMapHandle handle, QPolygonF futureTrack, QPolygonF pastTrack )
{
	if ( !_satellites.contains( handle ) ) {
		return;
	}

	Marker* s = _satellites.value( handle );

	ui->gisWidget->removeObject( QString( "satfut" ).append( handle ) );
	ui->gisWidget->removeObject( QString( "satpast" ).append( handle ) );

	defusePolygon( futureTrack );
	defusePolygon( pastTrack );

	pastTrack.append( QPointF( s->position()->lon, s->position()->lat ) );

	LonLatListPtr satFut( getPointList( futureTrack ) );
	LonLatListPtr satPast( getPointList( pastTrack ) );

	ui->gisWidget->addPath( QString( "satfut" ).append( handle ), satFut.data(), "", "", "satellite_future" );
	ui->gisWidget->addPath( QString( "satpast" ).append( handle ), satPast.data(), "", "", "satellite_past" );
}
