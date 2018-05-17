#ifndef TACTICMAPWINDOW_H
#define TACTICMAPWINDOW_H
#include <PwGis/pwgiswidget.h>
#include <PwGis/objects/PrimitiveMapObjectAbstract.h>
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

struct TempFigure {
	PrimitiveMapObjectAbstract* figure;
	PwGisStyle* style;
	QString styleName;
};

class DemoStyleWindow : public QMainWindow
{
	Q_OBJECT
private:

	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;
	MapObjectStyle* _mapObjectStyle;

public:
	explicit DemoStyleWindow(QWidget *parent = 0);
	~DemoStyleWindow();

	double getRandomLon();
	double getRandomLat();

private slots:
	void onBtnOpenMap();

	void onBtnAddSliceClicked();

	void onBtnCrazyClicked();

	void onBtnClearClicked();

	void on_btnTestStyle_clicked();

	void onMapReady();

	void on_btnAddMarker_clicked();

	void on_btnAddCircle_clicked();

	void on_btnAddPath_clicked();

	void on_btnAddPolygon_clicked();

	void on_btnRect_clicked();

	void onCancelStyle();

private:
	Ui::TacticMapWindow *ui;
	MapObjectStyle* _styleSector;
	PwGisStyle* _styleDynamicTest;
	PwGisStyle* _styleDynamicTest1;
	PwGisStyle* _styleDynamicTest2;
	MapObjectStyle* _styleCircle;
	Pw::Gis::IMapBounds* _mapBounds;
	GeoRectangle* _bounds;

	TempFigure _tempFigure;

	/**
	 * @brief Helper method. Create random point.
	 * @param obj parent QObject.
	 * @return
	 */
	PwGisLonLat* createRandomPoint(QObject* obj);
	void setStyleTo(PrimitiveMapObjectAbstract* obj);
	void setTempFigure(PrimitiveMapObjectAbstract* obj, PwGisStyle* style, QString styleName);
};

#endif // TACTICMAPWINDOW_H
