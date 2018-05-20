#include "DemoDirectionPath.h"
#include "ui_DemoDirectionPath.h"

#include <MapProvider.h>
#include <PwGis/IMapManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/IObjectsRegistry.h>
#include <PwGis/MapManagerEvents.h>

#include <QFileDialog>

DemoDirectionPath::DemoDirectionPath(QWidget *parent) :
	QWidget(parent)
	, _styleArrow("")
	, _currentStyle("")
	, ui(new Ui::DemoDirectionPath)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );

	connect( ui->btnAddPath, SIGNAL( clicked() ), this, SLOT( onBtnAddPath() ) );
	connect( ui->btnAppendPath, SIGNAL( clicked() ), this, SLOT( onBtnAppendPath() ) );
	connect( ui->btnRemovePath, SIGNAL( clicked() ), this, SLOT( onBtnRemovePath() ) );

	connect( ui->btnWindRose, SIGNAL( clicked() ), this, SLOT( onBtnAddWindRose() ) );
	connect( ui->btnRemoveWindRose, SIGNAL( clicked() ), this, SLOT( onBtnRemoveWindRose() ) );

	connect( ui->btnArrowStyle, SIGNAL( clicked() ), this, SLOT( onBtnArrowStyle() ) );
	connect( ui->btnSimpleStyle, SIGNAL( clicked() ), this, SLOT( onBtnSimpleStyle() ) );

	connect( &ui->pwgiswidget->mapProvider()->mapManager()->events(), SIGNAL( mapReady() ), SLOT( onMapReady() ) );
}

DemoDirectionPath::~DemoDirectionPath()
{
	delete ui;
}

void DemoDirectionPath::onBtnOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName( this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)") );
	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapFile, 30 );
}

void DemoDirectionPath::onMapReady()
{
	_pathStyle = ui->pwgiswidget->createStyle( "styleArrow" );
	_pathStyle->setProperty( PwGisStyle::strokeColor, "#FF0000" );
	_pathStyle->setProperty( PwGisStyle::strokeWidth, "5" );
	_pathStyle->setProperty( PwGisStyle::cursor, "pointer" );
	_pathStyle->setProperty( PwGisStyle::pathArrows, "true" );

	_styleArrow = "styleArrow";

	_windRoseStyle = ui->pwgiswidget->createStyle( "styleWindRose" );
	_windRoseStyle->setProperty( PwGisStyle::strokeColor, "#0000FF" );
	_windRoseStyle->setProperty( PwGisStyle::strokeWidth, "1" );
	_windRoseStyle->setProperty( PwGisStyle::pathArrows, "true" );
	_windRoseStyle->setProperty( PwGisStyle::heightArrow, "50" );
	_windRoseStyle->setProperty( PwGisStyle::widthArrows, "10" );
	_windRoseStyle->setProperty( PwGisStyle::minDistance, "100" );

	_pathStyle->apply();
	_windRoseStyle->apply();
}

void DemoDirectionPath::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

// ---------------- ADDING PATH DRAWING WINDROSE --------------------------------
void DemoDirectionPath::onBtnAddWindRose()
{
	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 37.61771, 55.74931 ) );
	pathPoints.append( new PwGisLonLat( 37.61771, 55.89896 ) );
	ui->pwgiswidget->addPath( "jsPathN", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.76877, 55.86122 ) );
	ui->pwgiswidget->addPath( "jsPathNE", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.8443, 55.74931 ) );
	ui->pwgiswidget->addPath( "jsPathE", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.79898, 55.63087 ) );
	ui->pwgiswidget->addPath( "jsPathSE", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.61771, 55.58278 ) );
	ui->pwgiswidget->addPath( "jsPathS", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.45429, 55.6456 ) );
	ui->pwgiswidget->addPath( "jsPathSW", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.37738, 55.74931 ) );
	ui->pwgiswidget->addPath( "jsPathW", &pathPoints, "", "", "styleWindRose" );

	pathPoints.removeLast();
	pathPoints.append( new PwGisLonLat( 37.42132, 55.87586 ) );
	ui->pwgiswidget->addPath( "jsPathNW", &pathPoints, "", "", "styleWindRose" );

	pathPoints.clear();

	ui->pwgiswidget->mapProvider()->mapBounds()->zoomMapTo(36, 54, 38, 56);
}

