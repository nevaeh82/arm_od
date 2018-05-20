#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Options.h"

#include <QMap>
#include <QString>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QMutexLocker>
#include <QPlainTextEdit>
#include <QApplication>
#include <QDir>
#include <QTime>
#include <QProcess>
#include <QMessageBox>
#include <QTextCodec>

#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/PwGisPointList.h>
#include <PwGis/MapFormatInfo.h>
#include <PwGis/MapFormatsCollection.h>
#include <IMapBounds.h>
#include <IMapManager.h>

#include "TestFormats.h"
#include "TestCaptionAndTooltip.h"
#include "DemoUpdateFeatureParameters.h"

QMutex mutex;
QPlainTextEdit* g_textEdit;

#if QT_VERSION < 0x050000
void myMessageOutput(QtMsgType type, const char *msg)
#else
#define qInstallMsgHandler qInstallMessageHandler

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
#endif
{
	QMutexLocker locker( &mutex);

	if ( !g_textEdit ) {
		return;
	}

	switch (type) {
	case QtDebugMsg:
		g_textEdit->appendPlainText(QString("[%1] Debug:  %2").arg( (int)QThread::currentThreadId()).arg( msg));
		break;
	case QtWarningMsg:
		g_textEdit->appendPlainText(QString("[%1] Warning: %2").arg((int)QThread::currentThreadId()).arg( msg));
		break;
	case QtCriticalMsg:
		g_textEdit->appendPlainText(QString("[%1] Critical: %2").arg( (int)QThread::currentThreadId()).arg( msg));
		break;
	case QtFatalMsg:
		g_textEdit->appendPlainText(QString("[%1] Fatal: %2").arg( (int)QThread::currentThreadId()).arg( msg));
		abort();
	}
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	qsrand(QTime::currentTime().second());
	ui->setupUi(this);
	g_textEdit = ui->plainTextEdit;

	qInstallMsgHandler(myMessageOutput);

	connect(ui->btnInit,SIGNAL(clicked()), this, SLOT(loadMap()));

	connect(ui->btnTestMapFormats,SIGNAL(clicked()),this,SLOT(testMapFormats()));
	connect( ui->btnTestCaptionAndTooltip, SIGNAL( clicked() ), this, SLOT( testCaptionAndTooltip() ) );
	connect( ui->btnDemoUpdateFeatureParameters, SIGNAL( clicked() ), this, SLOT( demoUpdateFeatureParameters() ) );

	_mapManager = ui->gisWidget->mapProvider()->mapManager();
	_mapBounds = ui->gisWidget->mapProvider()->mapBounds();

#ifdef __USE_ALL__

	connect(ui->btnKmlWorldMap, SIGNAL(clicked()), this, SLOT(loadKmlWorldMap()));
	connect(&_mapManager->events(),SIGNAL(loadProgress(int)),ui->progressBar, SLOT(setValue(int)));
	connect(&_mapManager->events(), SIGNAL(mapReady()), this, SLOT(onMapReady()));
	connect(ui->btnReload, SIGNAL(clicked()), this, SLOT(loadMap()));
	connect(ui->btnSetMarker, SIGNAL(clicked()), this, SLOT(onSetMarker()));
	connect(ui->btnDeleteMarker, SIGNAL(clicked()), this, SLOT(onRemoveMarker()));
	connect(ui->btnTooltipsAndCaptions, SIGNAL(clicked()), this, SLOT(tooltipsAndCaptions()));

	connect(ui->btnDrawCircle, SIGNAL(clicked()), this, SLOT(drawCircle()));
	connect(ui->btnCenterLat89, SIGNAL(clicked()), this, SLOT(centerLat89()));
	connect(ui->btnCenterLonM179, SIGNAL(clicked()), this, SLOT(centerLonM179()));
	connect(ui->btnAddSlice, SIGNAL(clicked()), this, SLOT(addSlice()));
	connect(ui->btnRemoveSlice, SIGNAL(clicked()), this, SLOT(removeSlice()));
	connect(ui->btnAdd2Markers, SIGNAL(clicked()), this, SLOT(add2Markers()));
	connect(ui->btnRemove2Markers, SIGNAL(clicked()), this, SLOT(remove2Markers()));
	connect(ui->btnAddMarker, SIGNAL(clicked()), this, SLOT(addMarker()));
	connect(ui->btnUpdateMarker, SIGNAL(clicked()), this, SLOT(updateMarker()));
	connect(ui->btnAdd4Objects, SIGNAL(clicked()), this, SLOT(add4Objects()));
	connect(ui->btnMove4Objects, SIGNAL(clicked()), this, SLOT(move4Objects()));
	connect(ui->btnDrawLongLine, SIGNAL(clicked()), this, SLOT(drawLongLine()));
	connect(ui->btnDrawLongRectangle, SIGNAL(clicked()), this, SLOT(drawLongRectangle()));
	connect(ui->btnDrawBrokenCircle, SIGNAL(clicked()), this, SLOT(drawBrokenCircle()));
	connect(ui->btnAddForkedObjects, SIGNAL(clicked()), this, SLOT(addForkedObjects()));
	connect(ui->btnRemoveForkedObjects, SIGNAL(clicked()), this, SLOT(removeForkedObjects()));

	connect(ui->btnSetCustomStyle, SIGNAL(clicked()), this, SLOT(setCustomStyle()));
	connect(ui->btnGetFeatureProperties, SIGNAL(clicked()), this, SLOT(onGetFeatureProperties()));

	ui->gisWidget->enableDebugger(true && !Options::testJs);
	_mapFile = QApplication::applicationDirPath() + QDir::separator() + "/../MapSamples/Ingit/Abakan/Abakan.chart";

	//this->loadMap();

	//this->loadMap();
#endif

}

