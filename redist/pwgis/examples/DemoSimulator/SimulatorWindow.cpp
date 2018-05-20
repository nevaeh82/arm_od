#include <QTimer>
#include <QDateTime>
#include <QDebug>

#include "SimulatorWindow.h"
#include "ui_SimulatorWindow.h"
#include <PwGis/IdGenerator.h>
#include <PwGis/MapProvider.h>
#include <PwGisLonLat.h>
#include <PwGis/TacticMap/TacticMap.h>
#include <PwGis/TacticMap/Post.h>
#include <PwGis/objects/UserInteractionStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/Marker.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/Polygon.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/TacticMap/styles/PostStyle.h>
#include <PwGis/TacticMap/styles/TrackingStyle.h>
#include <PwGis/TacticMap/styles/ITacticStyleFactory.h>
#include <QFileDialog>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapBounds.h>
#include <PwGis/objects/IconStyle.h>
#include <PwGis/PwGisPointList.h>
#include <math.h>
#include <PwGis/MapObjectEvents.h>

SimulatorWindow::SimulatorWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SimulatorWindow),
	_moveablePostRed(NULL),
	_moveablePostBlue(NULL),
	_angle(0),
	_cx(60),
	_cy(30.2),
	_r(0.1)
{
	ui->setupUi(this);

	//Map init.
	_idGenerator = new IdGenerator(this);
	_mapProvider = ui->pwgiswidget->mapProvider();
	//ui->pwgiswidget->enableDebugger(true);
	_tacticMap = new TacticMap(_mapProvider, _mapProvider->styleFactory(), _idGenerator, this);

	ui->lineStyleGroupBox->setProvider(_mapProvider);

	connect(&ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect(ui->btnOpenMap,SIGNAL(clicked()),SLOT(onBtnOpenMap()));
	connect(ui->btnStartTracking,SIGNAL(clicked()),SLOT(onBtnStartTracking()));
	connect(ui->btnStopTracking,SIGNAL(clicked()),SLOT(onBtnStopTracking()));

	//some periodic actions init.
	_timer = new QTimer(this);
	_timer->setInterval(100);

	connect(_timer, SIGNAL(timeout()), SLOT(onUpdateObjects()));

	connect(ui->itemComboBox,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentItemChanged(int)));

	Pw::Gis::MapObjectEvents* mapObjectEvents = &(_mapProvider->objectsManager()->events());
	connect( mapObjectEvents, SIGNAL( featureModified( const QString&, const QString& ) ), this, SLOT( onMapObjectChanged( const QString&, const QString& ) ) );
	connect( mapObjectEvents, SIGNAL( featureAdded(const QString&, const QString& )), SLOT( onFeatureAdded(const QString&, const QString& ) ));
}

SimulatorWindow::~SimulatorWindow()
{
	_timer->stop();
	delete _tacticMap;
	_mapProvider->mapManager()->closeMap();
	delete ui;
}

void SimulatorWindow::onMapObjectChanged(const QString& id, const QString& type)
{
	addToLog(id, type, QString(__FUNCTION__));
}

void SimulatorWindow::onFeatureAdded(const QString& id, const QString& type){
	addToLog(id, type, QString(__FUNCTION__));
}

void SimulatorWindow::addToLog(const QString& id, const QString& type, const QString& text)
{
	ui->textBrowserLog->append(QString(QString("(%1:%2:%3)").arg(id).arg(type).arg(text)));
}

void SimulatorWindow::onUpdateObjects()
{
	PwGisLonLat* point = new PwGisLonLat(this);
	static double course = 0;
	getNextPoint(point, &course);
	_moveablePostRed->setHistoryPosition(point, QDateTime::currentDateTime());
	_moveablePostRed->setCourse(course);
    _moveablePostRed->updateMap();

	PwGisLonLat* point2 = new PwGisLonLat(this);
	point2->setLat(point->getLat() + 0.05);
	point2->setLon(point->getLon() + 0.05);
	_moveablePostBlue->setHistoryPosition(point2, QDateTime::currentDateTime());
	_moveablePostBlue->setCourse(course);
    _moveablePostBlue->updateMap();
	for(int i=0;i<_markerList.size();i++){
		PwGisLonLat* point3 = new PwGisLonLat(this);
		point3->setLat(point->getLat() + 0.1 * (i+1));
		point3->setLon(point->getLon() + 0.1 * (i+1));
		_markerList.at(i)->setPosition(point3);
		_markerList.at(i)->setRotate(rand()%360);
		_markerList.at(i)->updateMap();
	}

}

