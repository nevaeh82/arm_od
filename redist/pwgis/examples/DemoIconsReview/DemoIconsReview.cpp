#include "DemoIconsReview.h"
#include "ui_DemoIconsReview.h"
#include <QDir>
#include <qmath.h>
#include <QFileDialog>
#include <PwGis/IMapBounds.h>
#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/objects/Marker.h>
#include <PwGis/objects/IStyleFactory.h>
#include <PwGis/objects/MapObjectsFactory.h>
#include <PwGis/objects/MapObjectStyle.h>
#include <PwGis/objects/IconStyle.h>
#include <PwGis/PwGisLonLat.h>
#include "IconStyles.h"

using namespace Pw::Gis;

DemoIconsReview::DemoIconsReview(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoIconsReview)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_mapBounds = ui->pwgiswidget->mapProvider()->mapBounds();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( ui->btnLoadMap, SIGNAL(clicked()), SLOT(onLoadMap()));

	QString styles = getallIconsStyleList();
	styleList = styles.split(" ");

	this->showMaximized();

	onLoadMap();
}

void DemoIconsReview::onLoadMap()
{
	_mapManager->closeMap();
	QString mapFile = QFileDialog::getOpenFileName(this,tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));
	if(mapFile == "") return;
	_mapManager->openMap(mapFile,30);
}

DemoIconsReview::~DemoIconsReview()
{
	_mapManager->closeMap();
	delete ui;
}

void DemoIconsReview::onMapReady()
{
	GeoRectangle* bounds = _mapBounds->getMapBounds();

	// распределяем иконки по квадрату. На одной стороне squareSide иконок:
	int squareSide = floor(sqrt((double)styleList.length()))+1;

	double deltaX = (bounds->getRight()-bounds->getLeft())/(squareSide+1);
	double deltaY = (bounds->getBottom()-bounds->getTop())/(squareSide+1);

	int num = styleList.length();

	for(int i=0;i<num;i++){
		int x = i % squareSide;
		int y = i / squareSide;
		double lon = bounds->getLeft()+deltaX*(x+1);
		double lat = bounds->getTop()+deltaY*(y+1);
		ui->pwgiswidget->addMarker(QString("Marker %1").arg(i),lon,lat,styleList.at(i),styleList.at(i),0,styleList.at(i)+ " selectAndDrag");
	}
}

void DemoIconsReview::addMarker(double lon, double lat, QString markerStyle)
{
	IMapProvider* mapProvider = ui->pwgiswidget->mapProvider();
	// создаем стиль
	MapObjectStyle *style = mapProvider->styleFactory()->createObjectStyle();

	// задаем иконку
	style->icon()->setIconPath("images/aircraft16.png");

	// создаем маркер
	Pw::Gis::Marker *marker = mapProvider->objectsFactory()->createMarker();

	// устанавливаем стиль
	marker->setStyle(style);

	//устанавливаем позицию и поворот маркера
	marker->position()->setLon(lon);
	marker->position()->setLat(lat);

	//задаем всплывающую подсказку
	marker->setName(markerStyle);
	marker->setToolTip(markerStyle);

	// добавляем маркер на карту
    marker->updateMap();
}
