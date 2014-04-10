#include "Icons/IconsGenerator.h"

#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MapWidget)
{
	ui->setupUi(this);

	ui->_pwwidget;
	ui->_panel_widget;

	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::gray);
	ui->_panel_widget->setAutoFillBackground(true);
	ui->_panel_widget->setPalette(p);

	IconsGenerator gen;
	ui->pb = gen.set_centred();
	ui->_panel_widget->setMaximumHeight(50);

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

QWidget* MapWidget::getPanelWidget()
{
	return ui->_panel_widget;
}

void MapWidget::mouseMoveEvent(QMouseEvent*)
{
	ui->_panel_widget->setMouseTracking(true);
}
