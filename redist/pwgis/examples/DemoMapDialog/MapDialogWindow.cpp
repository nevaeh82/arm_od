#include "MapDialogWindow.h"
#include "ui_MapDialogWindow.h"

#include <QApplication>
#include <QFile>

MapDialogWindow::MapDialogWindow( QWidget *parent ):
	QWidget( parent ),
	ui( new Ui::MapDialogWindow )
{
	ui->setupUi( this );

	connect( ui->btnOpenMapDialog, SIGNAL( clicked() ), this, SLOT( onBtnOpenMapDialog() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();
}

MapDialogWindow::~MapDialogWindow()
{
	delete ui;
}

void MapDialogWindow::onBtnOpenMapDialog()
{
	//QString mapfileName = ui->pwgiswidget->mapProvider()->mapManager()->openMapDialog("C:/IngitMap/abakan");
	QString mapfileName = ui->pwgiswidget->mapProvider()->mapManager()->openMapDialog();

	QFile mapFile ( mapfileName );
	if ( !mapFile.exists() ) {
		return;
	}

	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapfileName, 30 );
}

void MapDialogWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
