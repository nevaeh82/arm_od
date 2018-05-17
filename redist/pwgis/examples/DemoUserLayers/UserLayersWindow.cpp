#include "UserLayersWindow.h"
#include "ui_UserLayersWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <cmath>

#include <PwGis/pwgiswidget.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/IMapManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/ILayerManager.h>
#include "styles.generated.h"

#define frand(min,max) (fmod(qrand(),(max-min)) + min)


UserLayersWindow::UserLayersWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UserLayersWindow),
	_layersCounter(0),
	_markerCounter(0),
	_featureCounter(0)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_profileManager = ui->pwgiswidget->mapProvider()->profileManager();
	_mapBounds = ui->pwgiswidget->mapProvider()->mapBounds();
	_layerManager = ui->pwgiswidget->mapProvider()->layerManager();

	connect(ui->btnOpenMap,SIGNAL(clicked()),SLOT(onBtnOpenMap()));
	connect(ui->btnCloseMap,SIGNAL(clicked()),SLOT(onBtnCloseMap()));
	connect(&_mapManager->events(),SIGNAL(mapReady()),this,SLOT(onMapReady()));
	connect(ui->btnAddMarkerLayer,SIGNAL(clicked()),SLOT(onBtnAddMarkerLayer()));
	connect(ui->btnAddVectorLayer,SIGNAL(clicked()),SLOT(onBtnAddVectorLayer()));
	connect(ui->btnRegisterProfile,SIGNAL(clicked()),SLOT(onBtnRegisterProfile()));
	connect(ui->btnUnregisterProfile,SIGNAL(clicked()),SLOT(onBtnUnregisterProfile()));
	connect(ui->btnAddMarker,SIGNAL(clicked()),SLOT(onBtnAddMarker()));
	connect(ui->btnAddFeature,SIGNAL(clicked()),SLOT(onBtnAddFeature()));
	connect(ui->btnRemoveAll,SIGNAL(clicked()),SLOT(onBtnRemoveAll()));
	connect(ui->btnSetVisible,SIGNAL(clicked()),SLOT(onBtnSetVisible()));
	connect(ui->btnSetInvisible,SIGNAL(clicked()),SLOT(onBtnSetInvisible()));

	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	QString styles = getallIconsStyleList();
	_styleList = styles.split(" ").filter("Mobil");
	//_styleList.append("");   // default marker image
}

UserLayersWindow::~UserLayersWindow()
{
	onBtnCloseMap();
	delete ui;
}

void UserLayersWindow::onBtnOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this,
			tr( "Open map" ), QApplication::applicationDirPath(), tr( "All files(*.*)" ) );

	if(mapFile.length() == 0) return;

	_mapManager->setProfile("demo");
	_mapManager->openMap( mapFile , 30 );
}

void UserLayersWindow::onBtnCloseMap()
{
	_mapManager->closeMap();
}

void UserLayersWindow::onMapReady()
{
	fillLayersList();
}

void UserLayersWindow::fillLayersList()
{
	ui->layersCB_features->clear();
	ui->layersCB_visible->clear();
	_layersIds.clear();

	QHash<QString, QString> list = _layerManager->getLayersList();
	QHashIterator<QString, QString> it(list);
	while(it.hasNext()){
		it.next();
		ui->layersCB_features->addItem(it.value());
		ui->layersCB_visible->addItem(it.value());
		_layersIds.append(it.key());
	}
}

void UserLayersWindow::onBtnAddMarkerLayer()
{
	onAddLayer(MarkerLayer);
}

void UserLayersWindow::onBtnAddVectorLayer()
{
	onAddLayer(VectorLayer);
}

void UserLayersWindow::onAddLayer(int type)
{
	QString layerName = ui->editLayerName->text();
	if(layerName.length() == 0){
		QMessageBox::warning(0,QString("Warning"),QString("Write down layer name first"),QMessageBox::Ok );
		return;
	}

	if(type == MarkerLayer){
		_layerManager->addMarkerLayer(QString("layer_%1").arg(++_layersCounter),layerName);
	}
	else{
		_layerManager->addVectorLayer(QString("layer_%1").arg(++_layersCounter),layerName);
	}

	ui->editLayerName->clear();
	fillLayersList();
}

void UserLayersWindow::onBtnRegisterProfile()
{
	_profileManager->registerProfile("demo", ":profile");
}

