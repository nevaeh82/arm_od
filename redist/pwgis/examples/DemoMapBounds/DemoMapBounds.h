#ifndef DEMOMAPBOUNDS_H
#define DEMOMAPBOUNDS_H

#include <QWidget>

namespace Pw { namespace Gis {
class IMapBounds;
class IMapManager;
}}

namespace Ui {
class DemoMapBounds;
}

class DemoMapBounds : public QWidget
{
	Q_OBJECT

public:
	explicit DemoMapBounds(QWidget *parent = 0);
	~DemoMapBounds();

private:
	Ui::DemoMapBounds *ui;

	Pw::Gis::IMapBounds* _mapBounds;
	Pw::Gis::IMapManager* _mapManager;

public slots:
	void onBtnZoomMapTo();
	void onBtnZoomMapToByOffset();
	void onBtnSetMapCenter();
	void onBtnGetMapBounds();
	void onBtnGetViewBounds();
	void onBtnGetZoom();
	void onBtnSetZoom();
	void onBtnGetZoomInfo();
	void onZoomChanged(int zoomLevel);
	void onMapReady();
};

#endif // DEMOMAPBOUNDS_H
