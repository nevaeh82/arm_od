#include "TestProfiles.h"
#include "ui_TestProfiles.h"
#include <QFileDialog>
#include <IMapManager.h>
#include <IProfileManager.h>
#include <MapManagerEvents.h>

TestProfiles::TestProfiles(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TestProfiles)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_profileManager = ui->pwgiswidget->mapProvider()->profileManager();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( ui->btnOpenMap, SIGNAL(clicked()), SLOT(onOpenMap()));
	connect( ui->btnCloseMap, SIGNAL(clicked()), SLOT(onCloseMap()));
	connect( ui->btnRegisterProfile, SIGNAL(clicked()), SLOT(onRegisterProfile()));
	connect( ui->btnUnregisterProfile, SIGNAL(clicked()), SLOT(onUnregisterProfile()));

	//ui->pwgiswidget->enableDebugger(true);
}

TestProfiles::~TestProfiles()
{
	delete ui;
}

void TestProfiles::onOpenMap()
{
	_mapManager->setProfile("demo");
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));
	if(mapFile == "") return;
	_mapManager->openMap(mapFile,30);
}

void TestProfiles::onRegisterProfile()
{
	_profileManager->registerProfile("demo", QApplication::applicationDirPath() +"/../profile/PwGisClientApp");
}

void TestProfiles::onUnregisterProfile()
{
	_profileManager->unregisterProfile("demo");
}

void TestProfiles::onCloseMap()
{
	_mapManager->closeMap();
}
