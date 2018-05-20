#include <QTimer>
#include <QDateTime>
#include <QDebug>

#include "PostTrackingWindow.h"
#include "ui_PostTrackingWindow.h"
#include <PwGis/IdGenerator.h>
#include <PwGis/MapProvider.h>
#include <PwGisLonLat.h>
#include <PwGis/TacticMap/TacticMap.h>
#include <PwGis/TacticMap/Post.h>
#include <PwGis/TacticMap/styles/TrackingStyle.h>
#include <PwGis/TacticMap/styles/ITacticStyleFactory.h>
#include <QFileDialog>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapBounds.h>
#include <PwGis/objects/IconStyle.h>
#include <math.h>

PostTrackingWindow::PostTrackingWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PostTrackingWindow)
	, _angle(0)
	, _style(0)
	, _moveablePost(NULL)
	, _course_c1(3)
	, _course_c2(1)
	, _course_c3(1)
{
	ui->setupUi(this);

	QDoubleValidator *myDblVal = new QDoubleValidator(-199.99, 199.99, 2, ui->lineEdit_lon);
	myDblVal->setNotation(QDoubleValidator::StandardNotation);
	ui->lineEdit_lon->setValidator( myDblVal );

	QDoubleValidator *myDblVal1 = new QDoubleValidator(-199.99, 199.99, 2, ui->lineEdit_lat);
	myDblVal1->setNotation(QDoubleValidator::StandardNotation);
	ui->lineEdit_lat->setValidator( myDblVal1 );

	//Map init.
	_idGenerator = new IdGenerator(this);
	_mapProvider = ui->pwgiswidget->mapProvider();
	_tacticMap = new TacticMap(_mapProvider, _mapProvider->styleFactory(), _idGenerator, this);

	connect(&ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect(ui->btnOpenMap,SIGNAL(clicked()),SLOT(onBtnOpenMap()));
	connect(ui->btnStartTracking,SIGNAL(clicked()),SLOT(onBtnStartTracking()));
	connect(ui->btnStopTracking,SIGNAL(clicked()),SLOT(onBtnStopTracking()));

	connect(ui->onLifeTime, SIGNAL(valueChanged(int)), SLOT(onChangeStyle()));
	connect(ui->onTailLength, SIGNAL(valueChanged(int)), SLOT(onChangeStyle()));

	connect(ui->pushButton_setPost, SIGNAL(clicked()), SLOT(onSetPost()));
	connect(ui->horizontalSlider_course, SIGNAL(valueChanged(int)), SLOT(onRotatePost(int)));

	//some periodic actions init.
	_timer = new QTimer(this);
	_timer->setInterval(70);
	connect(_timer, SIGNAL(timeout()), SLOT(onUpdateObjects()));
}

PostTrackingWindow::~PostTrackingWindow()
{
	_timer->stop();
	_mapProvider->mapManager()->closeMap();
	delete ui;
}

void PostTrackingWindow::onUpdateObjects()
{
	_moveablePost->setHistoryPosition(createNextPoint(this), QDateTime::currentDateTime());
    _moveablePost->updateMap();
}


void PostTrackingWindow::onMapReady()
{
	if ( _moveablePost ) {
		//if duplicated call.
		return;
	}

	_center = _mapProvider->mapBounds()->getMapCenter();
	GeoRectangle* rect = _mapProvider->mapBounds()->getMapBounds();
	_mapHeight = rect->getTop() - rect->getBottom();

	//create objects on map ready step.
	_style = _tacticMap->styleFactory()->createPostStyle();
	_style->icon()->setIconPath("icons/airplane.png");
	_style->icon()->setIconSize(QSize(25, 25));

	_moveablePost = _tacticMap->objectsFactory()->createPost();
	onChangeStyle();

	_moveablePost->setHistoryPosition(createNextPoint(this), QDateTime::currentDateTime());
	_moveablePost->updateMap();

	ui->btnStartTracking->setEnabled(true);
	ui->btnStopTracking->setEnabled(true);
}

void PostTrackingWindow::onBtnOpenMap()
{
	QString mapPath = QFileDialog::getOpenFileName(this,tr( "Open map" ),
			QApplication::applicationDirPath(), tr( "Map files(*.*)" ) );

	if( mapPath.isEmpty() ){
		return;
	}

	_mapProvider->mapManager()->openMap(mapPath);
}

void PostTrackingWindow::onBtnStartTracking()
{
	_timer->start();
	onChangeStyle();
}

void PostTrackingWindow::onBtnStopTracking()
{
	_timer->stop();
}

void PostTrackingWindow::onChangeStyle()
{
	if(!_style) {
		return;
	}

	_style->tracking()->setTraceLifetime( ui->onLifeTime->value() );
	_style->tracking()->setTraceLength( (ui->onTailLength->value() * 1000) );

	_moveablePost->setStyle(_style);
}
void PostTrackingWindow::onSetPost()
{
	_moveablePost->setPosition( new PwGisLonLat(ui->lineEdit_lon->text().toDouble()
											  ,ui->lineEdit_lat->text().toDouble()
											  ,this) );
	_moveablePost->updateMap();
}

void PostTrackingWindow::onRotatePost(int angle)
{
	ui->lcdNumber_course->display(angle*90);
	_moveablePost->setCourse(angle*90);
	_moveablePost->updateMap();
}

PwGisLonLat* PostTrackingWindow::createNextPoint(QObject* obj)
{
	_angle += 0.174532925/4;
	if ( _angle >= +6.25 ) {
		_angle = 0.0;
	}

	_moveablePost->setCourse(_angle * 57.3 * _course_c3 - 50);

	double R = _mapHeight / _course_c1;
	double lon = _center->getLon() + R * cos( _angle ) * _course_c2;
	double lat = _center->getLat() + R * sin( _angle ) * _course_c2;

	return new PwGisLonLat(lon, lat, obj);
}
