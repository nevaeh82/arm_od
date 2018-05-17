#include "TestCaptionAndTooltip.h"
#include "ui_TestCaptionAndTooltip.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <PwGis/PwGisPointList.h>

TestCaptionAndTooltip::TestCaptionAndTooltip(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TestCaptionAndTooltip)
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnAddMarkers, SIGNAL( clicked() ), this, SLOT( onBtnAddMarkers() ) );
	connect( ui->btnMoveMarkers, SIGNAL( clicked() ), this, SLOT( onBtnMoveMarkers() ) );
	connect( ui->btnAddPath, SIGNAL( clicked() ), this, SLOT( onBtnAddPath() ) );

	QString mapFileName = QApplication::applicationDirPath() + QDir::separator()
		+ QString( "/../mapSamples/Ingit/Lenare/Lenare.chart" ).replace( "/", QDir::separator() );

	ui->pwgiswidget->mapProvider()->mapManager()->openMap( mapFileName );

	this->showMaximized();
}

TestCaptionAndTooltip::~TestCaptionAndTooltip()
{
	delete ui;
}

QString TestCaptionAndTooltip::getRuText( const QString &text )
{
	return QString::fromUtf8( text.toLatin1().data() );
}

void TestCaptionAndTooltip::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)")),30);
}

void TestCaptionAndTooltip::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void TestCaptionAndTooltip::onBtnAddMarkers()
{
	ui->pwgiswidget->addDragControls();

	ui->pwgiswidget->addMarker( "jsMarker3060", 30.190833, 60.068889, "355088843051235", "355088843051235", 0, "IriMarkerStyle selectAndDrag" );
	ui->pwgiswidget->addMarker( "jsMarker30560", 30.537778, 60.036389, "imei 123456789012345", "imei 123456789012345", 50, "IriMarkerStyle select" );
}

void TestCaptionAndTooltip::onBtnMoveMarkers()
{
	ui->pwgiswidget->moveFeature("jsMarker3060", 32.3394, 59.875 );
	ui->pwgiswidget->moveFeature("jsMarker30560", 31.0, 59.0 );
}

void TestCaptionAndTooltip::onBtnAddPath()
{
	PwGisPointList pathPoints;
	pathPoints.append( new PwGisLonLat( 29.283333, 59.933333 ) );
	pathPoints.append( new PwGisLonLat( 29.545833, 59.678056 ) );
	pathPoints.append( new PwGisLonLat( 30.182778, 59.524167 ) );
	pathPoints.append( new PwGisLonLat( 30.480833, 59.403333 ) );
	pathPoints.append( new PwGisLonLat( 31.165278, 59.581944 ) );
	ui->pwgiswidget->addPath( "jsPath3454", &pathPoints, getRuText("подпись.path"), getRuText("всплывающая подпись.path"), "redline selectAndDrag" );
}
