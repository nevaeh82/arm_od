#ifndef DEMOICONSREVIEW_H
#define DEMOICONSREVIEW_H

#include <QWidget>

namespace Ui {
class DemoIconsReview;
}

namespace Pw { namespace Gis {
	class IMapManager;
	class IMapBounds;
}}

class DemoIconsReview : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoIconsReview *ui;

	Pw::Gis::IMapBounds* _mapBounds;
	Pw::Gis::IMapManager* _mapManager;

	QStringList styleList;

	void addMarker(double lon, double lat, QString markerStyle);

public:
	explicit DemoIconsReview(QWidget *parent = 0);
	~DemoIconsReview();

public slots:
	void onMapReady();
	void onLoadMap();
};

#endif // DEMOICONSREVIEW_H
