#include "MapTabWidget.h"
#include "ui_MapTabWidget.h"
#include <QDebug>

MapTabWidget::MapTabWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::MapTabWidget)
{
	ui->setupUi(this);

	ui->spipDD1DockWidget->close();
	ui->spipDD2DockWidget->close();
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

QDockWidget *MapTabWidget::getBlaDockWidget() const
{
	return ui->blaDockWidget;
}

QDockWidget *MapTabWidget::getBplaDockWidget() const
{
	return ui->bplaDockWidget;
}

QDockWidget *MapTabWidget::getNiipp1DockWidget() const
{
	return ui->spipDD1DockWidget;
}

QDockWidget *MapTabWidget::getNiipp2DockWidget() const
{
	return ui->spipDD2DockWidget;
}
