#ifndef MAPCONTROLLERWIDGET_H
#define MAPCONTROLLERWIDGET_H

#include <QWidget>

#include <pwgiswidget.h>

#include "MapController.h"

namespace Ui {
class MapControllerWidget;
}

class MapController;

class MapControllerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MapControllerWidget( MapController* controller, QWidget* parent = 0 );
	~MapControllerWidget();

	QWidget* getWidget();
	PwGisWidget* getPwGis();
	QWidget*getPanelWidget();
private:
	Ui::MapControllerWidget* ui;

	MapController* _controller;
	void mouseMoveEvent( QMouseEvent* event );

signals:
	void showBLAtree();
	void showBPLAtree();
	void showNIIPP();
};

#endif // MAPCONTROLLERWIDGET_H
