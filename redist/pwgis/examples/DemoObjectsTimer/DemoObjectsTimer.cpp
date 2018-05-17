#include "DemoObjectsTimer.h"
#include "ui_DemoObjectsTimer.h"
#include <QDebug>
#include <QTime>

#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>

#include <PwGis/IMapBounds.h>
#include <PwGis/PwGisLonLat.h>

DemoObjectsTimer::DemoObjectsTimer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::DemoObjectsTimer)
{
	ui->setupUi(this);

	pwgiswidget = new PwGisWidget(this);
	_mapManager = pwgiswidget->mapProvider()->mapManager();
	_mapBounds =  pwgiswidget->mapProvider()->mapBounds();

	currentTimeStart = new QTime();
	objNums << "100" << "500" << "1000" << "2000" << "5000" << "8000" << "10000";
	startBenchmarkFlag = false;

	ui->verticalLayout_2->addWidget(pwgiswidget);
	connect(ui->btnOpenMap, SIGNAL(clicked()), this, SLOT(onOpenMap()));
	connect(ui->btnCloseMap, SIGNAL(clicked()), this, SLOT(onCloseMap()));

	connect(ui->btnZoomIn, SIGNAL(clicked()), this, SLOT(onZoomIn()));
	connect(ui->btnZoomOut, SIGNAL(clicked()), this, SLOT(onZoomOut()));
	connect(ui->btnUp, SIGNAL(clicked()), this, SLOT(onMoveUp()));
	connect(ui->btnDown, SIGNAL(clicked()), this, SLOT(onMoveDown()));
	connect(ui->btnLeft, SIGNAL(clicked()), this, SLOT(onMoveLeft()));
	connect(ui->btnRight, SIGNAL(clicked()), this, SLOT(onMoveRight()));

	connect(ui->btnAddMarker, SIGNAL(clicked()), this, SLOT(onAddMarkers()));
	connect(ui->btnAddPolygon, SIGNAL(clicked()), this, SLOT(onAddPolygons()));

	connect(ui->btnDeleteAllFeatures, SIGNAL(clicked()), this, SLOT(onBtnDeleteAllFeatures()));
	connect(ui->btnBenchmark, SIGNAL(clicked()), this, SLOT(onBenchmark()));

	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(onClearEdit()));
	connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(onSaveEdit()));
}

DemoObjectsTimer::~DemoObjectsTimer()
{
	delete currentTimeStart;
	delete ui;
}

void DemoObjectsTimer::onOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));

	pwgiswidget->mapProvider()->mapManager()->openMap(mapFile, 30);
}