MainWindow::~MainWindow()
{
	g_textEdit = NULL;
	delete ui;
}

void MainWindow::testMapFormats()
{
	showTest<TestFormats>();
}

void MainWindow::testCaptionAndTooltip()
{
	showTest<TestCaptionAndTooltip>();
}

void MainWindow::demoUpdateFeatureParameters()
{
	showTest<DemoUpdateFeatureParameters>();
}


template<typename T>
void MainWindow::showTest()
{
	T* widget = new T();
	connect(widget, SIGNAL(finished(int)), widget, SLOT(deleteLater()));
	widget->show();
}

void MainWindow::tooltipsAndCaptions()
{
	PwGisWidget *gis = ui->gisWidget;

	gis->removeObject( "caption" );
	gis->removeMarker( "tooltip" );
	gis->addCircle( "caption", 91.26, 53.63, 1000, "caption", "", "" );
	gis->addMarker( "tooltip", 91.30, 53.63, "", "tooltip", 0, "" );
}


QString MainWindow::getFileNameJs()
{
	QString appPath = QApplication::applicationDirPath();

	//copy gpx to wwwroot
	QString gpxSource = appPath + QDir::separator()
			+ QString("../mapSamples/GpxTracks/Socol.RZD.2011.10.31.gpx").replace("/", QDir::separator());
	QDir().mkpath(appPath + "/wwwroot/tracks");
	QFile::copy(gpxSource, appPath + "/wwwroot/tracks/Socol.RZD.2011.10.31.gpx");

	return appPath + QDir::separator()
			+ QString("../mapSamples/GpxTracks/testGpx.js").replace("/", QDir::separator());
}

QString MainWindow::getFileNameRzdJs()
{
	QString appPath = QApplication::applicationDirPath();

	//copy gpx to wwwroot
	QString gpxSource = appPath + QDir::separator()
			+ QString("../mapSamples/GpxTracks/Rzd.2010.08.16.gpx").replace("/", QDir::separator());
	QDir().mkpath(appPath + "/wwwroot/tracks");
	QFile::copy(gpxSource, appPath + "/wwwroot/tracks/Rzd.2010.08.16.gpx");

	return appPath + QDir::separator()
			+ QString("../mapSamples/GpxTracks/testRzdGpx.js").replace("/", QDir::separator());
}

QString MainWindow::readJs(const QString& fileNameJs)
{
	QFile file(fileNameJs);
	file.open(QIODevice::ReadOnly);
	return QTextStream(&file).readAll();
}

void MainWindow::loadGpxTrack()
{
	QString nameJs = getFileNameJs();
	QString textJs = readJs(nameJs);

	this->ui->gisWidget->executeScript( textJs );
}


