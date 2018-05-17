#include "DemoUpdateFeatureParameters.h"
#include "ui_DemoUpdateFeatureParameters.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <PwGis/PwGisPointList.h>

DemoUpdateFeatureParameters::DemoUpdateFeatureParameters(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoUpdateFeatureParameters),
	_counterLine( 0 )
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnAddPolygone, SIGNAL( clicked() ), this, SLOT( onBtnAddPolygone() ) );
	connect( ui->btnAddStylePolygone, SIGNAL( clicked() ), this, SLOT( onBtnAddStylePolygone() ) );
	connect( ui->btnOpenFile, SIGNAL( clicked() ), this, SLOT( onBtnOpenFile() ) );
	connect( ui->btnReadLine, SIGNAL( clicked() ), this, SLOT( onBtnReadLine() ) );

	QString mapFileName = QApplication::applicationDirPath() + QDir::separator()
		+ QString( "/../mapSamples/Ingit/Lenare/Lenare.chart" ).replace( "/", QDir::separator() );

	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapFileName );

	this->showMaximized();
	ui->btnReadLine->setEnabled( false );
}

DemoUpdateFeatureParameters::~DemoUpdateFeatureParameters()
{
	delete ui;
}

QString DemoUpdateFeatureParameters::getRuText( const QString &text )
{
	return QString::fromUtf8( text.toLatin1().data() );
}

void DemoUpdateFeatureParameters::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)")),30);
}

void DemoUpdateFeatureParameters::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void DemoUpdateFeatureParameters::onBtnAddPolygone()
{
	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 27.0, 59.0 ) );
	pathPoints.append( new PwGisLonLat( 29.0, 59.0 ) );
	pathPoints.append( new PwGisLonLat( 29.0, 61.0 ) );
	pathPoints.append( new PwGisLonLat( 27.0, 61.0) );

	ui->pwgiswidget->addPolygon(
				"idTestPolygon2759", &pathPoints, "path.27.59", getRuText( "всплывающая подпись.path.27.59" ), "" );
}

void DemoUpdateFeatureParameters::onBtnAddStylePolygone()
{
	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 30.0, 60.0 ) );
	pathPoints.append( new PwGisLonLat( 32.0, 60.0 ) );
	pathPoints.append( new PwGisLonLat( 32.0, 62.0 ) );
	pathPoints.append( new PwGisLonLat( 30.0, 62.0) );

	ui->pwgiswidget->addPolygon( "idTestPolygon2759", &pathPoints, "path.30.60", getRuText( "всплывающая подпись.path.30.60" ), "red" );
}

void DemoUpdateFeatureParameters::onBtnOpenFile()
{
	ui->btnOpenFile->setEnabled( false );

	QString logFileName = QFileDialog::getOpenFileName(this,
		tr( "Open log" ), QApplication::applicationDirPath(),tr( "All files(*.*)" ) );
	//QString logFileName = QApplication::applicationDirPath() +
	//	QString( "/../../../polygon-log.log" );

	_logFile.setFileName( logFileName );
	if ( !_logFile.open( QIODevice::ReadOnly ) ) {
		ui->edtLogLine->setText( "file is not opened" );
		return;
	}
	_logStream.setDevice( &_logFile );
	ui->edtLogLine->setText( "file is opened" );
	ui->btnReadLine->setEnabled( true );
}

void DemoUpdateFeatureParameters::onBtnReadLine()
{
	if ( _logStream.atEnd() ) {
		ui->btnReadLine->setEnabled( false );
		ui->edtLogLine->setText( "eof" );
		_logFile.close();
		return;
	}

	QString lineLog = _logStream.readLine();
	//ui->edtLogLine->setText( lineLog );

	executeLogLine( lineLog );
}

