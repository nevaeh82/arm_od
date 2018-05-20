#include "DemoStyleCombine.h"
#include "ui_DemoStyleCombine.h"
#include <QFileDialog>
#include <PwGis/IMapManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapManagerEvents.h>

#define PROFILE_NAME   "DemoStyleCombine"

DemoStyleCombine::DemoStyleCombine(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoStyleCombine)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_profileManager = ui->pwgiswidget->mapProvider()->profileManager();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( ui->btnOpenMap, SIGNAL(clicked()), SLOT(onOpenMap()));
	connect( ui->btnCloseMap, SIGNAL(clicked()), SLOT(onCloseMap()));
	connect( ui->btnRegisterProfile, SIGNAL(clicked()), SLOT(onRegisterProfile()));
	connect( ui->btnUnregisterProfile, SIGNAL(clicked()), SLOT(onUnregisterProfile()));
	connect( ui->btnOpenAtlas , SIGNAL(clicked()), SLOT(onOpenAtlas()));
	connect( ui->btnCloseAtlas, SIGNAL(clicked()), SLOT(onCloseAtlas()));

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	//ui->pwgiswidget->enableDebugger(true);
}

DemoStyleCombine::~DemoStyleCombine()
{
	delete ui;
}

void DemoStyleCombine::onOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));

	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openMap(mapFile, 30);
}

void DemoStyleCombine::onOpenAtlas()
{
	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openAtlas();
}

void DemoStyleCombine::onCloseAtlas()
{
	_mapManager->closeAtlas();
}

void DemoStyleCombine::onCloseMap()
{
	_mapManager->closeMap();
}

void DemoStyleCombine::onMapReady()
{
	GeoRectangle* bounds = ui->pwgiswidget->mapProvider()->mapBounds()->getMapBounds();
	double lon = 30.309621;
	double lat = 59.936765;

	ui->pwgiswidget->addMarker("m1",lon, lat, "marker m1", "tooltip m1", 0, "mark tooltip labelColor labelOutline font behavour line lineOutline");
	ui->pwgiswidget->addLine("s1", lon, lat+2, lon+3, lat, "line1", "tooltip line", "mark tooltip labelColor labelOutline font behavour line lineOutline");

	ui->pwgiswidget->addMarker("m2",30.317466, 59.906214, "marker m1", "tooltip m1", 0, "icon_station24");


	bounds->deleteLater();
}

void DemoStyleCombine::onRegisterProfile()
{
	//Register profile from Resources
	_profileManager->registerProfile(PROFILE_NAME, ":/DemoProfiles/profile");
}

void DemoStyleCombine::onUnregisterProfile()
{
	_profileManager->unregisterProfile(PROFILE_NAME);
}