void MainWindow::loadRzdTrack()
{
	QString nameJs = getFileNameRzdJs();
	QString textJs = readJs(nameJs);

	this->ui->gisWidget->executeScript( textJs );
}

void MainWindow::drawCircle()
{
	this->ui->gisWidget->addCircle( "smoothedCircleJsTest0", 0.0, 0.0, 125000, "", "", "" );
	this->ui->gisWidget->addCircle( "smoothedCircleJsTest1", 0.0, 7.0, 250000, "", "", "" );
	this->ui->gisWidget->addCircle( "smoothedCircleJsTest2", 0.0, 21.0, 500000, "", "", "" );
	this->ui->gisWidget->addCircle( "smoothedCircleJsTest3", 0.0, 50.0, 1000000, "", "", "" );
}

void MainWindow::centerLat89()
{
	_mapBounds->setMapCenter( 0, 89 );
}

void MainWindow::centerLonM179()
{
	_mapBounds->setMapCenter( -179.0, 68.0 );
}

void MainWindow::addSlice()
{
	this->ui->gisWidget->addSlice( "testSlice489", 92.5, 72.4, 500000, 1, 50, "slice 489", "", "" );
}

void MainWindow::removeSlice()
{
	this->ui->gisWidget->removeObject( "testSlice489"  );
}

void MainWindow::add2Markers()
{
	this->ui->gisWidget->addMarker( "testRemovedMarker00", 89.5, 77.1, "", "m-red", 0, "PostPlaneRed");
	this->ui->gisWidget->addMarker( "testRemovedMarker11", 96.4, 77.0, "", "m-blue", 50, "PostPlaneBlue" );
}

void MainWindow::remove2Markers()
{
	this->ui->gisWidget->removeMarker( "testRemovedMarker00" );
	this->ui->gisWidget->removeMarker( "testRemovedMarker11" );
}

void MainWindow::addMarker()
{
	this->ui->gisWidget->addMarker( "testMarker125", 94.7, 77.5, "", "m-red", 0, "PostPlaneRed" );
}

void MainWindow::updateMarker()
{
	this->ui->gisWidget->addMarker( "testMarker125", 97.3, 77.5, "", "m-blue", 50, "PostPlaneBlue" );
}

void MainWindow::add4Objects()
{
	this->ui->gisWidget->addPoint( "relocatableObject1", 37.6, 55.4, "Moskva", "", "red" );
	this->ui->gisWidget->addPoint( "relocatableObject2", 35.9, 56.5, "", "", "" );
	this->ui->gisWidget->addMarker( "relocatableObject3", 37.6, 55.4, "", "Moskva-marker", 0, "PostPlaneRed" );
	this->ui->gisWidget->addMarker( "relocatableObject4",  35.9, 56.5, "", "", 0, "PostPlaneBlue" );
}

void MainWindow::move4Objects()
{
	this->ui->gisWidget->moveFeature( "relocatableObject1", 39.9, 57.4 );
	this->ui->gisWidget->moveFeature( "relocatableObject2", 39.9, 58.9 );
	this->ui->gisWidget->moveFeature( "relocatableObject3", 34.3, 53.0 );
	this->ui->gisWidget->moveFeature( "relocatableObject4", 32.0, 54.5 );
}

void MainWindow::drawLongLine()
{
	this->ui->gisWidget->addLine( "horizontalBearing1", 31.0, 58.0, 390.0, 58.0, "long", "line", "" );
	this->ui->gisWidget->addLine( "verticalBearing1", 28.0, 60.0, 28.0, 84.0, "", "", "redline" );
	this->ui->gisWidget->addLine( "verticalBearing2", 29.0, 60.0, 29.0, 90.0, "", "", "redline" );
	this->ui->gisWidget->addLine( "verticalBearing3", 30.0, 60.0, 30.0, 419.0, "", "", "redline" );
	this->ui->gisWidget->addLine( "verticalBearing4", 31.0, 60.0, 31.0, -299.0, "", "", "redline" );
	this->ui->gisWidget->addLine( "slopingBearing5", 32.0, 60.0, 122.0, 84.0, "", "", "");
	this->ui->gisWidget->addLine( "longBearing6", 34.0, 60.0, 384.0, 50.0, "", "", "redline" );
}

