#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QSettings>
#include <QMap>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QMutex>
#include <QThread>

#include "Station.h"
#include "MapTabWidget.h"
#include "ITabManager.h"
#include "Map/MapController.h"
#include "NIIPP/NIIPPController.h"
#include "MapTabWidgetController.h"

#include "DbBla/DbUavManager.h"
#include "DbBla/DbUavController.h"
#include "Db/Defines.h"

#include "SettingsManager/DbBlaSettingsManager.h"
#include "Info/StationConfiguration.h"

#include "Solver/SolverSetupWidgetController.h"

class TabManager: public QObject, public ITabManager
{
	Q_OBJECT

private:
	QTabWidget* m_tabWidget;

	QMap<int, Station*> m_stationsMap;

	QMap<QString, MapTabWidgetController* > m_tabWidgetsMap;

	DbUavController* m_dbUavController;
	DbUavManager* m_dbUavManager;
	SolverSetupWidgetController* m_solverSetup;

	MapTabWidgetController* m_currentWidget;

	QString m_rpcHost;
	quint16 m_rpcPort;

	QMenu* m_viewMenu;

	MapWidget* m_mapWidget;

	RpcClientWrapper* m_rpcClient;

public:
	TabManager(QTabWidget *tabWidget, QObject *parent = 0);
	virtual ~TabManager();

	MapWidget* getMapWidget() {return m_mapWidget;}

	void start();

	void setViewMenu(QMenu* menu);


	virtual void send_data(int pid, IMessageOld* msg);
	virtual void send_data_niipp_control(int id, QByteArray ba);

	void set_arm_r_connection(bool);

	void setRpcConfig(const quint16& port, const QString& host);
	void setDbConnectionStruct(const DBConnectionStruct& connectionStruct);

	void setStationsConfiguration(const QList<StationConfiguration>& stationList);
	void addStationTabs();
	void clearAllInformation();

	void addSolverSetupController(SolverSetupWidgetController* controller);

	void setUavLifeTime(int msecs);

	void setARMRConnection(bool b);

	RpcClientWrapper* getRpcClient() {return m_rpcClient;}

private slots:
	void changeTabSlot(int index);

signals:
	void signalSendToNIIPPControl(int,QByteArray);

	void openAtlasSignal();
	void openMapSignal();
	void mapOpened();
	void atlasOpened();
	void cancelMapOpen();
	void readyToStart();
	void finished();

	void signalClearMap();
	void signalLoadObjects();
	void signalSaveObjects();

	void signalOnExtraBoardInfo(int);

    void signalAlarm(QString, bool);

};

#endif // TABMANAGER_H
