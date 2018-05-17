#include "PointCaptionWindow.h"
#include "ui_PointCaptionWindow.h"

#include <QApplication>
#include <QFileDialog>

PointCaptionWindow::PointCaptionWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PointCaptionWindow)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnAddPoints, SIGNAL( clicked() ), this, SLOT( onBtnAddPoints() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();
}

PointCaptionWindow::~PointCaptionWindow()
{
	delete ui;
}

void PointCaptionWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "Mapx files(*.*)" ) ), 30 );
}

void PointCaptionWindow::onBtnAddPoints()
{
	double deltaLon = 0.2;
	double deltaLat = 0.1375;
	for( int i=0; i < 20; ++i ) {
		ui->pwgiswidget->addMarker( "jsMarker" + QString::number( i ), 27.0 + i*deltaLon, 58.5 + i*deltaLat, "VBS_MiniDevice" + QString::number( i ), "VBS_MiniDevice", 0, "" );
		ui->pwgiswidget->addPoint( "jsPoint" + QString::number( i ), 31.0 + i*deltaLon, 58.5 + i*deltaLat, "VBS_MiniDevice" + QString::number( i ), "VBS_MiniDevice", "" );
	}
}

void PointCaptionWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