void MainWindow::drawLongRectangle()
{
	this->ui->gisWidget->addRectangle( "longRect210", 150.0, 79.7, 210.0, 80.0, "", "", "" );
	this->ui->gisWidget->addRectangle( "longRect91", 137.0, 81.0, 138.0, 91.0, "", "", "" );
}

void MainWindow::drawBrokenCircle()
{
	this->ui->gisWidget->addCircle( "brokenCircle1", 181.0, 79.5, 1000000, "", "", "" );
}

void MainWindow::addForkedObjects()
{
	this->ui->gisWidget->addLine( "testForkedLine", 20.0, 25.0, 220.0, 25.0, "forked", "", "" );
	this->ui->gisWidget->addRectangle( "testForkedRect", 20.0, 37.0, 230.0, 42.0, "test forked rectangle", "", "" );
	this->ui->gisWidget->addCircle( "testForkedCircle", 179.0, 65.0, 500000, "test forked circle", "", "" );
}

void MainWindow::removeForkedObjects()
{
	this->ui->gisWidget->removeObject( "testForkedLine" );
	this->ui->gisWidget->removeObject( "testForkedRect" );
	this->ui->gisWidget->removeObject( "testForkedCircle" );
}

void MainWindow::loadMap()
{
	_mapFile = ui->gisWidget->mapProvider()->mapManager()->openMapDialog();
	if ( _mapFile.isEmpty() ) return;

	//this->ui->gisWidget->setMapId("s/1");
	_mapManager->openMap(_mapFile.replace("/", QString( QDir::separator() )));
}

void MainWindow::onMapReady()
{
	PwGisWidget *gis = ui->gisWidget;
	/*
 PwGisStyle* cellphone = gis->createStyle("cellphone")
   ->setProperty("icon", "images/cellphone.png" )
   ->setProperty("fontColor", "#000000")
   ->setProperty("fontFamily", "arial" )
   ->setProperty("fontStyle", "background-color: yellow; border:1px solid black;")
   ->apply();
*/

	//PwGisPoint* point = gis->createPoint(91.38047386165967,53.68200710738423)

	const QString styles = ui->styleEdit->text();
	gis->addPoint( "point2", 91.38047386165967, 53.68200710738423, "point2", "", styles );
	gis->addLine( "line1", 91.3803989048849, 53.72209498850214, 91.38050000000239, 53.68199999999983, "", "", styles );

	gis->addMarker( "program-marker1", 91.30047386165967, 53.68200710738423, "", "Program defined mark", 90, styles );

//	gis->addMarker("html-marker2",
//				   91.30047386165967,53.78200710738423,
//				   MARKER_CONTENT(
//					   TITLE("Html marker title")
//					   BODY( "some text in marker body.")
//					   IMAGE( "images/map.png", "map image")
//					   ),
//                   "images/cellphone.png",0, styles);

//	gis->addMarker("html-marker3",
//				   91.30047386165967,53.88200710738423,
//				   html()
//				   .title("Html marker title")
//				   .body("some text in marker body.")
//				   .image("images/map.png", "map image")
//				   ,
//                   "images/cellphone.png",0, styles);

	PwGisPointList points = gis->getGeometry("point2");
	foreach( PwGisLonLat point, points ){
		qDebug()<< "geometry: point: " << point.lon << "," << point.lat;
	}


	gis->addCircle( "c1", 91.40047386165967,53.88200710738423, 1000, "my circle", "", styles );

	gis->addRectangle( "r1", 91.50047386165967,53.88200710738423, 91.6, 53.90, "", "my rectangle", styles );

	points.clear();

	points.append(new PwGisLonLat(91.5, 53.88));
	points.append(new PwGisLonLat(91.5, 53.89));
	points.append(new PwGisLonLat(91.6, 53.89));

	gis->addPath( "path1", &points, "airplane path", "", styles );
	points.clear();


	points.append(new PwGisLonLat(91.7, 53.85));
	points.append(new PwGisLonLat(91.5, 53.89));
	points.append(new PwGisLonLat(91.6, 53.83));
	points.append(new PwGisLonLat(91.6, 53.82));

	gis->addPolygon( "poly1", &points, "polygon", "", styles );
	points.clear();

	ui->gisWidget->addSlice( "slice", 91.5, 53.7, 1000, 0, 300, "", "", "blue default_gradient" );
}

