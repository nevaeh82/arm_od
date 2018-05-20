#include "GoogleCacheMapWindow.h"
#include "ui_GoogleCacheMapWindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>

GoogleCacheMapWindow::GoogleCacheMapWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GoogleCacheMapWindow)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnAddTrack, SIGNAL( clicked() ), this, SLOT( onBtnAddTrack() ) );

	this->showMaximized();
}

GoogleCacheMapWindow::~GoogleCacheMapWindow()
{
	delete ui;
}

QString GoogleCacheMapWindow::getRuText( const QString &text )
{
	return QString::fromUtf8( text.toLatin1().data() );
}

void GoogleCacheMapWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "GC files(*.gc)" ) ), 30 );
}

void GoogleCacheMapWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void GoogleCacheMapWindow::onBtnAddTrack()
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
	QString latlonName = "";
	int posChar = 0;
	int posStart = 0;
	int posEnd = 0;
	QString lat = "";
	QString lon = "";
	while ( !gpxStream.atEnd() ) {
		latlonName += gpxLine;

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
		gpxLine = gpxStream.readLine();
	}
	gpxFile.close();

	ui->pwgiswidget->addPath( "jsGoogleGpxTrack", &trackPoints, "", "", "pathWithArrows arrowStyle redline" );
}
