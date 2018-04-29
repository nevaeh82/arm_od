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
#include <QProcess>

#include <MapManager.h>
#include <pwgiswidget.h>
#include <IMapManager.h>
#include <IProfileManager.h>

#include <Interfaces/IController.h>
#include <Interfaces/IRpcListener.h>

#include "NIIPP/nstation.h"
#include "Station.h"
#include "zavCommon/UAVDefines.h"

#include "Interfaces/IUavHistoryListener.h"

#include "Map/Map.h"
#include "Map/MapWidget.h"
#include "Map/MapClient1.h"
#include "Map/IMapController.h"

#include "TCP/TcpClientManager.h"

#include "UAV/ZInterception.h"

#include "Tabs/Tree/UavTreeModel.h"
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

	UavTreeModel* m_ourModel;
	UavTreeModel* m_enemyModel;

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

    virtual void updateStationPowerZone(const NStation& niipp);
    virtual void removeStationPowerZone(const NStation& niipp);

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

	void saveCache();

	void setEnemyModel(UavTreeModel* model);
	void setOurModel(UavTreeModel* model);

    void setTcpClientManager(TcpClientManager* manager);

private:
	IMapClient* getMapClient();

	void setViewport(QString viewport);

	QTimer* m_initJsTimer;
	QTime m_mapUpTime;
	int m_mapUpTimeout;
    TcpClientManager* m_tcpClientManager;
	QProcess m_screenProcess;
	bool m_moving;
	QTimer m_screenGrabTimer;

public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();
	void onMapClicked(double lon, double lat);
    void onMapMouseMove(double lon, double lat);

	void loadMapSettings();
	void setStationVisible(bool state);

	void clearObjects();
	void loadObjects();
	void saveObjects();

private slots:
	void mapOpenFinished();
	void onExtraBoardInfo(int);
	void screenRequest();
	void movingReset();

signals:
	void mapOpened();
	void atlasOpened();
	void mapOpenCanceled();

	void mapClicked(double, double);
	void signalOnExtraBoardInfo(int);

    void onMovingAimAlarm(QString msg);
};

#endif // MAPCONTROLLER_H