void DemoDirectionPath::onBtnRemoveWindRose()
{
	ui->pwgiswidget->removeObject( "jsPathN" );
	ui->pwgiswidget->removeObject( "jsPathNE" );
	ui->pwgiswidget->removeObject( "jsPathE" );
	ui->pwgiswidget->removeObject( "jsPathSE" );
	ui->pwgiswidget->removeObject( "jsPathS" );
	ui->pwgiswidget->removeObject( "jsPathSW" );
	ui->pwgiswidget->removeObject( "jsPathW" );
	ui->pwgiswidget->removeObject( "jsPathNW" );
}

void DemoDirectionPath::onBtnAddPath()
{
	_trackPoints.clear();
	_currentStyle.clear();
	_trackPoints.append( new PwGisLonLat( 37.511683, 55.809407 ) );
	_trackPoints.append( new PwGisLonLat( 37.511278, 55.809118 ) );
	_trackPoints.append( new PwGisLonLat( 37.511278, 55.809118 ) );
	_trackPoints.append( new PwGisLonLat( 37.51116,  55.809165 ) );
	_trackPoints.append( new PwGisLonLat( 37.511455, 55.8091 ) );
	_trackPoints.append( new PwGisLonLat( 37.51162,  55.809035 ) );
	_trackPoints.append( new PwGisLonLat( 37.511338, 55.809082 ) );
	_trackPoints.append( new PwGisLonLat( 37.511542, 55.809008 ) );
	_trackPoints.append( new PwGisLonLat( 37.515233, 55.808973 ) );
	_trackPoints.append( new PwGisLonLat( 37.526625, 55.805873 ) );
	_trackPoints.append( new PwGisLonLat( 37.537427, 55.802763 ) );
	_trackPoints.append( new PwGisLonLat( 37.542943, 55.800493 ) );
	_trackPoints.append( new PwGisLonLat( 37.549167, 55.797288 ) );
	_trackPoints.append( new PwGisLonLat( 37.555188, 55.794228 ) );
	_trackPoints.append( new PwGisLonLat( 37.569133, 55.791187 ) );
	_trackPoints.append( new PwGisLonLat( 37.580620, 55.791798 ) );
	_trackPoints.append( new PwGisLonLat( 37.589210, 55.792107 ) );
	_trackPoints.append( new PwGisLonLat( 37.599348, 55.792388 ) );
	_trackPoints.append( new PwGisLonLat( 37.622273, 55.792753 ) );
	_trackPoints.append( new PwGisLonLat( 37.634743, 55.792053 ) );
	_trackPoints.append( new PwGisLonLat( 37.638882, 55.793172 ) );
	_trackPoints.append( new PwGisLonLat( 37.641997, 55.783798 ) );
	_trackPoints.append( new PwGisLonLat( 37.649580, 55.776225 ) );
	_trackPoints.append( new PwGisLonLat( 37.652748, 55.773462 ) );
	_trackPoints.append( new PwGisLonLat( 37.652482, 55.772788 ) );

	ui->pwgiswidget->addPath( "jsSokolKomsomolskajaTrack", &_trackPoints, "", "", "" );

	ui->pwgiswidget->mapProvider()->mapBounds()->zoomMapTo(36, 54, 38, 56);
}

void DemoDirectionPath::onBtnAppendPath()
{
	if(_trackPoints.length() == 0) {
		return;
	}

	_trackPoints.append( new PwGisLonLat( getRandomLon(), getRandomLat() ) );

	ui->pwgiswidget->addPath( "jsSokolKomsomolskajaTrack", &_trackPoints, "", "", _currentStyle );
}

void DemoDirectionPath::onBtnArrowStyle()
{
	_currentStyle = _styleArrow;
	ui->pwgiswidget->addPath( "jsSokolKomsomolskajaTrack", &_trackPoints, "", "", _styleArrow );
}

void DemoDirectionPath::onBtnSimpleStyle()
{
	_currentStyle.clear();
	ui->pwgiswidget->addPath( "jsSokolKomsomolskajaTrack", &_trackPoints, "", "", "" );
}

void DemoDirectionPath::onBtnRemovePath()
{
	ui->pwgiswidget->removeObject( "jsSokolKomsomolskajaTrack" );
	_trackPoints.clear();
	_currentStyle.clear();
}

// --------------------------------------------------------------------------------------

double DemoDirectionPath::getRandomLon()
{
	double width = 0.2;
	double rnd = (double)rand()/100;
	while(rnd > width) rnd -= width;
	return rnd + 37.511683;
}

double DemoDirectionPath::getRandomLat()
{
	double height = 0.1;
	double rnd = (double)rand()/100;
	while(rnd > height) rnd -= height;
	return rnd + 55.809407;
}

