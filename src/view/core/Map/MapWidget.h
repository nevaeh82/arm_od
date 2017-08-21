#ifndef MAPCONTROLLERWIDGET_H
#define MAPCONTROLLERWIDGET_H

#include <QWidget>

#include <QTimer>

#include <pwgiswidget.h>
#include "BaseParser/BaseParser.h"
#include "BaseParser/BaseParserWidget.h"

#include "MapInfoWidget.h"


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
	BaseParser* m_xmlParser;

    MapInfoWidget* m_infoWidget;

    QTimer* m_movingTimer;

private:
	Ui::MapWidget *ui;

signals:
	void mapClicked(double, double);

	void signalApply(int);
	void signalClear();
	void onShowBaseStation(double, double, QString);
	void onClearBaseStation();
	void signalOnExtraBoardInfo(int);

	void signalApplyCross(int);

private slots:
	void slotApply(int);
	void slotLoadBaseStations();
    void onSetSquareVal(QString val);
    void onSetMovingAlarm(QString val);
    void onClearMovingAlarm();
};

#endif // MAPCONTROLLERWIDGET_H
