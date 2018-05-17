#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class UserLayersWindow;
}
namespace Pw { namespace Gis {
	class IMapManager;
	class IProfileManager;
	class IMapBounds;
	class ILayerManager;
}}

class UserLayersWindow : public QWidget
{
	Q_OBJECT

	enum LayerType{
		MarkerLayer,
		VectorLayer
	};

	enum Features{
		Marker,
		Rectangle,
		Circle,
		Slice,
		Polygon
	};

public:
	explicit UserLayersWindow(QWidget *parent = 0);
	~UserLayersWindow();

private:
	Ui::UserLayersWindow *ui;

	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IProfileManager * _profileManager;
	Pw::Gis::IMapBounds * _mapBounds;
	Pw::Gis::ILayerManager* _layerManager;
	QStringList _layersIds;
	QStringList _styleList;

	int _layersCounter;
	int _markerCounter;
	int _featureCounter;

	void fillLayersList();

	void addRectangle();
	void addCircle();
	void addSlice();
	void addPolygon();

	void onAddLayer(int type);
	void addFeature(Features type,QString featureName);
	double getDistance(double lat1, double lon1, double lat2, double lon2);

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onMapReady();
	void onBtnAddMarkerLayer();
	void onBtnAddVectorLayer();
	void onBtnRegisterProfile();
	void onBtnUnregisterProfile();
	void onBtnAddMarker();
	void onBtnAddFeature();
	void onBtnRemoveAll();
	void onBtnSetVisible();
	void onBtnSetInvisible();
};

#endif // MAINWINDOW_H
