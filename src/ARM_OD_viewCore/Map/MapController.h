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

#include "Interfaces/IUavDbChangedListener.h"

#include "Map/Map.h"
#include "Map/MapWidget.h"
#include "Map/MapClient1.h"
#include "Map/IMapController.h"

#include "UAV/ZInterception.h"

#include "RPC/RpcDefines.h"

class MapController : public QObject, public IMapController, public IController<MapWidget>,
		public IRpcListener, public IUavDbChangedListener
{
	Q_OBJECT

private:
	MapWidget* m_view;
	Map* m_mapModel;

	QMap<int, INiippController*> m_niippControllers;

	QMap<int, int> m_mapPelengEvilIds;
	int m_pelengEvilIds;

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

	// interface IUavDbChangedListener
	virtual void onUavAdded(const Uav&, const QString&) {}
	virtual void onUavRemoved(const Uav& uav, const QString&);
	virtual void onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole);

private:
	IMapClient* getMapClient();

public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();
	void onMapClicked(double lon, double lat);

	void setStationVisible(bool state);

signals:
	void mapOpened();
	void atlasOpened();
	void cancelMapOpen();
};

#endif // MAPCONTROLLER_H
