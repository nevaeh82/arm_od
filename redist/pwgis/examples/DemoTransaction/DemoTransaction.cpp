#include <cstdlib>

#include "DemoTransaction.h"
#include "ui_DemoTransaction.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QTime>
#include <PwGis/PwGisPointList.h>

#define ADD_MARKERS_COUNT 5000
#define ADD_CIRCLES_COUNT 1000

DemoTransaction::DemoTransaction(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DemoTransaction),
	_counterLine( 0 )
{
	ui->setupUi(this);

	connect( ui->btnOpenMap, SIGNAL( clicked() ), this, SLOT( onBtnOpenMap() ) );
	connect( ui->btnCloseMap, SIGNAL( clicked() ), this, SLOT( onBtnCloseMap() ) );
	connect( ui->btnAddMarkersWithoutTrans, SIGNAL( clicked() ), this, SLOT( onBtnAddMarkersWithoutTrans() ) );
	connect( ui->btnAddMarkersWithTrans, SIGNAL( clicked() ), this, SLOT( onBtnAddMarkersWithTrans() ) );
	connect( ui->btnAddPathWithoutTrans, SIGNAL( clicked() ), this, SLOT( onBtnAddPathWithoutTrans() ) );
	connect( ui->btnAddPathWithTrans, SIGNAL( clicked() ), this, SLOT( onBtnAddPathWithTrans() ) );
	connect( ui->btnRemoveAllFeatures, SIGNAL( clicked() ), this, SLOT( onBtnRemoveAllFeatures() ) );
	connect( ui->btnBenchmark, SIGNAL( clicked() ), this, SLOT( onBenchmark() ) );
	connect( ui->btnAddPoints, SIGNAL( clicked() ), this, SLOT( onBtnAddPoints() ) );

	ui->txtCount->setText( QString("%1").setNum(ADD_MARKERS_COUNT));
}

DemoTransaction::~DemoTransaction()
{
	delete ui;
}

QString DemoTransaction::getRuText( const QString &text )
{
	return QString::fromUtf8( text.toLatin1().data() );
}

void DemoTransaction::onBtnOpenMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->openMap(
		QFileDialog::getOpenFileName(
		this,
		tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)")),30);
	this->showMaximized();
}

