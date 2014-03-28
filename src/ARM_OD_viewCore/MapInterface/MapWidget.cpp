#include "../Icons/IconsGenerator.h"

#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::MapWidget)
{
	ui->setupUi(this);

	connect(ui->pb_show_BLA_tree, SIGNAL(clicked()), this, SIGNAL(showBLAtree()));
	connect(ui->pb_show_BPLA_tree, SIGNAL(clicked()), this, SIGNAL(showBPLAtree()));
	connect(ui->pb_show_NiiPP, SIGNAL(clicked()), this, SIGNAL(showNIIPP()));
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