void UserLayersWindow::onBtnUnregisterProfile()
{
	_profileManager->unregisterProfile("demo");
}

void UserLayersWindow::onBtnRemoveAll()
{
	ui->pwgiswidget->removeAllObjects();
}

void UserLayersWindow::onBtnAddFeature()
{
	switch(qrand()%4){
	case 0:
		addRectangle();
		break;
	case 1:
		addCircle();
		break;
	case 2:
		addPolygon();
		break;
	default:
		addSlice();
		break;
	}
}


void UserLayersWindow::onBtnSetVisible()
{
	QString layerId = _layersIds.at(ui->layersCB_visible->currentIndex());
	_layerManager->setLayerVisibility(layerId, true);
}

void UserLayersWindow::onBtnSetInvisible()
{
	QString layerId = _layersIds.at(ui->layersCB_visible->currentIndex());
	_layerManager->setLayerVisibility(layerId, false);
}


void UserLayersWindow::onBtnAddMarker()
{
	QString markerName = QString("marker_%1").arg(++_markerCounter);
	addFeature(Marker, markerName);
}

void UserLayersWindow::addRectangle()
{
	QString featureName = QString("rect_%1").arg(++_featureCounter);
	addFeature(Rectangle, featureName);
}

void UserLayersWindow::addCircle()
{
	QString featureName = QString("cirlce_%1").arg(++_featureCounter);
	addFeature(Circle, featureName);
}

void UserLayersWindow::addSlice()
{
	QString featureName = QString("slice_%1").arg(++_featureCounter);
	addFeature(Slice, featureName);
}

void UserLayersWindow::addPolygon()
{
	QString featureName = QString("polygon_%1").arg(++_featureCounter);
	addFeature(Polygon, featureName);
}

void UserLayersWindow::addFeature(Features type,QString featureId){
	QString layerId = _layersIds.at(ui->layersCB_features->currentIndex());
	QString styleName = featureId;
	PwGisStyle* style = ui->pwgiswidget->createStyle(styleName);
	style->setProperty(PwGisStyle::layer, layerId)->apply();

	GeoRectangle* view = _mapBounds->getViewBounds();

	double width = view->getRight() - view->getLeft();
	double height = view->getTop() - view->getBottom();
	double lon = frand(view->getLeft(),view->getRight());
	double lat = frand(view->getBottom(),view->getTop());
	double angle = frand(0.0,179.0);
	double meters = getDistance(view->getTop(),view->getLeft(),view->getBottom(),view->getLeft());

	// add marker icon
	if(type == Marker){
		styleName.append(" ");
		styleName.append(_styleList.at( qrand()%_styleList.count() ));
	}

	switch(type){
	case Marker:
		ui->pwgiswidget->addMarker( featureId, lon, lat,
			featureId + ".caption", featureId + ".tooltip",
			frand(0.0,90.0), styleName);
		break;
	case Rectangle:
		ui->pwgiswidget->addRectangle( featureId,
			lon, lat, lon + frand(0.1,0.5)*width, lat + frand(0.1,0.5)*height,
			featureId + ".caption", featureId + ".tooltip", styleName);
		break;
	case Circle:
		ui->pwgiswidget->addCircle( featureId, lon, lat, frand(0.1,0.5)*meters,
			featureId + ".caption", featureId + ".tooltip", styleName);
		break;
	case Slice:
		ui->pwgiswidget->addSlice( featureId, lon, lat, frand(0.1,0.5)*meters, angle, angle+frand(30.0,179.0),
			featureId + ".caption", featureId + ".tooltip", styleName);
		break;
	case Polygon:
		ui->pwgiswidget->addClassicPolygon( featureId, lon, lat,
			frand(0.1,0.5)*meters, qrand()%10+4, angle,
			featureId + ".caption", featureId + ".tooltip", styleName);
	default:break;
	}
}

double UserLayersWindow::getDistance(double lat1, double lon1, double lat2, double lon2)
{
	double PI = 3.14;
	double R = 6378.137; // Radius of earth in KM
	double dLat = (lat2 - lat1) * PI / 180;
	double dLon = (lon2 - lon1) * PI / 180;
	double a = sin(dLat/2) * sin(dLat/2) +
			cos(lat1 * PI / 180) * cos(lat2 * PI / 180) *
			sin(dLon/2) * sin(dLon/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double d = R * c;
	return d * 1000; // meters
}
