#include <QTimer>
#include "TacticMapWindow.h"
#include "ui_TacticMapWindow.h"
#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/PwGisPointList.h>
#include <PwGis/TacticMap/styles/ITacticStyleFactory.h>
#include <PwGis/TacticMap/styles/RadioSourceStyle.h>
#include <PwGis/TacticMap/styles/BearingStyle.h>
#include <PwGis/TacticMap/styles/PostStyle.h>
#include <PwGis/TacticMap/RadioSource.h>
#include <PwGis/TacticMap/Bearing.h>
#include <PwGis/TacticMap/Post.h>
#include <PwGis/TacticMap/RadioSource.h>
#include <PwGis/TacticMap/Bearing.h>
#include <PwGis/TacticMap/Post.h>
#include <PwGis/objects/Sector.h>
#include <PwGis/TacticMap/TracedObjectAbstract.h>
#include <MapProvider.h>
#include <IdGenerator.h>
#include <PwGis/objects/Marker.h>
#include <PwGis/objects/Path.h>
#include <PwGis/objects/Polygon.h>
#include <PwGis/objects/Circle.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/Rectangle.h>
#include <QFileDialog>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>

TacticMapWindow::TacticMapWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::TacticMapWindow)
{
	ui->setupUi(this);

	_idGenerator = new IdGenerator(this);
	_mapProvider = ui->pwgiswidget->mapProvider();
	_tacticMap = new TacticMap(_mapProvider, _mapProvider->styleFactory(), _idGenerator, this);
	_mapBounds = _mapProvider->mapBounds();

	Pw::Gis::MapManagerEvents* mapEvents = &ui->pwgiswidget->mapProvider()->mapManager()->events();

	connect(ui->btnOpenMap, SIGNAL(clicked()), SLOT(onBtnOpenMap()));
	connect(ui->btnAddBearing, SIGNAL(clicked()), SLOT(onBtnAddBearingClicked()));
	connect(ui->btnAddPost, SIGNAL(clicked()), SLOT(onBtnAddPostClicked()));
	connect(ui->btnAddSlice, SIGNAL(clicked()), SLOT(onBtnAddSliceClicked()));
	connect(ui->btnAddSource, SIGNAL(clicked()), SLOT(onBtnAddSourceClicked()));
	connect(ui->btnClear, SIGNAL(clicked()), SLOT(onBtnClearClicked()));
	connect(ui->btnCrazy, SIGNAL(clicked()), SLOT(onBtnCrazyClicked()));
	connect(mapEvents,SIGNAL(mapReady()),this,SLOT(onMapReady()));

	_mapObjectStyle = _tacticMap->styleFactory()->createMapObjectStyle();
	_styleIri = _tacticMap->styleFactory()->createRadioSourceStyle();
	_stylePost = _tacticMap->styleFactory()->createPostStyle();
	_styleBearing = _tacticMap->styleFactory()->createBearingStyle();

	//	_styleIri->__setTailStyle(_mapObjectStyle);
	//	_styleIri->__setHeadStyle(_mapObjectStyle);
	//	_stylePost->__setTailStyle(_mapObjectStyle);
	//	_stylePost->__setHeadStyle(_mapObjectStyle);
	//	_styleBearing->__setStyle(_mapObjectStyle);

	//ui->pwgiswidget->enableDebugger(true);
}

TacticMapWindow::~TacticMapWindow()
{
	_mapProvider->mapManager()->closeMap();
	delete ui;
}

double TacticMapWindow::getRandomLon()
{
	double width = _bounds->getRight() - _bounds->getLeft();
	double rnd = (double)rand()/100;
	while(rnd > width) rnd -= width;
	return rnd + _bounds->getLeft();
}

double TacticMapWindow::getRandomLat()
{
	double height = _bounds->getTop() - _bounds->getBottom();
	double rnd = (double)rand()/100;
	while(rnd > height) rnd -= height;
	return rnd + _bounds->getBottom();
}

PwGisLonLat* TacticMapWindow::createRandomPoint(QObject* obj)
{
	return new PwGisLonLat(getRandomLon(),getRandomLat(), obj);
}

void TacticMapWindow::onBtnOpenMap()
{
	QString mapPath = QFileDialog::getOpenFileName(this,tr( "Open map" ),
			QApplication::applicationDirPath(), tr( "Map files(*.*)" ) );

	if( mapPath.isEmpty() ){
		return;
	}

	_mapProvider->mapManager()->openMap(mapPath);
}

void TacticMapWindow::onBtnAddSliceClicked()
{
	Pw::Gis::Sector* sector = _mapProvider->objectsFactory()->createSector();

	if(!sector) return;

	sector->setOriginPoint( new PwGisLonLat(getRandomLon(), getRandomLat()));
	sector->setRadius(1000 + (rand()%1000 - 300));
	sector->setStartAngle(90);
	sector->setEndAngle(160);
	sector->setName("I am slice");
	sector->setStyle(_mapObjectStyle);

	sector->updateMap();
}

void TacticMapWindow::onBtnAddBearingClicked()
{
	Bearing* bearing = _tacticMap->objectsFactory()->createBearing();

	bearing->setName("Bearing");
	bearing->setToolTip("Bearing");
	bearing->setTag(this);
	bearing->setStyle(_styleBearing);
	bearing->addDirection(createRandomPoint(this), rand() % 360, QDateTime::currentDateTime());
	bearing->setPosition( createRandomPoint(this));
	bearing->setName("I am bearing");

    bearing->updateMap();
}

