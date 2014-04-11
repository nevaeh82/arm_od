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

	IDbBlaSettingsManager* m_dbBlaSettingsManager;
	DbUavController* m_dbUavController;
	DbUavManager* m_dbUavManager;

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
