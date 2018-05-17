#include "DemoProfiles.h"
#include "ui_DemoProfiles.h"
#include <QFileDialog>
#include <PwGis/IMapManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapManagerEvents.h>

#define PROFILE_NAME   "DemoProfiles"

DemoProfiles::DemoProfiles(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DemoProfiles)
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

DemoProfiles::~DemoProfiles()
{
	delete ui;
}

void DemoProfiles::onOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));

	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openMap(mapFile, 30);
}

void DemoProfiles::onOpenAtlas()
{
	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openAtlas();
}

void DemoProfiles::onCloseAtlas()
{
	_mapManager->closeAtlas();
}

void DemoProfiles::onCloseMap()
{
	_mapManager->closeMap();
}

void DemoProfiles::onMapReady()
{
	GeoRectangle* bounds = ui->pwgiswidget->mapProvider()->mapBounds()->getMapBounds();
	double lon = 30;
	double lat = 60;

	ui->pwgiswidget->addMarker("m1",lon, lat, "marker m1", "tooltip m1", 0, "myStyle");

	bounds->deleteLater();
}

void DemoProfiles::onRegisterProfile()
{
	//Register profile from Resources
	_profileManager->registerProfile(PROFILE_NAME, ":/DemoProfiles/profile");
}

void DemoProfiles::onUnregisterProfile()
{
	_profileManager->unregisterProfile(PROFILE_NAME);
}

