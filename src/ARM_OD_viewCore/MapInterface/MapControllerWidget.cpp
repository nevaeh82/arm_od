#include "../Icons/IconsGenerator.h"

#include "MapControllerWidget.h"
#include "ui_MapControllerWidget.h"

MapControllerWidget::MapControllerWidget(MapController *controller, QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::MapControllerWidget)
	, _controller(controller)
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

MapControllerWidget::~MapControllerWidget()
{
	delete ui;
}

QWidget* MapControllerWidget::getWidget()
{
	return this;
}

PwGisWidget* MapControllerWidget::getPwGis()
{
	return ui->_pwwidget;
}

QWidget* MapControllerWidget::getPanelWidget()
{
	return ui->_panel_widget;
}

void MapControllerWidget::mouseMoveEvent(QMouseEvent* event)
{
	ui->_panel_widget->setMouseTracking(true);
}
