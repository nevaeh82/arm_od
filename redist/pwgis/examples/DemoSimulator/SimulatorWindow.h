#ifndef POSTTRACKINGWINDOW_H
#define POSTTRACKINGWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <PwGis/pwgiswidget.h>

class QTimer;
class Post;
class PostStyle;
class ITacticMap;
class IIdGenerator;
class IMapProvider;

namespace Pw {
namespace Gis {
class Marker;
class Polygon;
}
}

namespace Ui {
class SimulatorWindow;
}

using namespace Pw::Gis;

class SimulatorWindow : public QWidget
{
	Q_OBJECT

private:
	void addToLog(const QString& id, const QString& type, const QString& text);
	void getNextPoint(PwGisLonLat* point, double* angle);
	Ui::SimulatorWindow *ui;
	ITacticMap*   _tacticMap;
	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;
	PwGisLonLat* _center;
	QTimer* _timer ;

	Post* _moveablePostRed;
	Post* _moveablePostBlue;
	PostStyle* _styleRedPlane;
	PostStyle* _styleBluePlane;

	QList<Marker*> _markerList;

	Marker* _point1, *_point2;

	Polygon* _polygon;

	double _angle;
	double _mapHeight;
	double _cx;
	double _cy;
	double _r;

public:
	explicit SimulatorWindow(QWidget *parent = 0);
	~SimulatorWindow();

public slots:
	void onUpdateObjects();
	void onMapReady();

	void onBtnOpenMap();
	void onBtnStartTracking();
	void onBtnStopTracking();

	void onCurrentItemChanged(int index);
	void onMapObjectChanged(const QString& id, const QString& type);
	void onFeatureAdded(const QString& id, const QString& type);
};

#endif // POSTTRACKINGWINDOW_H
