#ifndef MAPCONTROLLERWIDGET_H
#define MAPCONTROLLERWIDGET_H

#include <QWidget>

#include <pwgiswidget.h>

namespace Ui {
class MapWidget;
}

class MapController;

class MapWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MapWidget( QWidget* parent = 0 );
	~MapWidget();

	QWidget* getWidget();
	QWidget* getPanelWidget();
	PwGisWidget* getPwGis();

private:
	Ui::MapWidget* ui;

	void mouseMoveEvent(QMouseEvent*);

signals:
	void showBLAtree();
	void showBPLAtree();
	void showNIIPP();
};

#endif // MAPCONTROLLERWIDGET_H
