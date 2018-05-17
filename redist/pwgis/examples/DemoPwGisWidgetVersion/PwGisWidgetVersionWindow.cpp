#include "PwGisWidgetVersionWindow.h"
#include "ui_PwGisWidgetVersionWindow.h"

#include <QApplication>
#include <QFileDialog>

PwGisWidgetVersionWindow::PwGisWidgetVersionWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::PwGisWidgetVersionWindow )
{
	ui->setupUi( this );

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnGetVersion, SIGNAL( clicked() ), this, SLOT( onBtnGetVersion() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();
}

PwGisWidgetVersionWindow::~PwGisWidgetVersionWindow()
{
	delete ui;
}

void PwGisWidgetVersionWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "Map files(*.*)" ) ), 30 );
}

void PwGisWidgetVersionWindow::onBtnGetVersion()
{
	QString currentVersion = "current version: " + ui->pwgiswidget->version();
	ui->lblVersion->setText( currentVersion );
}

void PwGisWidgetVersionWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
