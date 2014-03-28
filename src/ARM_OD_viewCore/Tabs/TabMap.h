#ifndef TABMAP_H
#define TABMAP_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QThread>

#include <QDockWidget>

#include "../Abstracts/ISubModule.h"

#include "TabsProperty.h"

#include "Tree/TreeView.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"
#include "ITabMap.h"

#include "RPC/RPCClient.h"

#include "../MapInterface/MapController.h"

#include "../Common/IRouter.h"

#include "../NIIPP/NIIPPController.h"

#include "Tree/BLAWidgetDelegate.h"

#include "../UAV/BLAPerehvatDialog.h"

namespace Ui {
	class TabMap;
}

class TabMap: public QWidget, public ISubModule, public ITabMap
{
    Q_OBJECT
public:
    TabMap(TabsProperty* prop, IRouter* router, ITabManager* tab_manager, QMap<int, QDockWidget*>* map_niipp, IDBManager* db_bla, IDBManager* db_evil);
    ~TabMap();

private:
	Ui::TabMap *_ui;
    int                 _id;
    QString             _name;
    TabsProperty*       _tab_property;

    IRouter*            _router;

    QHBoxLayout*        _hboxlayout;

    TreeView*           _tree_view_BLA;
    QDockWidget*        _dock_tree_BLA;
    TreeView*           _tree_view;
    QDockWidget*        _dock_tree_view;

//    IDBManager*          _db_manager;

    TreeModel*          _model;
    TreeModel*          _model_BLA;


    RPCClient*          _rpc_client1;

    ITabManager*        _tab_manager;

    QVBoxLayout*        _vboxlayout;

    DBManager*          _db_manager;
    DBManager*          _db_manager_target;

    QMap<int, QDockWidget* >*   _map_niipp;

    BLAWidgetDelegate*          _delegate_BLA;


//    BLAPerehvatDialog*          _bla_perehvat;



public:
    virtual int start();
    virtual int stop();
    virtual void show();
    virtual void hide();
    virtual int createRPC();
    virtual int closeRPC();
    virtual int createView(QWidget* view);
    virtual int createTree();

private:
    int _init();

public:
    TabsProperty *get_tab_property();
    void set_command(IMessageOld *msg);


signals:
    void signalStartRPC();
    void signalStopRPC();
    void signalFinishRPC();
    void signalGetPointsFromRPCFlakon(QVector<QPointF> points);

    void signalTreeBLAUpdate(int);

private slots:
    void _slot_get_points_from_rpc(QVector<QPointF> points);

    void _slot_tree_bla_double_clicked(QModelIndex index);

    void _slot_tree_BLA_Changed(QModelIndex, QModelIndex);

public slots:
    void slot_show_tree_BLA();
    void slot_show_tree_BPLA();
    void slot_show_niipp();
};

#endif // TABSPECTRUM_H