void SimulatorWindow::onMapReady()
{
	if ( _moveablePostRed || _moveablePostBlue) {
		return;
	}

	_moveablePostRed = _tacticMap->objectsFactory()->createPost();
	_moveablePostBlue = _tacticMap->objectsFactory()->createPost();

	_styleRedPlane = _tacticMap->styleFactory()->createPostStyle();
	_styleRedPlane->line()->setColor(QColor(170,0,0));
	_styleRedPlane->tracking()->setTraceLifetime(4);
	_styleRedPlane->icon()->setIconPath("images/tacticmap/PostPlaneRed.png");

	_styleBluePlane = _tacticMap->styleFactory()->createPostStyle();
	_styleBluePlane->line()->setColor(QColor(0,170,255));
	_styleBluePlane->line()->setDashStyle(LineStyle::Dash);
	_styleBluePlane->tracking()->setTraceLifetime(6);
	_styleBluePlane->icon()->setIconPath("images/tacticmap/PostPlaneBlue.png");

	_center = _mapProvider->mapBounds()->getMapCenter();
	GeoRectangle* rect = _mapProvider->mapBounds()->getMapBounds();
	_mapHeight = rect->getTop() - rect->getBottom();

	//Creating first "Plane" post
	_moveablePostRed->setStyle(_styleRedPlane);
    _moveablePostRed->updateMap();

	//Creating second "Plane" post
	_moveablePostBlue->setStyle(_styleBluePlane);
    _moveablePostBlue->updateMap();

	ui->btnStartTracking->setEnabled(true);
	ui->btnStopTracking->setEnabled(true);
	ui->itemComboBox->setEnabled(true);
	ui->lineStyleGroupBox->setEnabled(true);

	_moveablePostRed->setName("Red Plane");
	_moveablePostBlue->setName("Blue Plane");

	ui->itemComboBox->addItem(_moveablePostRed->name(), qVariantFromValue((void *) _moveablePostRed));
	ui->itemComboBox->addItem(_moveablePostBlue->name(), qVariantFromValue((void *) _moveablePostBlue));

	for(int i=0;i<10;i++){
		Marker* marker = _mapProvider->objectsFactory()->createMarker();
		marker->position()->setLat(60.13844589135558 + 0.01*i);
		marker->position()->setLon(30.003569030761715 + 0.01*i);
		MapObjectStyle* style = _mapProvider->styleFactory()->createObjectStyle();
		style->icon()->setIconPath("images/station48.png");
		style->icon()->setIconSize(QSize(48,48));

		marker->setStyle(style);

		marker->updateMap();
		_markerList.append(marker);
	}

	_point1 = _mapProvider->objectsFactory()->createMarker();
	_point1->position()->setLat(60.03844589135558);
	_point1->position()->setLon(30.103569030761715);
	MapObjectStyle* style = _mapProvider->styleFactory()->createObjectStyle();
	style->icon()->setIconPath("images/tacticmap/PointCircleRed.png");
	style->icon()->setIconSize(QSize(11,11));
	style->text()->setBackgroundColor(QColor(170,0,0));
	style->text()->setForegroundColor(Qt::white);
	_point1->setStyle(style);
	_point1->setName("Point 1");
	_point1->updateMap();

	_point2 = _mapProvider->objectsFactory()->createMarker();
	_point2->position()->setLat(60.03844589135558 - 0.03);
	_point2->position()->setLon(30.103569030761715 - 0.03);
	style = _mapProvider->styleFactory()->createObjectStyle();
	style->icon()->setIconPath("images/tacticmap/PointCircleRed.png");
	style->icon()->setIconSize(QSize(11,11));
	style->text()->setBackgroundColor(QColor(170,0,0));
	style->text()->setForegroundColor(Qt::white);
	style->userInteraction()->setAllowReshape(true);
	_point2->setStyle(style);
	_point2->setName("Point 2");
	_point2->updateMap();

	_polygon = _mapProvider->objectsFactory()->createPolygon();
	_polygon->setToolTip(QString(""));
	_polygon->setStyle(style);

	double lons[] = {30.1, 30.13, 30.2, 30.27, 30.3, 30.27, 30.2, 30.13};
	double lats[] = {60, 60.07, 60.1, 60.07, 60, 59.93, 59.9, 59.93};
	for(int i=0;i<sizeof(lons)/sizeof(lons[0]);i++){
		PwGisLonLat* point = new PwGisLonLat(this);
		point->setLat(lats[i]);
		point->setLon(lons[i]);
		_polygon->corners()->append(point);
	}
	_polygon->updateMap();

	onUpdateObjects();
}

void SimulatorWindow::onBtnOpenMap()
{
	QString mapPath = QFileDialog::getOpenFileName(this,tr( "Open map" ),
				QApplication::applicationDirPath(), tr( "Map files(*.*)" ));

	if( mapPath.isEmpty() ){
		return;
	}

	_mapProvider->mapManager()->openMap(mapPath);
}

void SimulatorWindow::onBtnStartTracking()
{
	_timer->start();
}

void SimulatorWindow::onBtnStopTracking()
{
	_timer->stop();
}

void SimulatorWindow::getNextPoint(PwGisLonLat* point, double* angle)
{
	point->setLon(cos((180 - *angle) * M_PI / 180) * _r + _cy);
	point->setLat(sin((180 - *angle) * M_PI / 180) * _r + _cx);
	*angle+=2;
}

void SimulatorWindow::onCurrentItemChanged(int index)
{
	TracedObjectAbstract* item = (TracedObjectAbstract*) ui->itemComboBox->itemData(index,Qt::UserRole).value<void*>();
	if(item == NULL){
		return;
	}

	ui->lineStyleGroupBox->setItem(item);
}
