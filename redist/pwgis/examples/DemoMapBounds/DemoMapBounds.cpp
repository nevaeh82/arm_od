#include "DemoMapBounds.h"
#include "ui_DemoMapBounds.h"
#include <QDir>
#include <QFileDialog>

#include <PwGis/IMapBounds.h>
#include <PwGis/MapBoundsEvents.h>

#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/MapZoomInfo.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/PwGisLonLat.h>

DemoMapBounds::DemoMapBounds(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoMapBounds)
{
	ui->setupUi(this);

	_mapBounds = ui->pwgiswidget->mapProvider()->mapBounds();
	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();

	connect(ui->btnZoomMapTo,SIGNAL(clicked()),this,SLOT(onBtnZoomMapTo()));
	connect(ui->btnZoomMapToByOffset,SIGNAL(clicked()),this,SLOT(onBtnZoomMapToByOffset()));
	connect(ui->btnSetMapCenter,SIGNAL(clicked()),this,SLOT(onBtnSetMapCenter()));
	connect(ui->btnGetMapBounds,SIGNAL(clicked()),this,SLOT(onBtnGetMapBounds()));
	connect(ui->btnGetViewBounds,SIGNAL(clicked()),this,SLOT(onBtnGetViewBounds()));
	connect(ui->btnGetZoom,SIGNAL(clicked()),this,SLOT(onBtnGetZoom()));
	connect(ui->btnSetZoom,SIGNAL(clicked()),this,SLOT(onBtnSetZoom()));
	connect(ui->btnGetZoomInfo,SIGNAL(clicked()),this,SLOT(onBtnGetZoomInfo()));
	connect(&_mapBounds->events(),SIGNAL(zoomChanged(int)),this,SLOT(onZoomChanged(int)));
	connect(&_mapManager->events(),SIGNAL(mapReady()),this,SLOT(onMapReady()));

	QString mapFile = QFileDialog::getOpenFileName(this,tr("Open map"));
	if(mapFile == "") return;

	_mapManager->openMap(mapFile);
}

DemoMapBounds::~DemoMapBounds()
{
	_mapManager->closeMap();
	delete ui;
}

void DemoMapBounds::onMapReady()
{
	GeoRectangle* rect = _mapBounds->getMapBounds();
	ui->editLeft->setText(QString("%1").arg(rect->getLeft() + 1));
	ui->editRight->setText(QString("%1").arg(rect->getRight() + 1));
	ui->editTop->setText(QString("%1").arg(rect->getTop() - 1));
	ui->editBottom->setText(QString("%1").arg(rect->getBottom() - 1));

	ui->editLon->setText(QString("%1").arg( (rect->getRight() + rect->getLeft())/2 ));
	ui->editLat->setText(QString("%1").arg( (rect->getTop() + rect->getBottom())/2 ));
	ui->editOffsetB->setText("1000");
	ui->editOffsetR->setText("1000");

	PwGisLonLat* res = _mapBounds->getMapCenter();
	ui->editCenterLon->setText(QString("%1").arg( res->getLon() ));
	ui->editCenterLat->setText(QString("%1").arg( res->getLat() ));

	ui->editSetZoom->setText("6");

	ui->editZoom->setText(QString("%1").arg(_mapBounds->getCurrentZoom()));
	delete rect;
}

void DemoMapBounds::onBtnZoomMapTo()
{
	_mapBounds->zoomMapTo(ui->editLeft->text().toDouble(),ui->editBottom->text().toDouble(),
							   ui->editRight->text().toDouble(),ui->editTop->text().toDouble());
}

void DemoMapBounds::onBtnZoomMapToByOffset()
{
	_mapBounds->zoomMapToByOffset(ui->editLon->text().toDouble(),ui->editLat->text().toDouble(),
									   ui->editOffsetB->text().toInt(),ui->editOffsetR->text().toInt());
}

void DemoMapBounds::onBtnSetMapCenter()
{
	_mapBounds->setMapCenter(ui->editCenterLon->text().toDouble(),ui->editCenterLat->text().toDouble());
}

void DemoMapBounds::onBtnGetMapBounds()
{
	GeoRectangle* rect = _mapBounds->getMapBounds();
	ui->plainTextEdit->appendPlainText(QString("Map bounds:\t  %1\t  %2\t  %3\t  %4").arg(rect->getLeft()).arg(rect->getBottom()).arg(rect->getRight()).arg(rect->getTop()));
	delete rect;
}

void DemoMapBounds::onBtnGetViewBounds()
{
	GeoRectangle* rect = _mapBounds->getViewBounds();
	ui->plainTextEdit->appendPlainText(QString("View bounds:\t  %1\t  %2\t  %3\t  %4").arg(rect->getLeft()).arg(rect->getBottom()).arg(rect->getRight()).arg(rect->getTop()));
	delete rect;
}

void DemoMapBounds::onBtnGetZoom()
{
	ui->plainTextEdit->appendPlainText(QString("Current Zoom:\t  %1").arg(_mapBounds->getCurrentZoom()));
}

void DemoMapBounds::onBtnSetZoom()
{
	_mapBounds->setCurrentZoom(ui->editSetZoom->text().toInt());
}

void DemoMapBounds::onBtnGetZoomInfo()
{
	MapZoomInfo* info = _mapBounds->getMapZoomInfo();

	QString str("Zoom Info:\nscales: ");
	for(int i=0;i<info->scales()->getCount();i++){
		str.append(QString("  %1 ").arg(info->scales()->get(i)));
	}

	str.append("\nresolutions:  ");
	for(int i=0;i<info->resolutions()->getCount();i++){
		str.append(QString("  %1 ").arg(info->resolutions()->get(i)));
	}

	str.append(QString("\nminScale: %1").arg(info->minScale()));
	str.append(QString("\nmaxScale: %1").arg(info->maxScale()));
	str.append(QString("\nminResolution: %1").arg(info->minResolution()));
	str.append(QString("\nmaxResolution: %1").arg(info->maxResolution()));
	str.append(QString("\nzoom levels: %1\n").arg(info->numZoomLevels()));

	ui->plainTextEdit->appendPlainText(str);

	delete info;
}

void DemoMapBounds::onZoomChanged(int zoomLevel)
{
	ui->editZoom->setText(QString("%1").arg(zoomLevel));
}