void MainWindow::onSelectFeature( const QString& id )
{
	ui->plainTextEdit->appendPlainText(	QString( "selected feature: %1").arg(id) );
}
void MainWindow::onClickFeature( const QString& id )
{
	qDebug() << QString( "clicked feature: %1").arg(id);

	PwGisPointList points = ui->gisWidget->getGeometry(id);
	foreach( PwGisLonLat point, points ){
		qDebug() << "geometry: point: " << point.lon << "," << point.lat;
	}
}
void MainWindow::onUnselectFeature( const QString& id )
{
	ui->plainTextEdit->appendPlainText(	QString( "unselected feature: %1").arg(id) );
}
void MainWindow::onAddFeature(const QString &type, const QString &id)
{
	ui->plainTextEdit->appendPlainText(	QString( "added feature: %1, type: %2").arg(id).arg(type) );
	PwGisPointList* points = ui->gisWidget->getGeometry(id);
	for(int i = 0;  i < points->length(); i++)
	{
		qDebug()<< "geometry: point: " << points->at(i)->lon << "," << points->at(i)->lat;
	}
	m_lastAddedFeature = id;
	m_lastPoints = points;
	PwGisLonLat* newll = ui->gisWidget->getFeatureLonLat(id);
	qDebug() << "PwGisLonLat " << newll->lon << " " << newll->lat;
	QStringList props = ui->gisWidget->getFeatureProperties(id);
	int index = props.indexOf("radius");
	QString radius = "unknown";
	if(index > -1)
	{
		radius = props.at(index + 1);
	}
	ui->gisWidget->addMarker( id+"_Centre", newll->lon, newll->lat, "", "radius " + radius, 0, "" );
	delete newll;
}

void MainWindow::loadNoMap()
{
	_mapManager->openMap(QString("/nomap" ) + _mapFile.replace("/", QString( QDir::separator() )));
}

void MainWindow::onSetMarker()
{
	ui->gisWidget->addMarker( "mymarker1", 91.5,53.72, "", "M1", 0, "PostPlaneRed" );

	_mapBounds->zoomMapTo(
				91.5- 0.003,
				53.72 + 0.003,
				91.5 + 0.003,
				53.72 - 0.003 );

}

void MainWindow::onRemoveMarker()
{
	ui->gisWidget->removeMarker("mymarker1");
}

void MainWindow::onMapClick(double lon, double lat)
{
	qDebug() << "User click on " << lon << " " << lat;
}

void MainWindow::onMapMouseMove(double lon, double lat)
{
	if(ui->cbMouseMoveEvents->checkState() != Qt::Checked) return;
	qDebug() << "Mouse move " << lon << " " << lat;
}

void MainWindow::setCustomStyle()
{
	PwGisStyle* style = ui->gisWidget->createStyle("customStyle");
	style->setProperty(PwGisStyle::fillColor, "#FF0000");
	style->setProperty(PwGisStyle::strokeColor, "#00FF00");
	delete style->apply();

	ui->gisWidget->addLine( "customStyleLine1", 91.45, 53.73, 91.6, 53.73, "", "", "customStyle" );
}

void MainWindow::on_pushButton_clicked()
{
	loadMap();
}

void MainWindow::onMeasure(const QVariantList &, double measure, int status)
{
	ui->measureData->setText(QString().setNum(measure) + " " + QString().setNum(status));
}

void MainWindow::on_removeGradFeature_clicked()
{
	ui->gisWidget->removeObject("slice");
}

void MainWindow::on_addGradFeature_clicked()
{
	ui->gisWidget->addSlice( "slice", 91.5 + (rand()%100 - 50)/1000.0, 53.7 + (rand()%100 - 50)/1000.0, 1000 + (rand()%1000 - 300), 0 + (rand()%180 - 90), 300 + (rand()%180 - 90), "", "", "blue default_gradient" );
}

