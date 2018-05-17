#ifndef BEARINGLIFETIMEWINDOW_H
#define BEARINGLIFETIMEWINDOW_H

#include <QWidget>

class QTimer;
class Bearing;
class ITacticMap;
class IIdGenerator;
class IMapProvider;
class PwGisLonLat;

namespace Ui {
class BearingLifetimeWindow;
}

class BearingLifetimeWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::BearingLifetimeWindow *ui;

	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;

	QTimer* _timer;
	QTimer* _limitTimer;
	Bearing* _bearing;
	double _angle;
	double _angleDeg;
	PwGisLonLat * _point;

	bool _isEnabledTimer;
	bool _isEnabledLimitTimer;
	ITacticMap* _tacticMap;

public:
	explicit BearingLifetimeWindow( QWidget *parent = 0 );
	~BearingLifetimeWindow();

public slots:
	void onBtnOpenMap();
	void onBtnEnableTimer();
	void onBtnEnableLimitTimer();
	void onUpdateBearing();
	void onBtnDeleteBearing();
	void onBtnDeleteLimitBearing();
	void onUpdateLimitBearing();
	void onMapReady();
};

#endif // BEARINGLIFETIMEWINDOW_H
