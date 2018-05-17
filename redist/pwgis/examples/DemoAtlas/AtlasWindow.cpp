#include "AtlasWindow.h"
#include "ui_AtlasWindow.h"

#include <QApplication>
#include <PwGis/IMapBounds.h>
#include <PwGis/MapManagerEvents.h>


AtlasWindow::AtlasWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::AtlasWindow )
{
	ui->setupUi(this);

	connect( ui->btnOpenAtlas, SIGNAL( clicked() ), this, SLOT( onBtnOpenAtlas() ) );
	connect( ui->btnCloseAtlas, SIGNAL( clicked() ), this, SLOT( onBtnCloseAtlas() ) );
	connect( ui->btnClearLog, SIGNAL( clicked() ), this, SLOT( onBtnClearLog() ) );
	Pw::Gis::IMapManager* mapManager = ui->pwgiswidget->mapProvider()->mapManager();

	connect( &mapManager->events(), SIGNAL( mapReady() ), this, SLOT( onMapReady() ) );
	connect( &mapManager->events(), SIGNAL( atlasReady() ), this, SLOT( onAtlasReady() ) );

	ui->pwgiswidget->enableDebugger(true);

	this->showMaximized();
}

AtlasWindow::~AtlasWindow()
{
	delete ui;
}

void AtlasWindow::onBtnOpenAtlas()
{
	ui->txtLog->appendPlainText( "click button `open atlas`" );

	ui->pwgiswidget->mapProvider()->mapManager()->openAtlas();
}

void AtlasWindow::onBtnCloseAtlas()
{
	ui->txtLog->appendPlainText( "click button `close atlas`" );

	ui->pwgiswidget->mapProvider()->mapManager()->closeAtlas();
}

void AtlasWindow::onBtnClearLog()
{
	ui->txtLog->clear();
}

void AtlasWindow::onMapReady()
{
	ui->txtLog->appendPlainText( "onMapReady" );

	ui->pwgiswidget->addMarker( "m1",30.309621, 59.936765, "marker m1", "tooltip m1", 0, "" );
	ui->pwgiswidget->addMarker( "m2",30.317466, 59.906214, "marker m2", "tooltip m2", 0, "" );
	ui->pwgiswidget->mapProvider()->mapBounds()->zoomMapTo( 30.233611, 59.901111, 30.394444, 59.941389 );
}

void AtlasWindow::onAtlasReady()
{
	ui->txtLog->appendPlainText( "onAtlasReady" );
}
