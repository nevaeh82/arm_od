#include "MapxWindow.h"
#include "ui_MapxWindow.h"

#include <QApplication>
#include <QFileDialog>

MapxWindow::MapxWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MapxWindow)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();
}

MapxWindow::~MapxWindow()
{
	delete ui;
}

void MapxWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open mapx map" ), QApplication::applicationDirPath(), tr( "Mapx files(*.gst)" ) ), 30 );
}

void MapxWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
