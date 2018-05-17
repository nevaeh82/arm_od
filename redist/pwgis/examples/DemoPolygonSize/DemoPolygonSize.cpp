#include "DemoPolygonSize.h"
#include "ui_DemoPolygonSize.h"

#include <PwGis/PwGisPointList.h>

#define polygonID "Polygon1"
#define circleID "circle1"

DemoPolygonSize::DemoPolygonSize(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::DemoPolygonSize)
{
	ui->setupUi(this);
	pwgiswidget = new PwGisWidget(this);

	ui->horizontalLayout->addWidget(pwgiswidget);
	connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(onOpenMap()));
	connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onCloseMap()));

	connect(ui->radioPolygon, SIGNAL(clicked(bool)), this, SLOT(onAddPolygon(bool)));
	connect(ui->radioCircle, SIGNAL(clicked(bool)), this, SLOT(onAddCircle(bool)));
	connect(ui->sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeSize(int)));
}

DemoPolygonSize::~DemoPolygonSize()
{
	delete ui;
}

void DemoPolygonSize::onOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));

	pwgiswidget->mapProvider()->mapManager()->openMap(mapFile, 30);
}

void DemoPolygonSize::onCloseMap()
{
	pwgiswidget->mapProvider()->mapManager()->closeMap();
}

void DemoPolygonSize::onAddPolygon(bool b)
{
	if(!b)
	{
		return;
	}

	int i = ui->sizeSlider->value();
	pwgiswidget->removeObject(circleID);
	PwGisPointList polygonPoints;
	polygonPoints.append( new PwGisLonLat( 60.0 - i, -1.0 - i/2 ) );
	polygonPoints.append( new PwGisLonLat( 80.0 - i, 11.0 + i/1.5 ) );
	polygonPoints.append( new PwGisLonLat( 90.0 + i/1.5, -9.0 - i/3) );
	pwgiswidget->addPolygon(polygonID, &polygonPoints, "", "", "blue selectAndDrag" );
}

void DemoPolygonSize::onAddCircle(bool b)
{
	if(!b)
	{
		return;
	}

	int i = ui->sizeSlider->value();
	pwgiswidget->removeObject(polygonID);
	pwgiswidget->addCircle(circleID, 90.0, 10.0, 500000.0 + (100000*i), "", "", "blue selectAndDrag");
}

void DemoPolygonSize::onChangeSize(int i)
{
	if(ui->radioPolygon->isChecked())
	{
		PwGisPointList polygonPoints;
		polygonPoints.append( new PwGisLonLat( 60.0 - i, -1.0 - i/2 ) );
		polygonPoints.append( new PwGisLonLat( 80.0 - i, 11.0 + i/1.5 ) );
		polygonPoints.append( new PwGisLonLat( 90.0 + i/1.5, -9.0 - i/3) );
		pwgiswidget->addPolygon(polygonID, &polygonPoints, "", "", "blue selectAndDrag" );
	}
	else if(ui->radioCircle->isChecked())
	{
		pwgiswidget->addCircle(circleID, 90.0, 10.0, 500000.0 + (100000*i), "", "", "blue selectAndDrag");
	}
	else
	{
		return;
	}
}