void DemoTransaction::onBtnCloseMap()
{
	ui->pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void DemoTransaction::onBtnAddMarkersWithoutTrans()
{
	addMarkers(false);
}

void DemoTransaction::onBtnAddMarkersWithTrans()
{
	addMarkers();
}

void DemoTransaction::onBtnAddPathWithoutTrans()
{
	addCircles(false);
}

void DemoTransaction::onBtnAddPathWithTrans()
{
	addCircles();
}

void DemoTransaction::onBtnAddPoints()
{
	addPoints();
}

void DemoTransaction::onBtnRemoveAllFeatures()
{
	ui->pwgiswidget->startTransaction();
	ui->pwgiswidget->removeAllObjects();
	ui->pwgiswidget->commitTransaction();
}

void DemoTransaction::addMarkers( bool useTransaction )
{
	QEventLoop loop;
	ui->pwgiswidget->addDragControls();

	if (useTransaction) {
		ui->pwgiswidget->startTransaction();
	}
	int count = ui->txtCount->text().toLong();
	bool isShowLabel = ui->cbShowLabels->isChecked();

	double lon, lat;
	for( int i=0; i < count; i++ ) {
		lon = -180 + (double) qrand() / RAND_MAX * 360;
		lat = -60 + (double) qrand() / RAND_MAX * 142;

		QString id = createMarkerName(i);
		ui->pwgiswidget->addMarker( id, lon, lat, isShowLabel ? id : "",  "tooltip", 0, "finder" );
		if ( i % 100 == 0 ) {
			loop.processEvents();
		}
	}

	if (useTransaction) {
		ui->pwgiswidget->commitTransaction();
	}
}

void DemoTransaction::addCircles( const bool useTransaction )
{
	QEventLoop loop;
	if (useTransaction) {
		ui->pwgiswidget->startTransaction();
	}
	int count = ui->txtCount->text().toLong();
	bool isShowLabel = ui->cbShowLabels->isChecked();

	double lon, lat, rad;
	for( int i=0; i < count; i++ ) {
		lon = -180 + (double) qrand() / RAND_MAX * 360;
		lat = -60 + (double) qrand() / RAND_MAX * 142;
		rad = 100000; // 100000 + (double) qrand() / RAND_MAX * 1000000;

		QString id = createObjectName(i);
		ui->pwgiswidget->addCircle( id, lon, lat, rad, isShowLabel ? id : "", "tooltip", "" );
		if ( i % 100 == 0 ) {
			loop.processEvents();
		}
	}

	if (useTransaction) {
		ui->pwgiswidget->commitTransaction();
	}
}

void DemoTransaction::addPoints()
{
	QEventLoop loop;

	int count = ui->txtCount->text().toLong();
	bool isShowLabel = ui->cbShowLabels->isChecked();

	double lon, lat;
	for( int i=0; i < count; i++ ) {
		lon = -180 + (double) qrand() / RAND_MAX * 360;
		lat = -60 + (double) qrand() / RAND_MAX * 142;

		QString id = createObjectName(i);
		ui->pwgiswidget->addPoint(id, lon, lat, isShowLabel ? id : "", "tooltip", "" );
		if ( i % 100 == 0 ) {
			loop.processEvents();
		}
	}
}

void DemoTransaction::onBenchmark()
{
	int cnt = 500;
	double lon = 91.5;
	double lat = 53.72;
	double cur_lon;
	int nt_add = 0;
	int nt_rem = 0;
	int t_add = 0;
	int t_rem = 0;
	qDebug() << QString("Starting transaction test with %1 objects").arg(cnt * 2);
	QTime t;
	t.start();
	for(int i = 0; i < cnt; i++) {
		cur_lon = lon + i * 0.01;
		ui->pwgiswidget->addCircle( QString("ttcircle%1").arg(i), cur_lon, lat, 1000, QString("ttc%1").arg(i), "", "" );
		ui->pwgiswidget->addMarker( QString("ttmarker%1").arg(i), cur_lon, lat, "", QString("ttm%1").arg(i), 0, "" );
	}
	nt_add = t.elapsed();
	qDebug() << "NOT transacted features add time: " << nt_add;
	t.restart();
	for(int i = 0; i < cnt; i++) {
		ui->pwgiswidget->removeObject(QString("ttcircle%1").arg(i));
		ui->pwgiswidget->removeMarker(QString("ttmarker%1").arg(i));
	}
	nt_rem = t.elapsed();
	qDebug() << "NOT transacted features remove time: " << nt_rem;
	t.restart();
	ui->pwgiswidget->startTransaction();
	for(int i = 0; i < cnt; i++) {
		cur_lon = lon + i * 0.01;
		ui->pwgiswidget->addCircle( QString("ttcircle%1").arg(i), cur_lon, lat, 1000, QString("ttc%1").arg(i), "", "" );
		ui->pwgiswidget->addMarker( QString("ttmarker%1").arg(i), cur_lon, lat, "", QString("tt%1").arg(i), 0, "" );
	}
	ui->pwgiswidget->commitTransaction();
	t_add = t.elapsed();
	qDebug() << "transacted features add time: " << t_add;
	t.restart();
	ui->pwgiswidget->startTransaction();
	for(int i = 0; i < cnt; i++) {
		ui->pwgiswidget->removeObject(QString("ttcircle%1").arg(i));
		ui->pwgiswidget->removeMarker(QString("ttmarker%1").arg(i));
	}
	ui->pwgiswidget->commitTransaction();
	t_rem = t.elapsed();
	qDebug() << "transacted features remove time: " << t_rem;
	qDebug() << "Transactions benchmark scores:";
	qDebug() << "NOT transacted add/remove time (sec): " << nt_add << "/" << nt_rem;
	qDebug() << "Transacted add/remove time (sec): " << t_add << "/" << t_rem;
}
