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

void MapWidget::slotApply(int)
{
	//QMessageBox::critical(this, "income", "Ok", QMessageBox::Ok);
}
