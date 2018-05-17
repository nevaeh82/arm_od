#include "SatelliteSpotWindow.h"
#include "ui_SatelliteSpotWindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <PwGis/PwGisPointList.h>
#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>
#include "SatelliteSpot.h"


SatelliteSpotWindow::SatelliteSpotWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::SatelliteSpotWindow )
{
	ui->setupUi( this );
	ui->btnStartTimer->setEnabled( false );
	ui->btnStopTimer->setEnabled( false );
	_spotName = "moveSatelliteSpot";
	_satelliteLatitude = 178.0;
	_satelliteLongitude = 0.0;
	_satelliteAltitude = 35786.0;

	connect( ui->btnOpenMap, SIGNAL( clicked() ), ui->satelliteMapWidget, SLOT( openDialogMap() ) );
	connect( ui->satelliteMapWidget, SIGNAL( mapReady() ), SLOT( onMapReady() ) );
	connect( ui->btnStartTimer, SIGNAL( clicked() ), this, SLOT( onBtnStartTimer() ) );
	connect( ui->btnStopTimer, SIGNAL( clicked() ), this, SLOT( onBtnStopTimer() ) );
	connect( ui->btnClearAll, SIGNAL( clicked() ), this, SLOT( onBtnClearAll() ) );
	connect( ui->btnLon0Lat0, SIGNAL( clicked() ), this, SLOT( onBtnLon0Lat0() ) );
	connect( ui->btnLon0Lat35, SIGNAL( clicked() ), this, SLOT( onBtnLon0Lat35() ) );
	connect( ui->btnLon90Lat0, SIGNAL( clicked() ), this, SLOT( onBtnLon90Lat0() ) );
	connect( ui->btnLon0Lat40, SIGNAL( clicked() ), this, SLOT( onBtnLon0Lat40() ) );
	connect( ui->btnLon0LatM40, SIGNAL( clicked() ), this, SLOT( onBtnLon0LatM40() ) );
	connect( ui->btnLon170Lat0, SIGNAL( clicked() ), this, SLOT( onBtnLon170Lat0() ) );
	connect( ui->btnLon180Lat0, SIGNAL( clicked() ), this, SLOT( onBtnLon180Lat0() ) );
	connect( ui->btnLonM150Lat0, SIGNAL( clicked() ), this, SLOT( onBtnLonM150Lat0() ) );
	connect( ui->btnLon180Lat6, SIGNAL( clicked() ), this, SLOT( onBtnLon180Lat6() ) );
	connect( ui->btnLonM180MLat6, SIGNAL( clicked() ), this, SLOT( onBtnLonM180MLat6() ) );
	connect( ui->btnLon180Lat60, SIGNAL( clicked() ), this, SLOT( onBtnLon180Lat60() ) );
	connect( ui->btnLon170Lat60, SIGNAL( clicked() ), this, SLOT( onBtnLon170Lat60() ) );
	connect( ui->btnLonM180LatM60, SIGNAL( clicked() ), this, SLOT( onBtnLonM180LatM60() ) );
	connect( ui->btnLonM170LatM60, SIGNAL( clicked() ), this, SLOT( onBtnLonM170LatM60() ) );
	connect( ui->btnLon170Lat45, SIGNAL( clicked() ), this, SLOT( onBtnLon170Lat45() ) );

	_satelliteTimer = new QTimer( this );
	_satelliteTimer->setInterval( 100 );
	connect( _satelliteTimer, SIGNAL( timeout() ), SLOT( onUpdateSatellitePosition() ) );

	//move satellite
	connect( ui->checkBoxMoveSatellite, SIGNAL( clicked( bool ) ), this, SLOT( onCheckBoxMoveSatellite( bool ) ) );
	//lon
	connect( ui->horSliderSatLon, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteLongitude( int ) ) );
	connect( ui->spinBoxSatLon, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteLongitude( int ) ) );
	//lat
	connect( ui->horSliderSatLat, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteLatitude( int ) ) );
	connect( ui->spinBoxSatLat, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteLatitude( int ) ) );
	//alt
	connect( ui->horSliderSatAlt, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteAltitude( int ) ) );
	connect( ui->spinBoxSatAlt, SIGNAL( valueChanged( int ) ),
			 this, SLOT( onSetSatelliteAltitude( int ) ) );
	//satellite node-spot
	connect( ui->btnSpotPoints, SIGNAL( clicked() ), this, SLOT( onBtnSpotPoints() ) );
	connect( ui->btnSpotPolygon, SIGNAL( clicked() ), this, SLOT( onBtnSpotPolygon() ) );

	this->showMaximized();
}

