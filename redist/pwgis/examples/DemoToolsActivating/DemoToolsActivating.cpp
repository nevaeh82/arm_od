#include "DemoToolsActivating.h"
#include "ui_DemoToolsActivating.h"

#include <QFileDialog>
#include <PwGis/IMapManager.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IMapBounds.h>
#include <PwGis/GeoRectangle.h>
#include <PwGis/MapManagerEvents.h>

#define PROFILE_NAME   "DemoToolsActivating"

DemoToolsActivating::DemoToolsActivating(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DemoToolsActivating)
{
	ui->setupUi(this);

	_mapManager = ui->pwgiswidget->mapProvider()->mapManager();
	_profileManager = ui->pwgiswidget->mapProvider()->profileManager();

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	connect( ui->btnOpenMap, SIGNAL(clicked()), SLOT(onOpenMap()));
	connect( ui->btnCloseMap, SIGNAL(clicked()), SLOT(onCloseMap()));
	connect( ui->btnRegisterProfile, SIGNAL(clicked()), SLOT(onRegisterProfile()));
	connect( ui->btnUnregisterProfile, SIGNAL(clicked()), SLOT(onUnregisterProfile()));
	connect( ui->btnOpenAtlas , SIGNAL(clicked()), SLOT(onOpenAtlas()));
	connect( ui->btnCloseAtlas, SIGNAL(clicked()), SLOT(onCloseAtlas()));

	connect( &_mapManager->events(), SIGNAL(mapReady()), SLOT(onMapReady()));
	//ui->pwgiswidget->enableDebugger(true);
}

DemoToolsActivating::~DemoToolsActivating()
{
	delete ui;
}

void DemoToolsActivating::onOpenMap()
{
	QString mapFile = QFileDialog::getOpenFileName(this, tr("Open map"), QApplication::applicationDirPath(), tr("All files(*.*)"));

	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openMap(mapFile, 30);
}

void DemoToolsActivating::onOpenAtlas()
{
	_mapManager->setProfile(PROFILE_NAME);
	_mapManager->openAtlas();
}

void DemoToolsActivating::onCloseAtlas()
{
	_mapManager->closeAtlas();
}

void DemoToolsActivating::onCloseMap()
{
	_mapManager->closeMap();
}

void DemoToolsActivating::onMapReady()
{
	GeoRectangle* bounds = ui->pwgiswidget->mapProvider()->mapBounds()->getMapBounds();
	double lon = 30;
	double lat = 60;

	ui->pwgiswidget->addMarker("m1",lon, lat, "marker m1", "tooltip m1", 0, "myStyle");

	bounds->deleteLater();
}

void DemoToolsActivating::onRegisterProfile()
{
	//Register profile from Resources
	_profileManager->registerProfile(PROFILE_NAME, ":/DemoToolsActivating/profile");
}

void DemoToolsActivating::onUnregisterProfile()
{
	_profileManager->unregisterProfile(PROFILE_NAME);
}

void DemoToolsActivating::on_actiavateTool_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolNavigation);
}

void DemoToolsActivating::on_actiavateTool_2_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolDrawCircle);
}

void DemoToolsActivating::on_actiavateTool_6_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolDrawRectangle);
}

void DemoToolsActivating::on_actiavateTool_5_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolRuler);
}

void DemoToolsActivating::on_actiavateTool_4_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolCurvometr);
}

void DemoToolsActivating::on_actiavateTool_3_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolDrawPoint);
}

void DemoToolsActivating::on_actiavateTool_7_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolDrawPath);
}

void DemoToolsActivating::on_actiavateTool_8_clicked()
{
	ui->pwgiswidget->activateTool(PwGisWidget::ToolChangeGeometry);
}


void DemoToolsActivating::on_checkTool_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolRuler,checked);
}

void DemoToolsActivating::on_checkTool_2_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolDrawRectangle,checked);
}

void DemoToolsActivating::on_checkTool_3_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolDrawCircle,checked);
}

void DemoToolsActivating::on_checkTool_4_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolNavigation,checked);
}

void DemoToolsActivating::on_checkTool_5_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolDrawPoint,checked);
}

void DemoToolsActivating::on_checkTool_6_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolCurvometr,checked);
}

void DemoToolsActivating::on_checkTool_7_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolDrawPath,checked);
}

void DemoToolsActivating::on_checkTool_9_toggled(bool checked)
{
	ui->pwgiswidget->showTool(PwGisWidget::ToolChangeGeometry,checked);
}
