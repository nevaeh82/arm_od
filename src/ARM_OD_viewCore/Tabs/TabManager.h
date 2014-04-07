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

#include "DbBla/DbBlaManager.h"
#include "DbBla/DbBlaController.h"
#include "Db/Defines.h"

#include "Interfaces/IDbBlaSettingsManager.h"
#include "DbBlaSettingsManager.h"

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
	DbBlaController* m_dbBlaController;
	DbBlaManager* m_dbBlaManager;

    DBManager*                  _db_manager_evil;

	MapTabWidgetController*                m_currentWidget;

    QMutex                      _mux;

public:
	void start();

	int createSubModules(const QString& settingsFile);

    virtual void send_data(int pid, IMessageOld* msg);
    virtual void send_data_niipp_control(int id, QByteArray ba);

private:
	int readSettings(const QString &settingsFile);

	DBConnectionStruct getDbBlaConnectionSettings();

private slots:
	void changeTabSlot(int index);

signals:
    void signalSendToNIIPPControl(int,QByteArray);

	void openAtlasSignal();
	void openMapSignal();
};

#endif // TABMANAGER_H
