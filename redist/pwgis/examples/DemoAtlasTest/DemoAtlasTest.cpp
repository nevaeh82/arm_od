#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <MapManagerEvents.h>
#include <MapBoundsEvents.h>

#include "DemoAtlasTest.h"
#include "ui_DemoAtlasTest.h"

DemoAtlasTest::DemoAtlasTest(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoAtlasTest),
	_mapReadyCounter(0)
	,_atlasReadyCounter(0)
	,_externalWindget(0)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( &_mapManager->events(), SIGNAL(atlasReady()), SLOT(onAtlasReady()));

	connect( ui->btnOpenMap, SIGNAL(clicked()), SLOT(onOpenMap()));
	connect( ui->btnCloseMap, SIGNAL(clicked()), SLOT(onCloseMap()));
	connect( ui->btnOpenAtlas, SIGNAL(clicked()), SLOT(onOpenAtlas()));
	connect( ui->btnCloseAtlas, SIGNAL(clicked()), SLOT(onCloseAtlas()));

	connect( ui->pushButton_mF, SIGNAL(clicked()), SLOT(openMapFrom()) );
	connect( ui->pushButton_aF, SIGNAL(clicked()), SLOT(openAtlasFrom()) );

	//ui->pwgiswidget->enableDebugger(true);
	updateSignalView();
}

DemoAtlasTest::~DemoAtlasTest()
{
	isOpenWidget();
	delete ui;
}

void DemoAtlasTest::updateSignalView()
{
	ui->mapReadyEdit->setText(QString("%1").arg(_mapReadyCounter));
	ui->atlasReadyEdit->setText(QString("%1").arg(_atlasReadyCounter));
}

void DemoAtlasTest::onMapReady()
{
	_mapReadyCounter++;
	updateSignalView();
}

void DemoAtlasTest::onAtlasReady()
{
	_atlasReadyCounter++;
	updateSignalView();
}

void DemoAtlasTest::onOpenMap()
{
	QString mapFile = QApplication::applicationDirPath() +
			"/../../../MapSamples/Ingit/abakan/ABAKAN.chart";

	if (!QFile::exists( mapFile )) {
		mapFile = QFileDialog::getOpenFileName( this, tr( "Open map" ),
												QApplication::applicationDirPath(),
												tr( "All files(*.*)" ) );
	}

	if (mapFile.isNull()) {
		return;
	}

	_mapManager->openMap( mapFile, 30 );
}

void DemoAtlasTest::onCloseMap()
{
	isOpenWidget();
	_mapManager->closeMap();
	_atlasReadyCounter = 0;
	_mapReadyCounter = 0;
	updateSignalView();
}

void DemoAtlasTest::onOpenAtlas()
{
	_mapManager->openAtlas(8080);
}

void DemoAtlasTest::onCloseAtlas()
{
	isOpenWidget();
	_mapManager->closeAtlas();
	_atlasReadyCounter = 0;
	_mapReadyCounter = 0;
	updateSignalView();
}

void DemoAtlasTest::openMapFrom()
{
	isOpenWidget();
	_externalWindget = new PwGisWidget(0);
	_externalWindget->setWindowTitle("Widget_openMapFrom");
	_externalWindget->resize(400, 300);
	Pw::Gis::IMapManager *manager = _externalWindget->mapProvider()->mapManager();
	manager->openMapFrom("localhost", 8080);
	connect(&manager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	_externalWindget->show();
}

void DemoAtlasTest::openAtlasFrom()
{
	isOpenWidget();
	_externalWindget = new PwGisWidget(0);
	_externalWindget->setWindowTitle("Widget_openAtlasFrom");
	_externalWindget->resize(400, 300);
	Pw::Gis::IMapManager *manager = _externalWindget->mapProvider()->mapManager();
	manager->openAtlasFrom("localhost", 8080);
	connect(&manager->events(), SIGNAL(atlasReady()), SLOT(onAtlasReady()));
	connect(&manager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	_externalWindget->show();
}

void DemoAtlasTest::isOpenWidget()
{
	if(_externalWindget) {
		_externalWindget->close();
		_externalWindget->deleteLater();
		_externalWindget = 0;
	}
}
