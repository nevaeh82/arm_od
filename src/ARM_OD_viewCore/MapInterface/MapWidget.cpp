#include "../Icons/IconsGenerator.h"

#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::MapWidget)
{
	ui->setupUi(this);
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
