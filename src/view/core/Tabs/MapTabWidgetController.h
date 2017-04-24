#ifndef MAPTABWIDGETCONTROLLER_H
#define MAPTABWIDGETCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QEventLoop>

#include "Station.h"

#include "ITabManager.h"
#include "ITabMap.h"

#include "RPC/RpcClientWrapper.h"

#include "../Map/MapController.h"

#include "../NIIPP/NIIPPController.h"

#include "Tree/TreeWidgetDelegate.h"
#include "Tree/UavTreeModel.h"

#include "../UAV/BLAPerehvatDialog.h"

#include <Interfaces/IController.h>
#include "MapTabWidget.h"

#include "DbBla/DbUavManager.h"

#include "Solver/SolverSetupWidgetController.h"

#include "ControlPanel/ControlPanelController.h"

#define DEFAULT_RPC_PORT		24500

namespace Ui {
class MapTabWidget;
}

class MapTabWidgetController: public QObject, public IController<MapTabWidget>, public ITabMap
{
	Q_OBJECT
public:
	explicit MapTabWidgetController(Station* station, QMap<int, Station*> map_settings, ITabManager* tabManager,
									DbUavManager* db_bla, SolverSetupWidgetController* solverSetup,
									QObject* parent = NULL);
	virtual ~MapTabWidgetController();

private:
	MapTabWidget* m_view;

	Station*       m_station;

	NiippController* m_niipp1;
	NiippController* m_niipp2;


	UavTreeModel*		m_allyUavTreeModel;
	UavTreeModel*		m_enemyUavTreeModel;
	SolverSetupWidgetController* m_solverSetup;

	RpcClientWrapper*	m_rpcClient;

	DbUavManager*       m_uavDbManager;

	QMap<int, Station *> m_mapSettings;

	TreeWidgetDelegate*          m_treeDelegate;

	///Map
	MapController*          m_mapController;

	ControlPanelController* m_controlPanelController;

	ITabManager* m_tabManager;

	quint16 m_rpcHostPort;
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

	void setRpcConfig(const quint16& port, const QString& host);

	void setARMRConnection(bool b);

private:
	int init();
//	void readSettings();

signals:
	void signalStartRPC();
	void signalStopRPC();
	void signalFinishRPC();

	void initFinished();

	void mapOpened();
	void atlasOpened();
	void cancelMapOpen();

	void onDbOutLog(QString log);

private slots:
	void onBlaTreeItemDoubleClicked(QModelIndex index);

public slots:

	void onSendDataToNiippControl(int id, QByteArray data);
	void openAtlasSlot();
	void openMapSlot();
	void clearMapSlot();
	void loadObjetsSlot();
	void saveObjectsSlot();
};

#endif // MAPTABWIDGETCONTROLLER_H