void TacticMapWindow::onBtnAddSourceClicked()
{
	static int countSource = 1;
	QString iriId = "iri" + QString().setNum(countSource++);
	RadioSource* iri = _tacticMap->objectsFactory()->createRadioSource();

	//set position without history
	QDateTime now = QDateTime::currentDateTime();
	iri->setHistoryPosition(createRandomPoint(this), now);
	iri->setHistoryPosition(createRandomPoint(this), now.addSecs(10));
	iri->setHistoryPosition(createRandomPoint(this), now.addSecs(20));
	iri->setToolTip(iriId);
	iri->setName("I am source");
	iri->setStyle(_styleIri);

    iri->updateMap();
}

void TacticMapWindow::onBtnAddPostClicked()
{
	static int countPost = 1;
	QString postId = "post" + QString().setNum(countPost++);
	Post* post = _tacticMap->objectsFactory()->createPost();

	QDateTime now = QDateTime::currentDateTime();
	post->setHistoryPosition(createRandomPoint(this), now);
	post->setHistoryPosition(createRandomPoint(this), now.addSecs(10));
	post->setHistoryPosition(createRandomPoint(this), now.addSecs(20));

	post->setToolTip(postId);
	post->setName("I am post");
	post->setStyle(_stylePost);

    post->updateMap();
}

void TacticMapWindow::onBtnCrazyClicked()
{
	for(int i = 0; i < 1000; i++)
	{
		if(!(i % 20)) onBtnAddSliceClicked();
		onBtnAddBearingClicked();
		onBtnAddSourceClicked();
		onBtnAddPostClicked();
	}
}

void TacticMapWindow::onBtnClearClicked()
{
	ui->pwgiswidget->removeAllObjects();
}

void TacticMapWindow::on_btnTestStyle_clicked()
{
}

void TacticMapWindow::onMapReady()
{
	ui->btnAddBearing->setEnabled(true);
	ui->btnAddPost->setEnabled(true);
	ui->btnAddSlice->setEnabled(true);
	ui->btnAddSource->setEnabled(true);
	ui->btnClear->setEnabled(true);
	ui->btnCrazy->setEnabled(true);
	ui->btnAddCircle->setEnabled(true);
	ui->btnAddPath->setEnabled(true);
	ui->btnAddPolygon->setEnabled(true);
	ui->btnAddMarker->setEnabled(true);
	ui->btnRect->setEnabled(true);

	_mapObjectStyle->line()->setThickness(3); //20 - OLD
	//red color FF0000 hex = 16711680 bin
	_mapObjectStyle->line()->setColor(16711680); //5413 - OLD

	_mapObjectStyle->icon()->setIconPath("images/car24.png"); //greenmark24 - OLD
	_mapObjectStyle->icon()->setShowIcon(true);

	//_mapObjectStyle->text()->setFontFamily("Comic Sans");
	//_mapObjectStyle->text()->setFontSize(40);
	//_mapObjectStyle->text()->setForegroundColor(32000);
	//_mapObjectStyle->textStyle()->setForegroundColor(5555);

	_styleIri->icon()->setIconPath("images/phone24.png");
	_styleIri->icon()->setShowIcon(true);
	_styleIri->line()->setColor(32000);

	_stylePost->icon()->setIconPath("images/aircraft24.png");
	_stylePost->icon()->setShowIcon(true);
	_stylePost->line()->setColor(Qt::white);

	_styleBearing->line()->setColor(12345);
	_styleBearing->line()->setThickness(10);
	_styleBearing->text()->setFontFamily("Comic Sans");
	_styleBearing->text()->setFontSize(20);
	_styleBearing->text()->setForegroundColor(99999999);

	_bounds = _mapBounds->getMapBounds();

}

void TacticMapWindow::on_btnAddMarker_clicked()
{
	Pw::Gis::Marker * marker = _mapProvider->objectsFactory()->createMarker();
	marker->setStyle(_mapObjectStyle);
	marker->setName("I am marker");
	marker->position()->setLat(getRandomLat());
	marker->position()->setLon(getRandomLon());
	marker->setToolTip("marker tooltip");
	marker->setRotate(50.0);
    marker->updateMap();
}

void TacticMapWindow::on_btnAddCircle_clicked()
{
	Circle* circle = _mapProvider->objectsFactory()->createCircle();
	circle->setStyle(_mapObjectStyle);
	circle->setRadius(1000);
	circle->originPoint()->setLat(getRandomLat());
	circle->originPoint()->setLon(getRandomLon());
	circle->setName("I am circle");
    circle->updateMap();
}

void TacticMapWindow::on_btnAddPath_clicked()
{
	Pw::Gis::Path* path = _mapProvider->objectsFactory()->createPath();
	path->setStyle(_mapObjectStyle);
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(),path));
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(),path));
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), path));
	path->setName("I am path");
    path->updateMap();
}

void TacticMapWindow::on_btnAddPolygon_clicked()
{
	Polygon* polygon = _mapProvider->objectsFactory()->createPolygon();
	polygon->setStyle(_mapObjectStyle);
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->setName("I am polygon");
    polygon->updateMap();
}

void TacticMapWindow::on_btnRect_clicked()
{
	Rectangle* rect = _mapProvider->objectsFactory()->createRectangle();
	rect->setStyle(_mapObjectStyle);
	rect->topRight()->setLat(getRandomLat());
	rect->topRight()->setLon(getRandomLon());
	rect->bottomLeft()->setLat(getRandomLat());
	rect->bottomLeft()->setLon(getRandomLon());
	rect->setName("I am rect");
    rect->updateMap();
}
