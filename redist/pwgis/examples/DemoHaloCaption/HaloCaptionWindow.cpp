#include "HaloCaptionWindow.h"
#include "ui_HaloCaptionWindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/PwGisStyle.h>


HaloCaptionWindow::HaloCaptionWindow( QWidget *parent ) :
	QWidget(parent),
	ui(new Ui::HaloCaptionWindow)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnAddFeatures, SIGNAL( clicked() ), this, SLOT( onBtnAddFeatures() ) );

	this->showMaximized();
}

HaloCaptionWindow::~HaloCaptionWindow()
{
	delete ui;
}

void HaloCaptionWindow::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr( "Open map" ), QApplication::applicationDirPath(), tr( "map files(*.*)" ) ), 30 );
}

void HaloCaptionWindow::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void HaloCaptionWindow::onBtnAddFeatures()
{
	this->addMarker();
	this->addPoint();
	this->addLine();
	this->addRectangle();
	this->addCircle();
	this->addPath();
	this->addPolygon();
	this->addClassicPolygon();
	this->addSlice();
}

void HaloCaptionWindow::addMarker()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloMarker" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapFontSize, "22px" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ff0000" );
	style->setProperty( PwGisStyle::mapLabelOutlineWidth, "15" );
	style->setProperty( PwGisStyle::mapLabelYOffset, "-29" );
	delete style->apply();

	ui->pwgiswidget->addMarker( "jsHaloMarker2959",  29.260896, 59.597736, "VBS_MiniDevice", "VBS_MiniDevice", 0, "customHaloMarker" );
}

void HaloCaptionWindow::addPoint()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloPoint" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapFontFamily, "times" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ff0000" );
	delete style->apply();

	ui->pwgiswidget->addPoint( "jsHaloPoint2959", 30.43, 59.25, "VBS_MiniDevice", "VBS_MiniDevice", "customHaloPoint" );
}

void HaloCaptionWindow::addLine()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloLine" );
	style->setProperty( PwGisStyle::mapFontColor, "#00ff00" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#0000ff" );
	style->setProperty( PwGisStyle::mapFontSize, "16px" );
	delete style->apply();

	ui->pwgiswidget->addLine( "jsHaloLine3060", 30.22, 60.93, 29.35, 60.42, "caption.line.halo", "tooltip.line.halo", "customHaloLine" );
}

void HaloCaptionWindow::addRectangle()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloRectangle" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ff00ff" );
	style->setProperty( PwGisStyle::mapFontWeight, "bold" );
	delete style->apply();

	ui->pwgiswidget->addRectangle( "jsHaloRectangle3160", 31.4, 60.47, 32.15, 60.85, "Caption.Rectangle.Halo", "tooltip.rectangle.halo", "customHaloRectangle" );
}

void HaloCaptionWindow::addCircle()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloCircle" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#00ffff" );
	delete style->apply();

	ui->pwgiswidget->addCircle( "jsHaloCircle3459", 31.7, 59.1, 50000, "caption.circle.halo", "tooltip.circle.halo", "customHaloCircle" );
}

void HaloCaptionWindow::addPath()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloPath" );
	style->setProperty( PwGisStyle::mapFontColor, "#00ffff" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ffff00" );
	delete style->apply();

	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 27.08, 59.9 ) );
	pathPoints.append( new PwGisLonLat( 28.38, 60.07 ) );
	pathPoints.append( new PwGisLonLat( 30.0, 59.88 ) );
	pathPoints.append( new PwGisLonLat( 32.32, 60.17 ) );
	ui->pwgiswidget->addPath( "jsHaloPath2759", &pathPoints, "caption.path.halo", "tooltip.path.halo", "customHaloPath" );
}

void HaloCaptionWindow::addPolygon()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloPolygon" );
	style->setProperty( PwGisStyle::mapFontColor, "#ff00ff" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapFontWeight, "bold" );
	delete style->apply();

	PwGisPointList polygonPoints;
	polygonPoints.append( new PwGisLonLat( 34.28, 60.65 ) );
	polygonPoints.append( new PwGisLonLat( 35.18, 60.72 ) );
	polygonPoints.append( new PwGisLonLat( 35.08, 60.34 ) );
	ui->pwgiswidget->addPolygon( "jsHaloPolygon3460", &polygonPoints, "caption.polygon.halo", "tooltip.polygon.halo","customHaloPolygon" );
}

void HaloCaptionWindow::addClassicPolygon()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloClassicPolygon" );
	style->setProperty( PwGisStyle::mapFontColor, "#ffff00" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#ff00ff" );
	delete style->apply();

	ui->pwgiswidget->addClassicPolygon( "jsHaloClassicPolygon3459", 34.28, 59.09, 50000, 7, 0, "caption.classicpolygon.halo", "tooltip.classicpolygon.halo", "customHaloClassicPolygon" );
}

void HaloCaptionWindow::addSlice()
{
	PwGisStyle* style = ui->pwgiswidget->createStyle( "customHaloSlice" );
	style->setProperty( PwGisStyle::mapFontColor, "#ff0000" );
	style->setProperty( PwGisStyle::mapLabelOutlineColor, "#00ffff" );
	delete style->apply();

	ui->pwgiswidget->addSlice( "jsHaloSlice3459", 34.1, 59.9, 50000, 10, 270, "caption.slice.halo", "tooltip.slice.halo", "customHaloSlice" );
}
