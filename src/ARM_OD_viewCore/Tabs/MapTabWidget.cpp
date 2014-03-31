#include "MapTabWidget.h"
#include "ui_MapTabWidget.h"
#include <QDebug>

MapTabWidget::MapTabWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::MapTabWidget)
{
	ui->setupUi(this);

}

MapTabWidget::~MapTabWidget()
{

}

MapWidget *MapTabWidget::getMapWidget() const
{
	return ui->mapWidget;
}

NiippWidget* MapTabWidget::getNiippWidget(const int number) const {

	if (number == 1) {
		return ui->spipDD1Widget;
	}

	if (number == 2) {
		return ui->spipDD2Widget;
	}

	return NULL;
}

QTreeView *MapTabWidget::getBlaTreeView() const
{
	return ui->blaTreeView;
}

QTreeView *MapTabWidget::getBplaTreeView() const
{
	return ui->bplaTreeView;
}

ControlPanel *MapTabWidget::getControlPanelWidget() const
{
	return ui->controlPanelWidget;
}

void MapTabWidget::changeBlaTreeVisibility()
{
	if (!ui->blaDockWidget->isVisible()){
		ui->blaDockWidget->show();
	} else{
		ui->blaDockWidget->hide();
	}

}

void MapTabWidget::changeBplaTreeVisibility()
{
	if (!ui->bplaDockWidget->isVisible()){
		ui->bplaDockWidget->show();
	} else{
		ui->bplaDockWidget->hide();
	}
}

void MapTabWidget::changeNiippVisibility()
{
	if (!ui->spipDD1DockWidget->isVisible()){
		ui->spipDD1DockWidget->show();
	} else{
		ui->spipDD1DockWidget->hide();
	}

	if (!ui->spipDD2DockWidget->isVisible()){
		ui->spipDD2DockWidget->show();
	} else{
		ui->spipDD2DockWidget->hide();
	}
}
