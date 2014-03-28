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

#include "../Abstracts/IModuleController.h"

#include "TabsProperty.h"

#include "TabMap.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"

#include "../MapInterface/MapController.h"

#include "../Common/Router.h"

#include "../NIIPP/NIIPPControl.h"

class TabManager: public QObject, public IModuleController, public ITabManager
{
    Q_OBJECT
public:
	TabManager(QTabWidget *tabWidget, QObject *parent = 0);
    ~TabManager();

private:
	QTabWidget* m_tabWidget;

    unsigned int    _id;
    QString         _name;
    QMap<int, TabsProperty *>   _map_settings;
    QMap<int, TabMap* >    _map_tabs;
    QVBoxLayout*                _layout;
//    Controller*                 _model_controller;

    DBManager*                  _db_manager_bla;
    DBManager*                  _db_manager_evil;

    TreeModel*                  _model_spectrum;

    TabMap*                _current_tab_widget;

    QMutex                      _mux;

    ///Map
    MapController*          _map_controller;

    Router*                 _router;
//    NIIPPControl*           _niipp1;
//    NIIPPControl*           _niipp2;
    QMap<int, QDockWidget *>* _map_niipp;


public:

public:
    virtual int start();
    virtual int stop();
    virtual void show();
    virtual void hide();
    virtual int createSubModules(QString path_to_ini_file);

public:
    virtual QString getStationName(int id);
    virtual void send_data(int pid, IMessageOld* msg);
    virtual MapController* get_map_controller();

    virtual void set_dbs(IDBManager* db_bla, IDBManager* db_evil);

    virtual void send_data_niipp_control(int id, QByteArray ba);




private:
    int _read_settings(QString path_to_ini_file);

private slots:
    void _slot_change_tab(int index);

    void _slot_show_niipp1(bool state);

    void _slot_send_data_to_niipp_control(int id, QByteArray data);

signals:
    void signalSendToNIIPPControl(int,QByteArray);
};

#endif // TABMANAGER_H