void DemoObjectsTimer::onCloseMap()
{
	pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void DemoObjectsTimer::startTimeCount()
{
	currentTimeStart->start();
}

void DemoObjectsTimer::getTimeResult(QString message)
{
	int time = 0;
	time = currentTimeStart->elapsed();
	QString rMessage = message + QString::number(time) + QString("ms")  + QString("\r\n");
	qDebug() << rMessage;
	ui->textEdit->insertPlainText(rMessage);
	if(startBenchmarkFlag)
		logString.append(rMessage);
}

void DemoObjectsTimer::onBtnDeleteAllFeatures()
{
	pwgiswidget->removeAllObjects();
}

void DemoObjectsTimer::onZoomIn()
{
	int currentZoom;
	currentZoom = _mapBounds->getCurrentZoom();
	if((currentZoom + 1) < 20)
	{
		startTimeCount();
		_mapBounds->setCurrentZoom(currentZoom+1);
		getTimeResult(QString("Zoom in time: "));
	}
}

void DemoObjectsTimer::onZoomOut()
{
	int currentZoom;
	currentZoom = _mapBounds->getCurrentZoom();
	if((currentZoom - 1) > 0)
	{
		startTimeCount();
		_mapBounds->setCurrentZoom(currentZoom - 1);
		getTimeResult(QString("Zoom out time: "));
	}
}

void DemoObjectsTimer::onMoveUp()
{
	PwGisLonLat* res = _mapBounds->getMapCenter();
	startTimeCount();
	_mapBounds->setMapCenter(res->getLon(), res->getLat() + 1);
	getTimeResult(QString("Move up time: "));
}

void DemoObjectsTimer::onMoveDown()
{
	PwGisLonLat* res = _mapBounds->getMapCenter();
	startTimeCount();
	_mapBounds->setMapCenter(res->getLon(), res->getLat() - 1);
	getTimeResult(QString("Move down time: "));
}

void DemoObjectsTimer::onMoveLeft()
{
	PwGisLonLat* res = _mapBounds->getMapCenter();
	startTimeCount();
	_mapBounds->setMapCenter(res->getLon() - 1, res->getLat());
	getTimeResult(QString("Move left time: "));
}

void DemoObjectsTimer::onMoveRight()
{
	PwGisLonLat* res = _mapBounds->getMapCenter();
	startTimeCount();
	_mapBounds->setMapCenter(res->getLon() + 1, res->getLat());
	getTimeResult(QString("Move right time: "));
}

void DemoObjectsTimer::onAddMarkers()
{
	addMarkers(false, ui->spinBox->value());
}

void DemoObjectsTimer::onAddPolygons()
{
	addPolygons(false, ui->spinBox->value());
}

void DemoObjectsTimer::addPolygons(bool useTransaction, int num)
{
	double lon, lat, rad;
	startTimeCount();

	if(useTransaction)
	{
		pwgiswidget->startTransaction();
	}

	for( int i=0; i < num; i++ ) {
		lon = -180 + (double) qrand() / RAND_MAX * 360;
		lat = -60 + (double) qrand() / RAND_MAX * 142;
		rad = 100000 + (double) qrand() / RAND_MAX * 1000000;
		pwgiswidget->addCircle(createPolygonName(i), lon, lat, rad, "", "", "");
	}

	if(useTransaction)
	{
		pwgiswidget->commitTransaction();
		getTimeResult(QString("Adding %1 polygons with transaction time: ").arg(num));
	}
	else
	{
		getTimeResult(QString("Adding %1 polygons time: ").arg(num));
	}
}

void DemoObjectsTimer::addMarkers(bool useTransaction, int num)
{
	double lon, lat, rad;
	startTimeCount();

	if(useTransaction)
		pwgiswidget->startTransaction();

	for( int i = 0; i < num; i++ ) {
		lon = -180 + (double) qrand() / RAND_MAX * 360;
		lat = -60 + (double) qrand() / RAND_MAX * 142;
		rad = 100000 + (double) qrand() / RAND_MAX * 1000000;

		pwgiswidget->addMarker(createMarkerName(i), lon, lat, "", "", 0, "PostPlaneRed selectAndDrag");
	}

	if(useTransaction)
	{
		pwgiswidget->commitTransaction();
		getTimeResult(QString("Adding %1 markers with transaction time: ").arg(num));
	}
	else
	{
		getTimeResult(QString("Adding %1 markers time: ").arg(num));
	}
}

void DemoObjectsTimer::benchmarkActions()
{
	onZoomIn();
	onZoomIn();
	onZoomOut();
	onMoveDown();
	onMoveDown();
	onMoveUp();
	onMoveUp();
	onMoveLeft();
	onMoveRight();
}

void DemoObjectsTimer::onBenchmark()
{
	startBenchmarkFlag = true;
	benchmarkActions();

	for(int i = 0; i < objNums.size(); i++ )
	{
		addPolygons(true, objNums.at(i).toInt());
		onBtnDeleteAllFeatures();
		addPolygons(false, objNums.at(i).toInt());
		benchmarkActions();
		qApp->processEvents();
	}

	onBtnDeleteAllFeatures();

	for(int i = 0; i < objNums.size(); i++ )
	{
		addMarkers(true, objNums.at(i).toInt());
		onBtnDeleteAllFeatures();
		addMarkers(false, objNums.at(i).toInt());
		benchmarkActions();
		qApp->processEvents();
	}

	onBtnDeleteAllFeatures();

	for(int i = 0; i < objNums.size(); i++ )
	{
		addPolygons(false, objNums.at(i).toInt() / 2);
		addMarkers(false, objNums.at(i).toInt() / 2);
		benchmarkActions();
		qApp->processEvents();
	}

	onBtnDeleteAllFeatures();
	startBenchmarkFlag = false;
}

void DemoObjectsTimer::onClearEdit()
{
	ui->textEdit->clear();
}

void DemoObjectsTimer::onSaveEdit()
{
	QString logFile;
	logFile = QFileDialog::getSaveFileName(this, "Save log", QApplication::applicationDirPath(), "Text file (*.txt)");

	QFile file(logFile);
	if(!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	file.write(logString.toUtf8());
	logString.clear();
	file.close();
}





