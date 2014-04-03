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

#include "Abstracts/IModuleController.h"

#include "Station.h"

#include "MapTabWidget.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"

#include "Map/MapController.h"

#include "Common/Router.h"

#include "NIIPP/NIIPPController.h"

#include "MapTabWidgetController.h"

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



    DBManager*                  _db_manager_bla;
    DBManager*                  _db_manager_evil;

    TreeModel*                  _model_spectrum;

	MapTabWidgetController*                m_currentWidget;

    QMutex                      _mux;

public:

public:
	void start();

	int createSubModules(const QString& settingsFile);

public:
    virtual void send_data(int pid, IMessageOld* msg);


    virtual void send_data_niipp_control(int id, QByteArray ba);




private:
	int readSettings(const QString &settingsFile);

private slots:
	void changeTabSlot(int index);


signals:
    void signalSendToNIIPPControl(int,QByteArray);

	void openAtlasSignal();
	void openMapSignal();
};

#endif // TABMANAGER_H