SatelliteSpotWindow::~SatelliteSpotWindow()
{
	delete _satelliteTimer;
	delete ui;
}

void SatelliteSpotWindow::onMapReady()
{
	QString name;
	QList<SatellitePoint> points;
	//QString fileName = QDir::currentPath() + "/resources/satellite_trace.csv";
	QString fileName = ":/resources/satellite_trace.csv";
	if ( loadSatelliteData( fileName, name, points ) ) {
		ui->txtLog->appendPlainText( "satellite_trace.csv is loaded" );
		ui->txtLog->appendPlainText( QString( "satellite count = %1" ).arg( points.length() ) );

		Satellite s;
		s.name = name;
		s.points = points;
		s.state = 0;
		_satelliteTrack << s;
		ui->btnStartTimer->setEnabled( true );
		ui->btnStopTimer->setEnabled( true );
	}
	else {
		ui->txtLog->appendPlainText( "satellite_trace.csv is not loaded" );
	}

	ui->btnStartTimer->setEnabled( true );
	ui->btnStopTimer->setEnabled( true );
	ui->btnLon0Lat0->setEnabled( true );
	ui->btnLon0Lat35->setEnabled( true );
	ui->btnLon90Lat0->setEnabled( true );
	ui->btnLon0Lat40->setEnabled( true );
	ui->btnLon0LatM40->setEnabled( true );
	ui->btnLon170Lat0->setEnabled( true );
	ui->btnLon180Lat0->setEnabled( true );
	ui->btnLonM150Lat0->setEnabled( true );
	ui->btnLon180Lat6->setEnabled( true );
	ui->btnLonM180MLat6->setEnabled( true );
	ui->btnLon180Lat60->setEnabled( true );
	ui->btnLon170Lat60->setEnabled( true );
	ui->btnLonM180LatM60->setEnabled( true );
	ui->btnLonM170LatM60->setEnabled( true );
	ui->btnLon170Lat45->setEnabled( true );
	ui->btnClearAll->setEnabled( true );
	ui->checkBoxMoveSatellite->setEnabled( true );

	//--------------
	//ui->satelliteMapWidget->addSatellite( "sat1", 0, QPointF(35, 65), empty, empty );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 0.0, 0.0 ), 3000 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 180, 0 ), 2000 );
	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 170, 0 ), 2000 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 180, 50 ), 2000 );
	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 170, 50 ), 2000 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 180, 59.5611 ), 600 );
	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 170, 59.5611 ), 600 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 180, 59.5611 ), 2000 );
	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 170, 59.5611 ), 2000 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 0.0, 40.0 ), 3000.0 );

	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 180, 60 ), 3000 );
	//error
	//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined, QPointF( 170, 60 ), 3000 );

}

