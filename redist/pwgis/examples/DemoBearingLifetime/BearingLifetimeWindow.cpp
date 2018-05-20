#include "BearingLifetimeWindow.h"
#include "ui_BearingLifetimeWindow.h"

#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <math.h>

#include <PwGis/IdGenerator.h>
#include <PwGis/MapProvider.h>
#include <PwGisLonLat.h>
#include <PwGis/TacticMap/TacticMap.h>
#include <PwGis/TacticMap/Bearing.h>
#include <PwGis/TacticMap/styles/BearingStyle.h>
#include <PwGis/TacticMap/styles/BearingBehaviourStyle.h>
#include <PwGis/TacticMap/styles/ITacticStyleFactory.h>
#include <PwGis/MapManagerEvents.h>
#include <QFileDialog>
#include <PwGisLonLat.h>
#include <PwGis/IMapBounds.h>

BearingLifetimeWindow::BearingLifetimeWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::BearingLifetimeWindow ),
	_bearing( NULL ),
	_isEnabledTimer( false ),
	_isEnabledLimitTimer( false ),
	_tacticMap(NULL)
{
	ui->setupUi( this );
	_angle = 0.0;
	_angleDeg = 0.0;

	_idGenerator = new IdGenerator( this );
	_mapProvider = ui->pwgiswidget->mapProvider();

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnEnableTimer, SIGNAL( clicked() ), this, SLOT( onBtnEnableTimer() ) );
	connect( ui->btnEnableLimitTimer, SIGNAL( clicked() ), this, SLOT( onBtnEnableLimitTimer() ) );
	connect( ui->btnDeleteBearing, SIGNAL( clicked()), this, SLOT( onBtnDeleteBearing()) );
	connect( ui->btnDeleteLimitBearing, SIGNAL( clicked()), this, SLOT( onBtnDeleteLimitBearing()) );

	_timer = new QTimer( this );
	_timer->setInterval( 1000 );
	connect( _timer, SIGNAL( timeout() ), SLOT( onUpdateBearing() ) );

	_limitTimer = new QTimer( this );
	_limitTimer->setInterval( 1000 );
	connect( _limitTimer, SIGNAL( timeout() ), SLOT( onUpdateLimitBearing() ) );

	this->showMaximized();
	ui->txtLog->append( "constructor +" );
}

BearingLifetimeWindow::~BearingLifetimeWindow()
{
	if ( !_isEnabledTimer ) {
		_timer->stop();
	}
	if ( !_isEnabledLimitTimer ) {
		_limitTimer->stop();
	}
	if(_tacticMap) delete _tacticMap;
	_mapProvider->mapManager()->closeMap();

	delete ui;
}

void BearingLifetimeWindow::onBtnOpenMap()
{
	QString mapPath = QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "Map files(*.*)" ) );
	ui->txtLog->append( "onBtnOpenMap 1+" );

	_tacticMap = new TacticMap( _mapProvider, _mapProvider->styleFactory(), _idGenerator, this );
	ui->txtLog->append( "onBtnOpenMap 2+" );

	connect( &ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL( mapReady() ), SLOT( onMapReady() ) );
	ui->txtLog->append( "onBtnOpenMap 3+" );

	_mapProvider->mapManager()->openMap( mapPath, 30 );
	ui->txtLog->append( "onBtnOpenMap 4+" );
}

void BearingLifetimeWindow::onBtnEnableTimer()
{
	if ( !_isEnabledTimer ) {
		_isEnabledTimer = true;
		ui->btnEnableTimer->setText("stop bearing");
		_timer->start();
	}
	else {
		_isEnabledTimer = false;
		ui->btnEnableTimer->setText("start bearing");
		_timer->stop();
	}
}

void BearingLifetimeWindow::onBtnEnableLimitTimer()
{
	if ( !_isEnabledLimitTimer ) {
		_isEnabledLimitTimer = true;
		ui->btnEnableLimitTimer->setText("stop limit bearing");
		_limitTimer->start();
	}
	else {
		_isEnabledLimitTimer = false;
		ui->btnEnableLimitTimer->setText("start limit bearing");
		_limitTimer->stop();
	}
}


void BearingLifetimeWindow::onMapReady()
{
	ui->txtLog->append( "onMapReady 1+" );

	if ( _bearing ) {
		//if duplicated call.
		return;
	}

	ui->txtLog->append( "onMapReady 2+" );

	//create objects on map ready step.
	BearingStyle* style = _tacticMap->styleFactory()->createBearingStyle();
	style->behaviour()->setDirectionsLimit( 5 );

	_point = _mapProvider->mapBounds()->getMapCenter();
	_point->setParent(this);

	_bearing = _tacticMap->objectsFactory()->createBearing();
	_bearing->setStyle( style );

	_bearing->addDirection(
				_point,
				_angle,
				QDateTime::currentDateTime() );

	onUpdateBearing();
	_bearing->updateMap();
}

void BearingLifetimeWindow::onUpdateBearing()
{
	_angle += 0.174532925;
	if ( _angle > 6.1 ) {
		_angle = 0.0;
	}

	double R = 30.0;
	double lon = _point->getLon() + R * sin( _angle );
	double lat = _point->getLat() + R * cos( _angle );

	ui->pwgiswidget->addLine( "idBearing", _point->getLon(), _point->getLat(), lon, lat, "", "", "" );
	ui->pwgiswidget->addPoint( "idCenter", _point->getLon(), _point->getLat(), "", "", "" );
	ui->pwgiswidget->addPoint( "idArrow", lon, lat, "", "", "slimRedLine" );

	QString message = QString( "angle=%1 lon=%2 lat=%3" ).arg( _angle ).arg( lon ).arg( lat );
	ui->txtLog->append( message );
}

void BearingLifetimeWindow::onUpdateLimitBearing()
{
	_angleDeg += 10.0;
	if ( _angleDeg >= 360.0) {
		_angleDeg = 0.0;
	}

	_bearing->addDirection(
				_point,
				_angleDeg,
				QDateTime::currentDateTime() );

	_tacticMap->updateObject(_bearing);
}

void BearingLifetimeWindow::onBtnDeleteBearing()
{
	ui->pwgiswidget->removeObject( "idBearing" );
	ui->pwgiswidget->removeObject( "idCenter" );
	ui->pwgiswidget->removeObject( "idArrow" );
}

void BearingLifetimeWindow::onBtnDeleteLimitBearing()
{
	_tacticMap->removeObject(_bearing);
	_bearing->updateMap();
}
