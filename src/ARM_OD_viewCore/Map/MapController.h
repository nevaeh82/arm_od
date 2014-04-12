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

#include "MapWidget.h"
#include "Map.h"

#include "Station.h"

#include "MapClient1.h"
#include "IMapController.h"

#include "../UAV/ZInterception.h"

#include <Interfaces/IController.h>
#include <Interfaces/IRpcListener.h>

#include "RPC/RpcDefines.h"
#include "UAVDefines.h"

const double m_zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
					   5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
					   18, 20, 22, 24, 28, 30};
const double m_zoneDir[28] = {2.5, 3, 4, 5,
					   6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
					   22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

class MapWidget;

class MapController : public QObject, public IMapController, public IController<MapWidget>, public IRpcListener
{
	Q_OBJECT

private:
	MapWidget* m_view;
	Map* m_mapModel;

	QMap<int, int> m_mapPelengEvilIds;
	int m_pelengEvilIds;
	int m_rdsEvilIds;

public:
	MapController(QObject* parent =NULL);
	virtual ~MapController();
	void init(QMap<int, Station *> map_settings);

	virtual IMapClient  *getMapClient(int id);

	void addMarkerLayer(int id, QString name);

	void appendView(MapWidget* view);

	void closeMap();
	void closeAtlas();

public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();

private slots:
	void _slot_station_visible(bool state);

signals:
	void mapOpened();

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
	void sendEnemyUavPoints(const QByteArray& data, const int& mapClientId);
};

#endif // MAPCONTROLLER_H