void SatelliteSpotWindow::drawSpot( double satelliteLongitude, double satelliteLatitude, double satelliteAltitude )
{
	Geo3DPoint satellitePosition( satelliteLongitude, satelliteLatitude, satelliteAltitude );
	SatelliteSpot earthSpot( satellitePosition );

	SpotsList earthSpots = earthSpot.getEarthSpots( 5.0 );
	QString strLen = QString( "earthSpots.length=%1").arg( earthSpots.length() );
	ui->txtLog->appendPlainText( strLen );
	QString strLon, strLat, strLonLat;
	for( int j=0; j < earthSpots.length(); ++j ) {
		Geo2DList earthList = earthSpots.at( j );
		strLen = QString( "length=%1").arg( earthList.length() );
		ui->txtLog->appendPlainText( strLen );

		for( int i=0; i < earthList.length(); ++i ) {
			strLon = QString::number( earthList.at( i ).Longitude, 'f', 6 );
			strLat = QString::number( earthList.at( i ).Latitude, 'f', 6 );
			strLonLat = QString( "i=%1, lon=%2, lat=%3").arg( i ).arg( strLon ).arg( strLat );
			ui->txtLog->appendPlainText( strLonLat );
		}

		GeometricFunctions geometricFunc;
		GeoBorder spotBorder = geometricFunc.getGeoBorder( earthList );

		QString minMaxCoord = QString( "minLon=%1 maxLon=%2 minLat=%3 maxLat=%4" )
			.arg( spotBorder.MinLongitude )
			.arg( spotBorder.MaxLongitude )
			.arg( spotBorder.MinLatitude )
			.arg( spotBorder.MaxLatitude );
		ui->txtLog->appendPlainText( minMaxCoord );

		//ui->satelliteMapWidget->addPolylinePath( earthList );
		//---
		ui->satelliteMapWidget->addPointsPath( earthList );
		ui->satelliteMapWidget->addPolygonPath( earthList );
		//ui->satelliteMapWidget->showCoverageMapCalc( CoverageMapType::SatelliteDefined,
		//	QPointF( satelliteLongitude, satelliteLatitude ), satelliteAltitude );
	}

	QPolygonF empty;
	ui->satelliteMapWidget->addSatellite( "", 0,
		QPointF( satelliteLongitude, satelliteLatitude ), empty, empty );

	ui->txtLog->appendPlainText( "finish" );
}