void DemoUpdateFeatureParameters::executeLogLine( const QString &logLine )
{
	_counterLine++;
	QString commandType = QString( "%1." ).arg( _counterLine );


	if ( logLine.indexOf( "commitTransaction" ) != -1 ) {
		commandType += "commitTransaction";
		commitTransactionFromLog();
	}
	else if ( logLine.indexOf( "removeAllObjects" ) != -1 ) {
		commandType += "removeAllObjects";
		removeAllObjectsFromLog();
	}
	else if ( logLine.indexOf( "addMarker" ) != -1 ) {
		commandType += "addMarker";
		addMarkerFromLog( logLine );
	}
	else if ( logLine.indexOf( "removeMarker" ) != -1 ) {
		commandType += "removeMarker";
		removeMarkerFromLog( logLine );
	}
	else if ( logLine.indexOf( "removeFeature" ) != -1 ) {
		commandType += "removeFeature";
		removeFeatureFromLog( logLine );
	}
	else if ( logLine.indexOf( "addPolygon" ) != -1 ) {
		commandType += "addPolygon";
		addPolygonFromLog( logLine );
	}

	ui->edtLogLine->setText( commandType + ": " + logLine );
}

QString DemoUpdateFeatureParameters::getTextBetweenQuotes( const QString &text )
{
	int pos1quote = text.indexOf( "\"" );
	int pos2quote = text.indexOf( "\"", pos1quote + 1 );
	if (( pos1quote == -1 ) || ( pos2quote <= pos1quote ) ) {
		return "";
	}
	return text.mid( pos1quote + 1, pos2quote - pos1quote - 1 );
}

void DemoUpdateFeatureParameters::addMarkerFromLog( const QString &logLine )
{
	QStringList fieldList = logLine.split(",");
	QString id = getTextBetweenQuotes( fieldList[0] );
	double lon = QString(fieldList[1]).toDouble();
	double lat = QString(fieldList[2]).toDouble();
	QString caption = getTextBetweenQuotes( fieldList[3] );
	QString tooltip = getTextBetweenQuotes( fieldList[4] );
	double rotateAngle = QString(fieldList[5]).toDouble();
	QString style = getTextBetweenQuotes( fieldList[6] );

	ui->pwgiswidget->addMarker( id, lon, lat, caption, tooltip, rotateAngle, style );
}

void DemoUpdateFeatureParameters::addPolygonFromLog( const QString &logLine )
{
	QStringList fieldList = logLine.split(",");
	QString id = getTextBetweenQuotes( fieldList[0] );

	PwGisPointList pathPoints;
	int coordCount = ( fieldList.count() - 1 - 3 ) / 2;
	double lon;
	double lat;
	QStringList tempList;
	QString tempStr;
	for ( int i = 0; i < coordCount;  ++i) {
		//lon
		int j = i*2 + 1;
		tempList = fieldList[j].split(":");
		lon = (tempList[1].trimmed()).toDouble();
		//lat
		j++;
		tempList = fieldList[j].split(":");
		tempStr = tempList[1];
		tempList = tempStr.split("}");
		lat = (tempList[0].trimmed()).toDouble();

		pathPoints.append( new PwGisLonLat( lon, lat ) );
	}
	QString caption = getTextBetweenQuotes( fieldList[fieldList.count() - 3] );
	QString tooltip = getTextBetweenQuotes( fieldList[fieldList.count() - 2] );
	QString style = getTextBetweenQuotes( fieldList[fieldList.count() - 1] );

	ui->pwgiswidget->addPolygon( id, &pathPoints, caption, tooltip, style );
}

void DemoUpdateFeatureParameters::removeMarkerFromLog( const QString &logLine )
{
	QString id = getTextBetweenQuotes( logLine );
	ui->pwgiswidget->removeMarker( id );
}

void DemoUpdateFeatureParameters::removeFeatureFromLog( const QString &logLine )
{
	QString id = getTextBetweenQuotes( logLine );
	ui->pwgiswidget->removeObject( id );
}

void DemoUpdateFeatureParameters::removeAllObjectsFromLog()
{
	ui->pwgiswidget->removeAllObjects();
}

void DemoUpdateFeatureParameters::commitTransactionFromLog()
{
	ui->pwgiswidget->commitTransaction();
}
