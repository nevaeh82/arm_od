#include "BorderMapWindow.h"
#include "ui_BorderMapWindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>

BorderMapWindow::BorderMapWindow( QWidget *parent ) :
	QWidget(parent),
	ui(new Ui::BorderMapWindow)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();

	QPalette thisPalette( palette() );
	thisPalette.setColor( QPalette::Background, Qt::red );
	this->setAutoFillBackground( true );
	this->setPalette( thisPalette );

	QPalette mapPalette( palette() );
	mapPalette.setColor( QPalette::Background, Qt::blue );
	ui->pwgiswidget->setAutoFillBackground( true );
	ui->pwgiswidget->setPalette( mapPalette );
}

BorderMapWindow::~BorderMapWindow()
{
	delete ui;
}

void BorderMapWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "map files(*.*)" ) ), 30 );
}

void BorderMapWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
