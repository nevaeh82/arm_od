#ifndef MAPCONTROLLERWIDGET_H
#define MAPCONTROLLERWIDGET_H

#include <QWidget>

#include <pwgiswidget.h>

#include "MapController.h"

namespace Ui {
class MapWidget;
}

class MapController;

class MapWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MapWidget(QWidget *parent = 0);
	~MapWidget();

	QWidget* getWidget();
	PwGisWidget* getPwGis();

private:
	Ui::MapWidget *ui;

};

#endif // MAPCONTROLLERWIDGET_H
