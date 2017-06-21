#include <QMessageBox>

#include "../Icons/IconsGenerator.h"

#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::MapWidget)
{
	ui->setupUi(this);

	connect( ui->_pwwidget, SIGNAL(mapClicked(double,double)), this, SIGNAL(mapClicked(double, double)) );

	m_xmlParser = new BaseParser(this);

	connect(m_xmlParser->getView(), SIGNAL(signalBaseStationOnMap(double,double,QString)), this, SIGNAL(onShowBaseStation(double, double, QString)));
	connect(m_xmlParser->getView(), SIGNAL(signalClearStationOnMap()), this, SIGNAL(onClearBaseStation()));
}

MapWidget::~MapWidget()
{
	delete ui;
}

QWidget* MapWidget::getWidget()
{
	return this;
}

PwGisWidget* MapWidget::getPwGis()
{
	return ui->_pwwidget;
}

void MapWidget::slotApply(int id)
{
	emit signalApplyCross(id);
	//QMessageBox::critical(this, "income", "Ok", QMessageBox::Ok);
}

void MapWidget::slotLoadBaseStations()
{
	m_xmlParser->getView()->show();
}
