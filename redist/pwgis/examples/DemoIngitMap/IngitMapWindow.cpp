#include "IngitMapWindow.h"
#include "ui_IngitMapWindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/PwGisStyle.h>

IngitMapWindow::IngitMapWindow( QWidget *parent ):
	QWidget( parent ),
	ui( new Ui::IngitMapWindow ),
	_trackCounter( 0 )
{
	ui->setupUi( this );

	connect( ui->btnOpenMapDialog, SIGNAL( clicked() ), this, SLOT( onBtnOpenMapDialog() ) );
	connect( ui->btnAddPoints, SIGNAL( clicked() ), this, SLOT( onBtnAddPoints() ) );
	connect( ui->btnAddTrack, SIGNAL( clicked() ), this, SLOT( onBtnAddTrack() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	this->showMaximized();

	loadReadme();
}

IngitMapWindow::~IngitMapWindow()
{
	delete ui;
}

void IngitMapWindow::loadReadme()
{
	QString readmePathName =  QDir::toNativeSeparators( QApplication::applicationDirPath() + "/../../../examples/DemoIngitMap/readme.txt" );
	QFile readmeFile( readmePathName );
	if ( !readmeFile.exists() ) {
		return;
	}

	ui->txtInfo->clear();
	readmeFile.open( QIODevice::ReadOnly );
	ui->txtInfo->appendPlainText( QTextStream( &readmeFile ).readAll() );
}

void IngitMapWindow::onBtnOpenMapDialog()
{
	QString mapfileName = ui->pwgiswidget->mapProvider()->mapManager()->openMapDialog();

	QFile mapFile ( mapfileName );
	if ( !mapFile.exists() ) {
		return;
	}

	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapfileName, 30 );
}

void IngitMapWindow::onBtnAddPoints()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloPoint" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapFontFamily, "times" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ff0000" );
	delete style->apply();

	ui->pwgiswidget->addPoint( "jsPoint1", 29.97664, 60.03893, "point1", "point1", "customHaloPoint" );
	ui->pwgiswidget->addPoint( "jsPoint2", 30.50933, 59.98207, "point2", "point2", "customHaloPoint" );
	ui->pwgiswidget->addPoint( "jsPoint3", 30.50501, 59.85473, "point3", "point3", "customHaloPoint" );
	ui->pwgiswidget->addPoint( "jsPoint4", 30.07508, 59.81605, "point4", "point4", "customHaloPoint" );
	ui->pwgiswidget->addPoint( "jsPoint5", 30.31589, 59.93902, "point5", "point5", "customHaloPoint" );
}

void IngitMapWindow::onBtnAddTrack()
{
	QString gpxFileName = QFileDialog::getOpenFileName(
		this,
		tr( "Open gpx" ), QApplication::applicationDirPath(), tr( "GPX files(*.gpx)" ) );

	QFile gpxFile;
	QTextStream gpxStream;
	gpxFile.setFileName( gpxFileName );
	if ( !gpxFile.exists() ) {
		return;
	}
	if ( !gpxFile.open( QIODevice::ReadOnly ) ) {
		return;
	}
	gpxStream.setDevice( &gpxFile );

	PwGisPointList trackPoints;
	QString gpxLine = gpxStream.readLine();
	QString latlonName = gpxLine;
	int posChar = 0;
	int posStart = 0;
	int posEnd = 0;
	QString lat = "";
	QString lon = "";
	while ( !gpxStream.atEnd() ) {
		gpxLine = gpxStream.readLine();
		latlonName += gpxLine;

		if ( gpxLine.indexOf( "<trkseg>" ) != -1 ) {
			latlonName = "";
			continue;
		}

		if ( gpxLine.indexOf( "</trkpt>" ) != -1 ) {
			lat = "";
			lon = "";

			//latitude
			posChar = latlonName.indexOf( "lat" );
			if ( posChar == -1 ) {
				latlonName = "";
				continue;
			}
			posStart = latlonName.indexOf( "\"", posChar );
			posEnd = latlonName.indexOf( "\"", posStart + 1 );
			lat = latlonName.mid( posStart+1, posEnd - posStart - 1 );

			//longitude
			posChar = latlonName.indexOf( "lon" );
			if ( posChar == -1 ) {
				latlonName = "";
				continue;
			}
			posStart = latlonName.indexOf( "\"", posChar );
			posEnd = latlonName.indexOf( "\"", posStart + 1 );
			lon = latlonName.mid( posStart+1, posEnd - posStart - 1 );

			trackPoints.append( new PwGisLonLat( lon.toDouble(), lat.toDouble() ) );
			latlonName = "";
		}
	}
	gpxFile.close();

	_trackCounter++;
	QString trackID = QString( "jsDemoGpxTrack%1" ).arg( _trackCounter );
	ui->pwgiswidget->addPath( trackID, &trackPoints, "", "", "pathWithArrows arrowStyle redline" );
}

void IngitMapWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}
