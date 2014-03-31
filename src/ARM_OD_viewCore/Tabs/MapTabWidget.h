#ifndef MAPTABWIDGET_H
#define MAPTABWIDGET_H

#include <QWidget>
#include <QTreeView>

#include "MapInterface/MapWidget.h"
#include "NIIPP/NiippWidget.h"
#include "ControlPanel/ControlPanel.h"

namespace Ui {
class MapTabWidget;
}

class MapTabWidget: public QWidget
{
	Q_OBJECT
public:
	MapTabWidget(QWidget* parent = NULL);
	~MapTabWidget();

	MapWidget* getMapWidget() const;
	NiippWidget* getNiippWidget(const int number) const;

	QTreeView* getBlaTreeView() const;
	QTreeView* getBplaTreeView() const;
	ControlPanel* getControlPanelWidget() const;

	void changeBlaTreeVisibility();
	void changeBplaTreeVisibility();
	void changeNiippVisibility();

private:
	Ui::MapTabWidget* ui;

};

#endif // MAPTABWIDGET_H
