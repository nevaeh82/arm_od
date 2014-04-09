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

#include "../Abstracts/IModuleController.h"

#include "Station.h"

#include "MapTabWidget.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
//#include "Tree/TreeModel.h"

#include "ITabManager.h"

#include "../MapInterface/MapController.h"

#include "../NIIPP/NIIPPController.h"

#include "MapTabWidgetController.h"

#include "DbBla/DbUavManager.h"
#include "DbBla/DbUavController.h"
#include "Db/Defines.h"

#include "Interfaces/IDbBlaSettingsManager.h"
#include "DbBlaSettingsManager.h"
#include "Info/StationConfiguration.h"

class TabManager: public QObject, public ITabManager
{
    Q_OBJECT
public:
	TabManager(QTabWidget *tabWidget, QObject *parent = 0);
	virtual ~TabManager();

private:
	QTabWidget* m_tabWidget;

    unsigned int    _id;
    QString         _name;
	QMap<int, Station*>   m_stationsMap;

	QMap<QString, MapTabWidgetController* >    m_tabWidgetsMap;

   // DBManager*                  _db_manager_bla;
	IDbBlaSettingsManager* m_dbBlaSettingsManager;
	DbUavController* m_dbUavController;
	DbUavManager* m_dbUavManager;

    DBManager*                  _db_manager_evil;

	MapTabWidgetController*                m_currentWidget;

    QMutex                      _mux;

	QString m_rpcHost;
	quint16 m_rpcPort;

public:
	void start();

//	int createSubModules(const QString& settingsFile);

    virtual void send_data(int pid, IMessageOld* msg);
    virtual void send_data_niipp_control(int id, QByteArray ba);

	void setRpcConfig(const quint16& port, const QString& host);
	void setDbConnectionStruct(const DBConnectionStruct& connectionStruct);

	void setStationsConfiguration(const QList<StationConfiguration>& stationList);
	void addStationTabs();

private:
//	int readSettings(const QString &settingsFile);

//	DBConnectionStruct getDbBlaConnectionSettings();

private slots:
	void changeTabSlot(int index);

signals:
    void signalSendToNIIPPControl(int,QByteArray);

	void openAtlasSignal();
	void openMapSignal();

	void readyToStart();
};

#endif // TABMANAGER_H
