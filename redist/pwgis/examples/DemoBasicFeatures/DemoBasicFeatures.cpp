#include "DemoBasicFeatures.h"
#include "ui_DemoBasicFeatures.h"

#include <QApplication>
#include <QDir>
#include <PwGis/PwGisPointList.h>
#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>
#include <QFileDialog>
#include "ShowLog.h"


DemoBasicFeatures::DemoBasicFeatures(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoBasicFeatures)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL(clicked()), SLOT(onOpenMap()));
	connect( ui->btnCloseMap, SIGNAL(clicked()), SLOT(onCloseMap()));

	connect( ui->btnAddMarker, SIGNAL( clicked() ), this, SLOT( onBtnAddMarker() ) );
	connect( ui->btnAddPoint, SIGNAL( clicked() ), this, SLOT( onBtnAddPoint() ) );
	connect( ui->btnAddLine, SIGNAL( clicked() ), this, SLOT( onBtnAddLine() ) );
	connect( ui->btnAddRectangle, SIGNAL( clicked() ), this, SLOT( onBtnAddRectangle() ) );
	connect( ui->btnAddCircle, SIGNAL( clicked() ), this, SLOT( onBtnAddCircle() ) );
	connect( ui->btnAddPath, SIGNAL( clicked() ), this, SLOT( onBtnAddPath() ) );
	connect( ui->btnAddPolygon, SIGNAL( clicked() ), this, SLOT( onBtnAddPolygon() ) );
	connect( ui->btnAddClassicPolygon, SIGNAL( clicked() ), this, SLOT( onBtnAddClassicPolygon() ) );
	connect( ui->btnAddSlice, SIGNAL( clicked() ), this, SLOT( onBtnAddSlice() ) );
	connect( ui->btnGetGeometry, SIGNAL( clicked() ), this, SLOT( onBtnGetGeometry() ) );
	connect( ui->btnDeleteFeature, SIGNAL( clicked() ), this, SLOT( onBtnDeleteFeature() ) );
	connect( ui->btnDeleteAllFeatures, SIGNAL( clicked() ), this, SLOT( onBtnDeleteAllFeatures() ) );

	connect( ui->btnAddRing, SIGNAL( clicked() ), this, SLOT( onAddRing() ) );
	connect( ui->btnAddRingSegment, SIGNAL( clicked() ), this, SLOT( onAddRingSegment() ) );

	connect( &ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL( mapReady() ), SLOT( onMapReady() ) );

	installLogHandler(ui->txtLog);
	this->showMaximized();
}

DemoBasicFeatures::~DemoBasicFeatures()
{
	delete ui;
}

void DemoBasicFeatures::onMapReady()
{
	ui->txtLog->appendPlainText( "onMapReady" );
}

void DemoBasicFeatures::addMessageToLog( const QString &message, const QString &featureID )
{
	QString messageID = message;
	if ( !featureID.isEmpty() ) {
		ui->edtFeatureId->setText( featureID );
		messageID += ". id=`" + featureID + "`.";
	}
	ui->txtLog->appendPlainText(messageID );
}

void DemoBasicFeatures::onBtnAddMarker()
{
	ui->pwgiswidget->addMarker( "jsMarker260",  -26.0, 0.0, "подпись.marker", "tooltip.layer-drag.id=`jsMarker260`", 0, "PostPlaneRed selectAndDrag" );
	//ui->pwgiswidget->addMarker( "testJSBasicFeaturesMarker2610", -26.0, 10.0, "caption.m-blue", "tooltip.style-select", 50, "PostPlaneBlue select" );
	//ui->pwgiswidget->addMarker( "testJSBasicFeaturesMarker2620", -26.0, 20.0, "caption.m-yellow", "tooltip.m-yellow", 50, "PostPlaneYellow" );
	addMessageToLog( "added marker",  "jsMarker260" );
}

void DemoBasicFeatures::onBtnAddPoint()
{
	ui->pwgiswidget->addPoint( "jsPoint4816", -48.0, -16.0, "подпись.point", "tooltip.point.id=`jsPoint4816`", "red select" );
	addMessageToLog( "added point", "jsPoint4816" );
}

void DemoBasicFeatures::onBtnAddLine()
{
	ui->pwgiswidget->addLine( "jsLine7631", -76.0, 31.0, -7.0, 48.0, "подпись.line", "tooltip.redline.id=`jsLine7631`", "redline selectAndDrag" );
	//ui->pwgiswidget->addLine( "pwgisClientLine126677757", -126.0, 67.0, -77.0, 57.0, "caption.redline2", "tooltip.redline2", "redline select" );
	addMessageToLog( "added line", "jsLine7631" );
}

void DemoBasicFeatures::onBtnAddRectangle()
{
	ui->pwgiswidget->addRectangle( "jsRectangle12115", -121.0, -15.0, -86.0, 0.0, "подпись.rectangle", "tooltip.rectangle.id=`jsRectangle12115`", "green select" );
	addMessageToLog( "added rectangle", "jsRectangle12115" );
}

