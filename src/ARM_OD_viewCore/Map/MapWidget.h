#ifndef MAPCONTROLLERWIDGET_H
#define MAPCONTROLLERWIDGET_H

#include <QWidget>

#include <pwgiswidget.h>

namespace Ui {
class MapWidget;
}

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

signals:
	void mapClicked(double, double);
};

#endif // MAPCONTROLLERWIDGET_H
