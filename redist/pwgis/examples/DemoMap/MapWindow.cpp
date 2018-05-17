#include "MapWindow.h"
#include "ui_MapWindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>
#include <PwGis/MapManagerEvents.h>


MapWindow::MapWindow( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::MapWindow ),
	_trackCounter( 0 )
{
	ui->setupUi( this );
	ui->radioButtonRU->setChecked( true );
	_translator = new QTranslator( this );

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnOpenAtlas, SIGNAL( clicked() ), this, SLOT( onBtnOpenAtlas() ) );
	connect( ui->btnCloseAtlas, SIGNAL( clicked() ), this, SLOT( onBtnCloseAtlas() ) );
	connect( ui->btnAddTrack, SIGNAL( clicked() ), this, SLOT( onBtnAddTrack() ) );
	connect( ui->btnClearLog, SIGNAL( clicked() ), this, SLOT( onBtnClearLog() ) );

	connect( &ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL( mapReady() ),
		this, SLOT( onMapReady() ) );

	connect( &ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL( atlasReady() ),
		this, SLOT( onAtlasReady() ) );

	connect( ui->radioButtonRU, SIGNAL( clicked() ),
		this, SLOT( clickedRadioButton() ) );
	connect( ui->radioButtonEN, SIGNAL( clicked() ),
		this, SLOT( clickedRadioButton() ) );
	connect( ui->radioButtonDE, SIGNAL( clicked() ),
		this, SLOT( clickedRadioButton() ) );
	connect( ui->checkBoxBaseLayerSwitcher, SIGNAL( stateChanged( int ) ),
			 this, SLOT( baseLayerSwitcherChanged( int ) ) );

	this->showMaximized();

	emit ui->radioButtonRU->click();
}

MapWindow::~MapWindow()
{
	delete _translator;
	delete ui;
}

void MapWindow::onMapReady()
{
	ui->txtLog->appendPlainText( tr( "onMapReady" ) );
}

void MapWindow::onAtlasReady()
{
	ui->txtLog->appendPlainText( tr( "onAtlasReady" ) );
}

void MapWindow::onBtnClearLog()
{
	ui->txtLog->clear();
}

void MapWindow::onBtnOpenMap()
{
	ui->txtLog->appendPlainText( tr( "click button `open map`" ) );

	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
			this,
			tr( "Open map" ),
			QApplication::applicationDirPath(),
			tr( "Map files(*.*)" ) ),
		30 );
}

void MapWindow::onBtnCloseMap()
{
	ui->txtLog->appendPlainText( tr( "click button `close map`" ) );

	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void MapWindow::onBtnOpenAtlas()
{
	ui->txtLog->appendPlainText( tr( "click button `open atlas`" ) );

	ui->pwgiswidget->mapProvider()->mapManager()->openAtlas();
}

void MapWindow::onBtnCloseAtlas()
{
	ui->txtLog->appendPlainText( tr( "click button `close atlas`" ) );

	ui->pwgiswidget->mapProvider()->mapManager()->closeAtlas();
}

void MapWindow::onBtnAddTrack()
{
	ui->txtLog->appendPlainText( tr( "click button `add track`" ) );

	QString gpxFileName = QFileDialog::getOpenFileName(
		this,
		tr( "Open gpx" ),
		QApplication::applicationDirPath(),
		tr( "GPX files(*.gpx)" ) );

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

void MapWindow::clickedRadioButton()
{
	qApp->removeTranslator( _translator );

	QLocale::Language localeLanguage;
	if ( ui->radioButtonRU->isChecked() ) {
		_translator->load( ":/DemoMap_ru.qm" );
		localeLanguage = QLocale::Russian;
	}
	else if ( ui->radioButtonEN->isChecked() ) {
		_translator->load( ":/DemoMap_en.qm" );
		localeLanguage = QLocale::English;
	}
	else {
		_translator->load( ":/DemoMap_de.qm" );
		localeLanguage = QLocale::German;
	}

	qApp->installTranslator( _translator );

	this->retranslateStrings();

	QLocale gisLocale( localeLanguage );
	ui->pwgiswidget->setLocale( gisLocale );
}

void MapWindow::baseLayerSwitcherChanged( int state )
{
	if ( state == 0 ) {
		ui->pwgiswidget->showBaseLayerSwitcher( false );
	}
	else {
		ui->pwgiswidget->showBaseLayerSwitcher( true );
	}
}

void MapWindow::retranslateStrings()
{
	ui->btnOpenMap->setText( QObject::tr( "open map" ) );
	ui->btnCloseMap->setText( QObject::tr( "close map" ) );
	ui->btnAddTrack->setText( QObject::tr( "add track" ) );
	ui->btnClearLog->setText( QObject::tr( "clear log" ) );
	this->setWindowTitle( QObject::tr( "Demo map" ) );
	ui->checkBoxBaseLayerSwitcher->setText( QObject::tr( "base layer switcher" ) );
	ui->btnOpenAtlas->setText( QObject::tr( "open atlas" ) );
	ui->btnCloseAtlas->setText( QObject::tr( "close atlas" ) );
}
