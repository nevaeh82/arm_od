#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QMenu>
#include <QEvent>
#include <QMap>

#include <MapManager.h>
#include <pwgiswidget.h>
#include <IMapManager.h>
#include <IProfileManager.h>

#include <Interfaces/IController.h>
#include <Interfaces/IRpcListener.h>

#include "Station.h"
#include "UAVDefines.h"

#include "Interfaces/IUavHistoryListener.h"

#include "Map/Map.h"
#include "Map/MapWidget.h"
#include "Map/MapClient1.h"
#include "Map/IMapController.h"

#include "UAV/ZInterception.h"

#include "RPC/RpcDefines.h"

#include "SolverPacket1.pb.h"

#define MAP_INITJS_TIMEOUT 1000

class MapController : public QObject, public IMapController, public IController<MapWidget>,
		public IRpcListener, public IUavHistoryListener
{
	Q_OBJECT

private:
	MapWidget* m_view;
	Map* m_mapModel;

	QMap<int, INiippController*> m_niippControllers;

	QMap<int, int> m_mapPelengEvilIds;
	int m_pelengEvilIds;

	QRectF m_viewport;

public:
	MapController(QObject* parent = NULL);
	virtual ~MapController();

	void init(QMap<int, Station *> stations);

	void closeMap();
	void closeAtlas();

	// interface IMapController
	virtual IMapClient* getMapClient(int id);
	virtual void setNiippController(INiippController* controller);
	virtual void updateNiippPowerZone(const Niipp& niipp);
	virtual void removeNiippPoint();

	// interface IController
	virtual void appendView(MapWidget* view);

	// interface IRpcListener
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	// interface IUavHistoryListener
	virtual void onUavAdded(const Uav&, const QString&) {}
	virtual void onUavRemoved(const Uav& uav, const QString&);
	virtual void onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole,
								  const QVector<QPointF> &tail = QVector<QPointF>(),
								  const QVector<QPointF> &tailStdDev = QVector<QPointF>());
	virtual void onStatusChanged(Status) {}

private:
	IMapClient* getMapClient();

	void setViewport(QString viewport);

	QTimer* m_initJsTimer;

public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();
	void onMapClicked(double lon, double lat);

	void loadMapSettings();
	void setStationVisible(bool state);

private slots:
	void mapOpenFinished();

signals:
	void mapOpened();
	void atlasOpened();
	void mapOpenCanceled();

	void mapClicked(double, double);
};

#endif // MAPCONTROLLER_H
