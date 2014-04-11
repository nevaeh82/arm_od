#ifndef MAPTABWIDGETCONTROLLER_H
#define MAPTABWIDGETCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QThread>

#include "Station.h"

#include "ITabManager.h"
#include "ITabMap.h"

#include "RPC/RPCClient.h"

#include "../MapInterface/MapController.h"

#include "../NIIPP/NIIPPController.h"

#include "Tree/TreeWidgetDelegate.h"
#include "Tree/UavTreeModel.h"

#include "../UAV/BLAPerehvatDialog.h"

#include <Interfaces/IController.h>
#include "MapTabWidget.h"

#include "DbBla/DbUavManager.h"

#define DEFAULT_RPC_PORT		24500

namespace Ui {
class MapTabWidget;
}

class MapTabWidgetController: public QObject, public IController<MapTabWidget>, public ITabMap
{
	Q_OBJECT
public:
	MapTabWidgetController(Station* station, QMap<int, Station*> map_settings, ITabManager* tabManager, DbUavManager* db_bla, QObject* parent = NULL);
	virtual ~MapTabWidgetController();

private:
	MapTabWidget* m_view;

	Station*       m_station;

	NiippController* m_niipp1;
	NiippController* m_niipp2;


	UavTreeModel*		m_allyUavTreeModel;
	UavTreeModel*		m_enemyUavTreeModel;

	RPCClient*          m_rpcClient;

	DbUavManager*          m_uavDbManager;

	QMap<int, Station *> m_mapSettings;

	TreeWidgetDelegate*          m_treeDelegate;

	///Map
	MapController*          m_mapController;

	ITabManager* m_tabManager;

	int m_rpcHostPort;
	QString m_rpcHostAddress;

public:
	virtual int start();
	virtual int stop();
	virtual void show();
	virtual void hide();
	virtual int createRPC();
	virtual int closeRPC();
	virtual int createTree();

	void set_command(IMessageOld *msg);

	void appendView(MapTabWidget* view);

private:
	int init();
	void readSettings();

signals:
	void signalStartRPC();
	void signalStopRPC();
	void signalFinishRPC();

private slots:
	void onBlaTreeItemDoubleClicked(QModelIndex index);

public slots:
	void onShowBlaTree();
	void onShowBplaTree();
	void onShowNiipp();

	void onSendDataToNiippControl(int id, QByteArray data);
	void openAtlasSlot();
	void openMapSlot();
};

#endif // MAPTABWIDGETCONTROLLER_H
