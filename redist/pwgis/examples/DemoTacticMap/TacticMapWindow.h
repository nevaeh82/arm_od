#ifndef TACTICMAPWINDOW_H
#define TACTICMAPWINDOW_H
#include <PwGis/pwgiswidget.h>
#include <PwGis/TacticMap/TacticMap.h>
#include <PwGis/TacticMap/ITacticMap.h>
#include <QMainWindow>
#include <QList>
#include <stdlib.h>
#include <PwGis/IMapBounds.h>

class IMapProvider;
class IIdGenerator;
class RadioSourceStyle;
class PostStyle;
class BearingStyle;
class MapObjectStyle;

namespace Ui {
class TacticMapWindow;
}

using namespace Pw::Gis;

class TacticMapWindow : public QMainWindow
{
	Q_OBJECT
private:
	ITacticMap* _tacticMap;
	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;
	MapObjectStyle* _mapObjectStyle;

public:
	explicit TacticMapWindow(QWidget *parent = 0);
	~TacticMapWindow();

	double getRandomLon();
	double getRandomLat();

private slots:
	void onBtnOpenMap();

	void onBtnAddSliceClicked();

	void onBtnAddBearingClicked();

	void onBtnAddSourceClicked();

	void onBtnAddPostClicked();

	void onBtnCrazyClicked();

	void onBtnClearClicked();

	void on_btnTestStyle_clicked();

	void onMapReady();

	void on_btnAddMarker_clicked();

	void on_btnAddCircle_clicked();

	void on_btnAddPath_clicked();

	void on_btnAddPolygon_clicked();

	void on_btnRect_clicked();

private:
	Ui::TacticMapWindow *ui;
	RadioSourceStyle* _styleIri;
	PostStyle* _stylePost;
	BearingStyle* _styleBearing;
	MapObjectStyle* _styleSector;
	PwGisStyle* _styleDynamicTest;
	MapObjectStyle* _styleCircle;
	Pw::Gis::IMapBounds* _mapBounds;
	GeoRectangle* _bounds;

	/**
	 * @brief Helper method. Create random point.
	 * @param obj parent QObject.
	 * @return
	 */
	PwGisLonLat* createRandomPoint(QObject* obj);
};

#endif // TACTICMAPWINDOW_H
