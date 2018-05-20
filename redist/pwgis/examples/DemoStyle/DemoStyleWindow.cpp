#include <QTimer>
#include "DemoStyleWindow.h"
#include "ui_DemoStyleWindow.h"
#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/PwGisPointList.h>

#include <PwGis/objects/Sector.h>
#include <PwGis/TacticMap/TracedObjectAbstract.h>
#include <MapProvider.h>
#include <IdGenerator.h>
#include <PwGis/objects/Marker.h>
#include <PwGis/objects/Path.h>
#include <PwGis/objects/Polygon.h>
#include <PwGis/objects/Circle.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/Rectangle.h>
#include <QFileDialog>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>

DemoStyleWindow::DemoStyleWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::TacticMapWindow)
{
	ui->setupUi(this);

	_idGenerator = new IdGenerator(this);
	_mapProvider = ui->pwgiswidget->mapProvider();
	_mapBounds = _mapProvider->mapBounds();
	_tempFigure.figure = NULL;

	Pw::Gis::MapManagerEvents* mapEvents = &ui->pwgiswidget->mapProvider()->mapManager()->events();

	connect(ui->btnOpenMap, SIGNAL(clicked()), SLOT(onBtnOpenMap()));
	connect(ui->btnAddSlice, SIGNAL(clicked()), SLOT(onBtnAddSliceClicked()));
	connect(ui->btnClear, SIGNAL(clicked()), SLOT(onBtnClearClicked()));
	connect(ui->btnCrazy, SIGNAL(clicked()), SLOT(onBtnCrazyClicked()));
	connect(mapEvents,SIGNAL(mapReady()), SLOT(onMapReady()));
	connect(ui->btnCancelStyle, SIGNAL(clicked()), SLOT(onCancelStyle()));

	_styleDynamicTest = _mapProvider->styleFactory()->createNamedStyle("style");
	_styleDynamicTest1 = _mapProvider->styleFactory()->createNamedStyle("style1");
	_styleDynamicTest2 = _mapProvider->styleFactory()->createNamedStyle("style2");
}

DemoStyleWindow::~DemoStyleWindow()
{
	_mapProvider->mapManager()->closeMap();
	delete ui;
}

double DemoStyleWindow::getRandomLon()
{
	double width = _bounds->getRight() - _bounds->getLeft();
	double rnd = (double)rand()/100;
	while(rnd > width) rnd -= width;
	return rnd + _bounds->getLeft();
}

double DemoStyleWindow::getRandomLat()
{
	double height = _bounds->getTop() - _bounds->getBottom();
	double rnd = (double)rand()/100;
	while(rnd > height) rnd -= height;
	return rnd + _bounds->getBottom();
}

PwGisLonLat* DemoStyleWindow::createRandomPoint(QObject* obj)
{
	return new PwGisLonLat(getRandomLon(),getRandomLat(), obj);
}

void DemoStyleWindow::onBtnOpenMap()
{
	QString mapPath = QFileDialog::getOpenFileName( this, tr( "Open map" ),
			QApplication::applicationDirPath(), tr( "Map files(*.*)" ) );

	if ( mapPath.isEmpty() ) {
		return;
	}

	_mapProvider->mapManager()->openMap(mapPath);
}

void DemoStyleWindow::onBtnAddSliceClicked()
{
	Sector* sector = _mapProvider->objectsFactory()->createSector();

	if(!sector) return;

	sector->setOriginPoint( new PwGisLonLat(getRandomLon(), getRandomLat()));
	sector->setRadius(1000 + (rand()%1000 - 300));
	sector->setStartAngle(90);
	sector->setEndAngle(160);
	sector->setName( "I am slice" );
	setStyleTo(sector);

    sector->updateMap();
}

void DemoStyleWindow::onBtnCrazyClicked()
{
	for (int i = 0; i < 1000; i++) {
		if(!(i % 20)) {
			onBtnAddSliceClicked();
			ui->checkBoxStyle->setChecked(true);
			ui->checkBoxStyle2->setChecked(false);
			ui->checkBoxStyle3->setChecked(false);
		}
		if(!(i % 30)) {
			on_btnAddCircle_clicked();
			ui->checkBoxStyle->setChecked(false);
			ui->checkBoxStyle2->setChecked(true);
			ui->checkBoxStyle3->setChecked(false);
		}
		if(!(i % 40)) {
			on_btnAddPolygon_clicked();
			ui->checkBoxStyle->setChecked(false);
			ui->checkBoxStyle2->setChecked(false);
			ui->checkBoxStyle3->setChecked(true);
		}
		if(!(i % 50)) {
			on_btnAddPath_clicked();
		}

	}
}

void DemoStyleWindow::onBtnClearClicked()
{
	ui->pwgiswidget->removeAllObjects();
}

void DemoStyleWindow::on_btnTestStyle_clicked()
{
}