void SatelliteSpotWindow::onBtnLon0Lat0()
{
	drawSpot( 0.0, 0.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon0Lat35()
{
	drawSpot( 0.0, 35.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon90Lat0()
{
	drawSpot( 90.0, 0.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon0Lat40()
{
	drawSpot( 0.0, 40.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon0LatM40()
{
	drawSpot( 0.0, -40.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon170Lat0()
{
	drawSpot( 170.0, 0.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon180Lat0()
{
	drawSpot( 180.0, 0.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLonM150Lat0()
{
	drawSpot( -150.0, 0.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon180Lat6()
{
	drawSpot( 180.0, 6.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLonM180MLat6()
{
	drawSpot( -180.0, -6.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon180Lat60()
{
	drawSpot( 180.0, 60.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon170Lat60()
{
	drawSpot( 170.0, 60.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLonM180LatM60()
{
	drawSpot( -180.0, -60.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLonM170LatM60()
{
	drawSpot( -170.0, -60.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnLon170Lat45()
{
	drawSpot( 170.0, 45.0, 3000.0 );
}

void SatelliteSpotWindow::onBtnClearAll()
{
	ui->satelliteMapWidget->removeAllObjects();
	ui->txtLog->clear();
}

bool SatelliteSpotWindow::loadSatelliteData( const QString& file, QString& name, QList<SatellitePoint>& path )
{
	QFile f( file );
	if ( !f.open( QIODevice::ReadOnly ) ) {
		return false;
	}

	QString text = f.readAll();
	f.close();

	QStringList rows = text.split('\n');
	if ( rows.length() == 0 ) {
		return false;
	}

	path.clear();
	name = rows.takeFirst().trimmed();

	rows.removeFirst();

	double minLon = 1000000;
	double minLat = 1000000;
	double maxLon = -1000000;
	double maxLat = -1000000;
	foreach ( QString row, rows ) {
		QStringList data = row.split(';');
		if (data.length() < 6) continue;

		SatellitePoint point;
		point.time = data[0].toInt();
		point.bearing = data[1].replace( ",", "." ).toDouble();
		point.placeAngle = data[2].replace( ",", "." ).toDouble();
		point.height = data[3].replace( ",", "." ).toDouble();
		point.position.setX( data[5].replace( ",", "." ).toDouble() );
		point.position.setY( data[4].replace( ",", "." ).toDouble() );

		//QString logSatPoint = QString( "time=%1 bearing=%2 placeAngle=%3 height(km)=%4 x-lon=%5 y-lat=%6" )
		//	.arg( point.time )
		//	.arg( point.bearing )
		//	.arg( point.placeAngle )
		//	.arg( point.height )
		//	.arg( point.position.x() )
		//	.arg( point.position.y() );
		//ui->txtLog->appendPlainText( logSatPoint );

		if ( point.position.x() < minLon ) {
			minLon = point.position.x();
		}
		if ( point.position.y() < minLat ) {
			minLat = point.position.y();
		}
		if ( point.position.x() > maxLon ) {
			maxLon = point.position.x();
		}
		if ( point.position.y() > maxLat ) {
			maxLat = point.position.y();
		}

		path << point;
	}

	QString minMaxCoord = QString( "minLon=%1 maxLon=%2 minLat=%3 maxLat=%4" )
		.arg( minLon )
		.arg( maxLon )
		.arg( minLat )
		.arg( maxLat );
	ui->txtLog->appendPlainText( minMaxCoord );
	ui->txtLog->appendPlainText( "name=`" + name + "`" );

	return true;
}

void SatelliteSpotWindow::onBtnStartTimer()
{
	_satelliteTimer->start();
}

void SatelliteSpotWindow::onBtnStopTimer()
{
	_satelliteTimer->stop();
}

void SatelliteSpotWindow::onUpdateSatellitePosition()
{
	for (int i=0; i < _satelliteTrack.length(); i++) {
		Satellite& s = _satelliteTrack[i];
		QPolygonF futureTrack, pastTrack;
		for (int i=0; i < s.points.length(); i++) {
			if (i < s.state) {
				pastTrack << s.points[i].position;
			} else {
				futureTrack << s.points[i].position;
			}
		}

		if (futureTrack.size()) {
			pastTrack << futureTrack.first();
		}

#if QT_VERSION >= 0x040700
		QDateTime time = QDateTime::fromMSecsSinceEpoch( s.points[s.state].time );
#else
		QDateTime time = QDateTime::fromTime_t( s.points[s.state].time / 1000 );
		QTime msecs = time.time();
		msecs.addMSecs( s.points[s.state].time % 1000 );
		time.setTime(msecs);
#endif
		QPointF position = s.points[s.state].position;

		if (s.id.isEmpty()) {
			s.id = ui->satelliteMapWidget->addSatellite(
						s.name, time.toString(), position, futureTrack, pastTrack );
		} else {
			ui->satelliteMapWidget->updateSatellitePosition( s.id, time.toString(), position );
			ui->satelliteMapWidget->updateSatelliteTrack( s.id, futureTrack, pastTrack);
		}

		s.state++;
		if (s.state >= s.points.length()) {
			s.state = 0;
		}
	}
}

//move satellite
void SatelliteSpotWindow::onCheckBoxMoveSatellite( bool checked )
{
	ui->btnSpotPoints->setEnabled( checked );
	ui->btnSpotPolygon->setEnabled( checked );
	ui->lblSatLon->setEnabled( checked );
	ui->spinBoxSatLon->setEnabled( checked );
	ui->horSliderSatLon->setEnabled( checked );
	ui->lblSatLat->setEnabled( checked );
	ui->spinBoxSatLat->setEnabled( checked );
	ui->horSliderSatLat->setEnabled( checked );
	ui->lblSatAlt->setEnabled( checked );
	ui->spinBoxSatAlt->setEnabled( checked );
	ui->horSliderSatAlt->setEnabled( checked );

	if ( checked ) {
		this->moveSpot();
	}
	else {
		ui->txtLog->clear();
		ui->satelliteMapWidget->removeAllObjects();
	}
}

void SatelliteSpotWindow::onSetSatelliteLongitude( int longitude )
{
	if ( ui->horSliderSatLon->value() != longitude ) {
		ui->horSliderSatLon->setValue( longitude );
	}
	if ( ui->spinBoxSatLon->value() != longitude ) {
		ui->spinBoxSatLon->setValue( longitude );
	}
	this->moveSpot();
}

void SatelliteSpotWindow::onSetSatelliteLatitude( int latitude )
{
	if ( ui->horSliderSatLat->value() != latitude ) {
		ui->horSliderSatLat->setValue( latitude );
	}
	if ( ui->spinBoxSatLat->value() != latitude ) {
		ui->spinBoxSatLat->setValue( latitude );
	}
	this->moveSpot();
}

void SatelliteSpotWindow::onSetSatelliteAltitude( int altitude )
{
	if ( ui->horSliderSatAlt->value() != altitude ) {
		ui->horSliderSatAlt->setValue( altitude );
	}
	if ( ui->spinBoxSatAlt->value() != altitude ) {
		ui->spinBoxSatAlt->setValue( altitude );
	}
	this->moveSpot();
}

void SatelliteSpotWindow::onBtnSpotPoints()
{
	ui->txtLog->clear();
	ui->satelliteMapWidget->removeAllObjects();

	_satelliteLongitude = ui->spinBoxSatLon->value();
	_satelliteLatitude = ui->spinBoxSatLat->value();
	_satelliteAltitude = ui->spinBoxSatAlt->value();
	QString strSatPosition = QString( "_satelliteLongitude=%1, _satelliteLatitude=%2, _satelliteAltitude=%3" )
		.arg( _satelliteLongitude ).arg( _satelliteLatitude ).arg( _satelliteAltitude );
	ui->txtLog->appendPlainText( strSatPosition );

	QPolygonF empty;
	ui->satelliteMapWidget->addSatellite( "", 0,
		QPointF( _satelliteLongitude, _satelliteLatitude ), empty, empty );

	Geo3DPoint satellitePosition( _satelliteLongitude, _satelliteLatitude, _satelliteAltitude );
	SatelliteSpot earthSpot( satellitePosition );
	Geo2DList earthSpotPoints = earthSpot.getEarthSpotNodes();
	ui->satelliteMapWidget->addPointsPath( earthSpotPoints );
	ui->txtLog->appendPlainText( QString( "earthSpotPoints.length=%1").arg( earthSpotPoints.length() ) );

	QString strLon, strLat, strLonLat;
	for( int i=0; i < earthSpotPoints.length(); ++i ) {
		strLon = QString::number( earthSpotPoints.at( i ).Longitude, 'f', 6 );
		strLat = QString::number( earthSpotPoints.at( i ).Latitude, 'f', 6 );
		strLonLat = QString( "i=%1, lon=%2, lat=%3").arg( i ).arg( strLon ).arg( strLat );
		ui->txtLog->appendPlainText( strLonLat );
	}
	ui->txtLog->appendPlainText( "void SatelliteSpotWindow::onBtnSpotPoints - finish" );
}

void SatelliteSpotWindow::moveSpot()
{
	if ( ( _satelliteLongitude == ui->spinBoxSatLon->value() ) &&
		 ( _satelliteLatitude == ui->spinBoxSatLat->value() ) &&
		 ( _satelliteAltitude == ui->spinBoxSatAlt->value() ) ) {
		ui->txtLog->appendPlainText( "void SatelliteSpotWindow::moveSpot - old position" );
		return;
	}

	ui->txtLog->clear();
	ui->satelliteMapWidget->removeAllObjects();

	_satelliteLongitude = ui->spinBoxSatLon->value();
	_satelliteLatitude = ui->spinBoxSatLat->value();
	_satelliteAltitude = ui->spinBoxSatAlt->value();

	QString strSatPosition = QString( "_satelliteLongitude=%1, _satelliteLatitude=%2, _satelliteAltitude=%3" )
		.arg( _satelliteLongitude ).arg( _satelliteLatitude ).arg( _satelliteAltitude );
	ui->txtLog->appendPlainText( strSatPosition );

	QPolygonF empty;
	ui->satelliteMapWidget->addSatellite( "", 0,
		QPointF( _satelliteLongitude, _satelliteLatitude ), empty, empty );

	Geo3DPoint satellitePosition( _satelliteLongitude, _satelliteLatitude, _satelliteAltitude );
	SatelliteSpot earthSpot( satellitePosition );
	SpotsList earthSpots = earthSpot.getEarthSpots( 5.0 );
	QString strLen = QString( "earthSpots.length=%1").arg( earthSpots.length() );
	ui->txtLog->appendPlainText( strLen );
	QString strLon, strLat, strLonLat;
	for( int j=0; j < earthSpots.length(); ++j ) {
		Geo2DList earthList = earthSpots.at( j );
		strLen = QString( "nodesCount=%1").arg( earthList.length() );
		ui->txtLog->appendPlainText( strLen );

		for( int i=0; i < earthList.length(); ++i ) {
			strLon = QString::number( earthList.at( i ).Longitude, 'f', 6 );
			strLat = QString::number( earthList.at( i ).Latitude, 'f', 6 );
			strLonLat = QString( "i=%1, lon=%2, lat=%3").arg( i ).arg( strLon ).arg( strLat );
			ui->txtLog->appendPlainText( strLonLat );
		}
		ui->satelliteMapWidget->addPolygonPath( earthList );
	}
	ui->txtLog->appendPlainText( "void SatelliteSpotWindow::moveSpot - finish" );
}

void SatelliteSpotWindow::onBtnSpotPolygon()
{
	ui->txtLog->clear();
	ui->satelliteMapWidget->removeAllObjects();

	_satelliteLongitude = ui->spinBoxSatLon->value();
	_satelliteLatitude = ui->spinBoxSatLat->value();
	_satelliteAltitude = ui->spinBoxSatAlt->value();
	QString strSatPosition = QString( "_satelliteLongitude=%1, _satelliteLatitude=%2, _satelliteAltitude=%3" )
		.arg( _satelliteLongitude ).arg( _satelliteLatitude ).arg( _satelliteAltitude );
	ui->txtLog->appendPlainText( strSatPosition );

	QPolygonF empty;
	ui->satelliteMapWidget->addSatellite( "", 0,
		QPointF( _satelliteLongitude, _satelliteLatitude ), empty, empty );

	Geo3DPoint satellitePosition( _satelliteLongitude, _satelliteLatitude, _satelliteAltitude );
	SatelliteSpot earthSpot( satellitePosition );
	SpotsList earthSpots = earthSpot.getEarthSpots( 5.0 );
	QString strLen = QString( "earthSpots.length=%1").arg( earthSpots.length() );
	ui->txtLog->appendPlainText( strLen );
	QString strLon, strLat, strLonLat;
	for( int j=0; j < earthSpots.length(); ++j ) {
		strLen = QString( "spotIndex=%1, spotsCount=%2").arg( j ).arg( earthSpots.length() );
		ui->txtLog->appendPlainText( strLen );

		Geo2DList earthList = earthSpots.at( j );
		for( int i=0; i < earthList.length(); ++i ) {
			strLon = QString::number( earthList.at( i ).Longitude, 'f', 6 );
			strLat = QString::number( earthList.at( i ).Latitude, 'f', 6 );
			strLonLat = QString( "i=%1, lon=%2, lat=%3").arg( i ).arg( strLon ).arg( strLat );
			ui->txtLog->appendPlainText( strLonLat );
		}
		ui->satelliteMapWidget->addPolygonPath( earthList );
		ui->satelliteMapWidget->addPointsPath( earthList );
	}
	ui->txtLog->appendPlainText( "void SatelliteSpotWindow::onBtnSpotPolygon - finish" );
}