void MainWindow::onGetFeatureProperties()
{
	static const QString prefix = "gfp";
	static const QString featuresNames[] = { prefix + "Point1",
							  prefix + "Line1",
							  prefix + "Rectangle1",
							  prefix + "Circle1",
							  prefix + "Path1",
							  prefix + "Polygon1",
							  prefix + "Slice1" };

	static bool created = false;
	if(created) {
		for(size_t i = 0; i < sizeof(featuresNames) / sizeof(QString); i++) {
			ui->gisWidget->removeObject(featuresNames[i]);
		}
		created = false;
		return;
	}
	int i = 0;
	PwGisPointList points;

	ui->gisWidget->addPoint(featuresNames[i], 91.3, 53.68, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	ui->gisWidget->addLine( featuresNames[i], 91.3, 53.68, 91.5, 53.68, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	ui->gisWidget->addRectangle( featuresNames[i], 91.3, 53.4, 91.5, 53.68, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	ui->gisWidget->addCircle( featuresNames[i], 91.3, 53.4, 1000, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	points.clear();
	points.append(new PwGisLonLat(91.5, 53.88));
	points.append(new PwGisLonLat(91.5, 53.89));
	points.append(new PwGisLonLat(91.6, 53.89));
	ui->gisWidget->addPath( featuresNames[i], &points, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	points.clear();
	points.append(new PwGisLonLat(91.7, 53.85));
	points.append(new PwGisLonLat(91.5, 53.89));
	points.append(new PwGisLonLat(91.6, 53.83));
	points.append(new PwGisLonLat(91.6, 53.82));
	ui->gisWidget->addPolygon( featuresNames[i], &points, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	ui->gisWidget->addSlice( featuresNames[i], 91.3, 53.4, 1000, 0.0, 45.0, "", "", "" );
	qDebug() << "[" << ui->gisWidget->getFeatureProperties(featuresNames[i]).join(", ") << "]";
	i++;

	created = true;
}


void MainWindow::on_testEditFeatureButton_clicked()
{
	ui->gisWidget->pathRemovePoints(m_lastAddedFeature,0,m_lastPoints->count() - 1);
	for(int i = 0; i < m_lastPoints->length(); i++)
	{
		m_lastPoints->at(i)->lat += (rand()%100 - 50)/1000.0;
		m_lastPoints->at(i)->lon += (rand()%100 - 50)/1000.0;
	}
	ui->gisWidget->pathAddPoints(m_lastAddedFeature,m_lastPoints);
}

void MainWindow::on_testEditFeatureButton_2_clicked()
{
	double newLat = m_lastPoints->at(m_lastPoints->count() - 1)->lat + (rand()%100 - 50)/1000.0;
	double newLon = m_lastPoints->at(m_lastPoints->count() - 1)->lon + (rand()%100 - 50)/1000.0;

	PwGisLonLat* newpoint = new PwGisLonLat(newLon,newLat);
	m_lastPoints->addLonLat(newpoint);
	PwGisPointList list;
	list.addLonLat(newpoint);

	ui->gisWidget->pathAddPoints(m_lastAddedFeature,&list);
}

void MainWindow::on_testEditFeatureButton_3_clicked()
{

	ui->gisWidget->pathRemovePoints(m_lastAddedFeature,m_lastPoints->count() - 1,m_lastPoints->count() - 1);
	m_lastPoints->removeLast();
	if(m_lastPoints->count() <= 1)
	{
		ui->testEditFeatureButton->setEnabled(false);
		ui->testEditFeatureButton_2->setEnabled(false);
		ui->testEditFeatureButton_3->setEnabled(false);
	}


}

void MainWindow::on_testFormats_clicked()
{
}

void MainWindow::on_btnCustomStyleMarker_clicked()
{
	PwGisWidget *gis = ui->gisWidget;
	static bool styleCreated = false;
	if(!styleCreated)
	{
		PwGisStyle* style = gis->createStyle("markerCustomStyle")
				->setProperty(PwGisStyle::graphicHeight, "48")
				->setProperty(PwGisStyle::graphicWidth, "48")
				->apply();
		styleCreated = true;
		delete style;
	}
	gis->addMarker( "customStyledMarker",
					  91.64, 53.65, "", "custom styled marker",
					  0, "markerCustomStyle" );
}

void MainWindow::on_btnRemoveAllObjects_clicked()
{
	ui->gisWidget->removeAllObjects();
}

void MainWindow::log(QString message)
{
	g_textEdit->appendPlainText(message);
}