void DemoStyleWindow::onMapReady()
{
	ui->btnAddSlice->setEnabled(true);
	ui->btnClear->setEnabled(true);
	ui->btnCrazy->setEnabled(true);
	ui->btnAddCircle->setEnabled(true);
	ui->btnAddPath->setEnabled(true);
	ui->btnAddPolygon->setEnabled(true);
	ui->btnAddMarker->setEnabled(true);
	ui->btnRect->setEnabled(true);

	_styleDynamicTest->setProperty(PwGisStyle::strokeColor, "#FF0000");
	_styleDynamicTest->setProperty(PwGisStyle::fillColor, "#FF0000");
	_styleDynamicTest->setProperty(PwGisStyle::strokeWidth, "5");
	_styleDynamicTest->setProperty(PwGisStyle::cursor, "pointer");
	_styleDynamicTest->setProperty(PwGisStyle::strokeDashstyle, "dash");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontOpacity, "1");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontColor, "#FF0000");

	_styleDynamicTest->apply();

	_styleDynamicTest1->setProperty(PwGisStyle::strokeColor, "#00FF00");
	_styleDynamicTest1->setProperty(PwGisStyle::fillColor, "#00FF00");
	_styleDynamicTest1->setProperty(PwGisStyle::strokeWidth, "10");
	_styleDynamicTest1->setProperty(PwGisStyle::cursor, "crosshair");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontOpacity, "1");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontColor, "#00FF00");

	_styleDynamicTest1->apply();

	_styleDynamicTest1->setProperty(PwGisStyle::strokeColor, "#0000FF");
	_styleDynamicTest2->setProperty(PwGisStyle::fillColor, "#0000FF");
	_styleDynamicTest2->setProperty(PwGisStyle::strokeWidth, "20");
	_styleDynamicTest2->setProperty(PwGisStyle::cursor, "progress");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontOpacity, "1");
	_styleDynamicTest->setProperty(PwGisStyle::mapFontColor, "#0000FF");

	_styleDynamicTest2->apply();

	_bounds = _mapBounds->getMapBounds();

}

void DemoStyleWindow::on_btnAddMarker_clicked()
{
	Marker * marker = _mapProvider->objectsFactory()->createMarker();

	setStyleTo(marker);
	marker->setName( "I am marker" );
	marker->position()->setLat(getRandomLat());
	marker->position()->setLon(getRandomLon());
	marker->setToolTip("marker tooltip");
	marker->setRotate(50.0);
    marker->updateMap();
}

void DemoStyleWindow::on_btnAddCircle_clicked()
{
	Circle* circle = _mapProvider->objectsFactory()->createCircle();
	setStyleTo(circle);
	circle->setRadius(1000);
	circle->originPoint()->setLat(getRandomLat());
	circle->originPoint()->setLon(getRandomLon());
	circle->setName( "I am circle" );
    circle->updateMap();
}

void DemoStyleWindow::on_btnAddPath_clicked()
{
	Path* path = _mapProvider->objectsFactory()->createPath();
	setStyleTo(path);
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), path));
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), path));
	path->points()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), path));
	path->setName("I am path");
	path->updateMap();
}

void DemoStyleWindow::on_btnAddPolygon_clicked()
{
	Polygon* polygon = _mapProvider->objectsFactory()->createPolygon();
	setStyleTo(polygon);
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->corners()->append(new PwGisLonLat(getRandomLon(), getRandomLat(), polygon));
	polygon->setName("I am polygon");
	polygon->updateMap();
}

void DemoStyleWindow::on_btnRect_clicked()
{
	Rectangle* rect = _mapProvider->objectsFactory()->createRectangle();
	setStyleTo(rect);
	rect->topRight()->setLat(getRandomLat());
	rect->topRight()->setLon(getRandomLon());
	rect->bottomLeft()->setLat(getRandomLat());
	rect->bottomLeft()->setLon(getRandomLon());
	rect->setName("I am rect");
	rect->updateMap();
}


void DemoStyleWindow::setStyleTo(PrimitiveMapObjectAbstract* obj)
{
	if (ui->checkBoxStyle->isChecked()) {
		if (ui->comboBoxStyle->currentIndex() == 0) {
			obj->addStyle(_styleDynamicTest);
		}
		else {
			obj->addStyleByName("style");
		}

		setTempFigure( obj, _styleDynamicTest, "style" );

		return;
	}

	if(ui->checkBoxStyle2->isChecked()) {
		if(ui->comboBoxStyle->currentIndex() == 0) {
			obj->addStyle(_styleDynamicTest1);
		}
		else {
			obj->addStyleByName("style1");
		}

		setTempFigure( obj, _styleDynamicTest1, "style1" );

		return;
	}

	if(ui->checkBoxStyle3->isChecked()) {
		if(ui->comboBoxStyle->currentIndex() == 0) {
			obj->addStyle(_styleDynamicTest2);
		}
		else {
			obj->addStyleByName( "style2" );
		}

		setTempFigure( obj, _styleDynamicTest2, "style2" );

		return;
	}

	return;
}

void DemoStyleWindow::onCancelStyle()
{
	if (!_tempFigure.figure) {
		return;
	}

	if (ui->comboBoxStyle->currentIndex() == 0) {
		_tempFigure.figure->removeStyle(_tempFigure.style);
	}
	else {
		_tempFigure.figure->removeStyleByName(_tempFigure.styleName);
	}

	_tempFigure.figure->updateMap();
}

void DemoStyleWindow::setTempFigure(PrimitiveMapObjectAbstract* obj, PwGisStyle* style, QString styleName)
{
	_tempFigure.figure = obj;
	_tempFigure.style = style;
	_tempFigure.styleName = styleName;
}
