#include "DemoAtlasProfiles.h"
#include "ui_DemoAtlasProfiles.h"
#include <QFileDialog>
#include <PwGis/IMapManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapManagerEvents.h>

#define PROFILE_NAME   "DemoAtlasProfiles"

DemoAtlasProfiles::DemoAtlasProfiles(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DemoAtlasProfiles)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_profileManager = ui->pwgiswidget->mapProvider()->profileManager();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( ui->btnRegisterProfile, SIGNAL(clicked()), SLOT(onRegisterProfile()));
	connect( ui->btnUnregisterProfile, SIGNAL(clicked()), SLOT(onUnregisterProfile()));
	connect( ui->btnOpenAtlas , SIGNAL(clicked()), SLOT(onOpenAtlas()));
	connect( ui->btnCloseAtlas, SIGNAL(clicked()), SLOT(onCloseAtlas()));

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	//ui->pwgiswidget->enableDebugger(true);
}

DemoAtlasProfiles::~DemoAtlasProfiles()
{
	delete ui;
}

void DemoAtlasProfiles::onOpenAtlas()
{
	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openAtlas();
}

void DemoAtlasProfiles::onCloseAtlas()
{
	_mapManager->closeAtlas();
}

void DemoAtlasProfiles::onMapReady()
{
	GeoRectangle* bounds = ui->pwgiswidget->mapProvider()->mapBounds()->getMapBounds();
	double lon = 30;
	double lat = 60;

	ui->pwgiswidget->addMarker("m1",lon, lat, "marker m1", "tooltip m1", 0, "myStyle");

	bounds->deleteLater();
}

void DemoAtlasProfiles::onRegisterProfile()
{
	//Register profile from Resources
	_profileManager->registerProfile(PROFILE_NAME, ":/DemoAtlasProfiles/profile-atlas");
	onOpenAtlas();
}

void DemoAtlasProfiles::onUnregisterProfile()
{
	_profileManager->unregisterProfile(PROFILE_NAME);
	onOpenAtlas();
}

