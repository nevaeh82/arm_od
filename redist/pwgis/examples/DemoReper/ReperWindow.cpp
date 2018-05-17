#include "ReperWindow.h"
#include "ui_ReperWindow.h"

#include <QApplication>
#include <QFile>
#include <PwGis/IMapBounds.h>


ReperWindow::ReperWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::ReperWindow )
{
	ui->setupUi(this);

	connect( ui->btnOpenMapDialog, SIGNAL( clicked() ), this, SLOT( onBtnOpenMapDialog() ) );
	connect( ui->btnAddOsmReper, SIGNAL( clicked() ), this, SLOT( onBtnAddOsmReper() ) );
	connect( ui->btnAddSatReper, SIGNAL( clicked() ), this, SLOT( onBtnAddSatReper() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();
}

ReperWindow::~ReperWindow()
{
	delete ui;
}

void ReperWindow::onBtnOpenMapDialog()
{
	QString mapfileName = ui->pwgiswidget->mapProvider()->mapManager()->openMapDialog();

	QFile mapFile ( mapfileName );
	if ( !mapFile.exists() ) {
		return;
	}

	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapfileName, 30 );
}

void ReperWindow::onBtnAddOsmReper()
{
	ui->pwgiswidget->addMarker( "m1",37.51143, 55.80923, "Stiks-osm", "Stiks-osm", 0, "" );
	ui->pwgiswidget->addMarker( "m2",37.62144, 55.75257, "Kreml", "Kreml", 0, "" );
	ui->pwgiswidget->addMarker( "m3",37.53078, 55.70293, "Mgu", "Mgu", 0, "" );
	ui->pwgiswidget->addMarker( "m4",37.67652, 55.79471, "Sokolniki", "Sokolniki", 0, "" );
	ui->pwgiswidget->mapProvider()->mapBounds()->zoomMapTo( 37.502222, 55.806111, 37.522222, 55.811944 );
}

void ReperWindow::onBtnAddSatReper()
{
	ui->pwgiswidget->addMarker( "m5", 37.511386, 55.809275, "Stiks-googleSat", "Stiks-googleSat", 0, "" );
	ui->pwgiswidget->mapProvider()->mapBounds()->zoomMapTo( 37.502222, 55.806111, 37.522222, 55.811944 );
}

void ReperWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