void DemoBasicFeatures::onBtnAddCircle()
{
	ui->pwgiswidget->addCircle( "jsCircle7334", 73.0, 34.0, 1000000.0, "подпись.circle", "tooltip.circle.id=`jsCircle7334`", "red selectAndDrag" );
	//ui->pwgiswidget->addCircle( "pwgisClientCircle3439", 30.0, 0.0, 1000000.0, "caption.circle", "tooltip.circle");
	addMessageToLog( "added circle", "jsCircle7334" );
}

void DemoBasicFeatures::onBtnAddPath()
{
	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 34.0, 44.0 ) );
	pathPoints.append( new PwGisLonLat( 37.0, 50.0 ) );
	pathPoints.append( new PwGisLonLat( 37.0, 55.0) );
	pathPoints.append( new PwGisLonLat( 60.0, 56.0) );
	pathPoints.append( new PwGisLonLat( 82.0, 54.0) );
	ui->pwgiswidget->addPath( "jsPath3454", &pathPoints, "подпись.path", "tooltip.path.id=`jsPath3454`", "redline selectAndDrag" );
	addMessageToLog( "added path", "jsPath3454" );
}

void DemoBasicFeatures::onBtnAddPolygon()
{
	PwGisPointList polygonPoints;
	polygonPoints.append( new PwGisLonLat( 67.0, -1.0 ) );
	polygonPoints.append( new PwGisLonLat( 82.0, 11.0 ) );
	polygonPoints.append( new PwGisLonLat( 91.0, -9.0) );
	ui->pwgiswidget->addPolygon( "jsPolygon679", &polygonPoints, "подпись.polygon", "tooltip.polygon.id=`jsPolygon679`", "blue selectAndDrag" );
	addMessageToLog( "added polygon", "jsPolygon679" );
}

void DemoBasicFeatures::onBtnAddClassicPolygon()
{
	ui->pwgiswidget->addClassicPolygon( "jsClassicPolygon1727", 17.0, 27.0, 700000.0, 7, 80.0, "подпись.classicPolygon", "tooltip.classicPolygon.id=`jsClassicPolygon1727`", "red selectAndDrag" );
	addMessageToLog( "added classic polygon", "jsClassicPolygon1727" );
}

void DemoBasicFeatures::onBtnAddSlice()
{
	ui->pwgiswidget->addSlice( "jsSlice2938", -29.0, -38.0, 700000.0, 45.0, 90.0, "подпись.slice", "tooltip.slice.id=`jsSlice2938`", "blue selectAndDrag" );
	addMessageToLog( "added slice", "jsSlice2938" );
}

void DemoBasicFeatures::onBtnGetGeometry()
{
	addMessageToLog( "get geometry. id=`" + ui->edtFeatureId->text() + "`.", "" );
	PwGisPointList* points = ui->pwgiswidget->getGeometry( ui->edtFeatureId->text() );
	for( int i = 0;  i < points->length(); ++i ) {
		addMessageToLog( QString("geometry.point[%1]: lon=%2, lat=%3").arg(i).arg(points->at(i)->lon).arg(points->at(i)->lat) );
	}
}

void DemoBasicFeatures::onBtnDeleteFeature()
{
	addMessageToLog( "delete feature. id=`" + ui->edtFeatureId->text() + "`.", "" );
	ui->pwgiswidget->removeObject( ui->edtFeatureId->text() );
	ui->edtFeatureId->setText( "" );
}

void DemoBasicFeatures::onBtnDeleteAllFeatures()
{
	addMessageToLog( "all delete feature" );
	ui->pwgiswidget->removeAllObjects();
	ui->edtFeatureId->setText( "" );
	ui->txtLog->clear();
}


void DemoBasicFeatures::onOpenMap()
{
	ui->txtLog->appendPlainText( "click button `open map`" );

	QString mapFile = QFileDialog::getOpenFileName( this,
		tr( "Open map" ), QApplication::applicationDirPath(),
		tr( "All files(*.*)" ) );

	ui->pwgiswidget->mapProvider()->mapManager()->openMap(mapFile, 30);
}

void DemoBasicFeatures::onCloseMap()
{
	ui->txtLog->appendPlainText( "click button `close map`" );

	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void DemoBasicFeatures::onAddRing()
{
	ui->pwgiswidget->addRing("ring", 30, 60,
							 30000, 60000, 0, 360,
							 "ring caption", "ring tooltip", "");

	addMessageToLog( "added ring", "ring" );
}

void DemoBasicFeatures::onAddRingSegment()
{
	ui->pwgiswidget->addRing("ringSegment", 30, 60,
							 30000, 60000, 45, 90,
							 "segment caption", "segment \"tooltip\" ", "");

	addMessageToLog( "added ring segment", "ringSegment" );
}
