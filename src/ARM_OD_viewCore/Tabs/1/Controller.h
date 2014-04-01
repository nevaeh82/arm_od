#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QStringList>
#include "MainTreeModelManager.h"
#include "MainTabView.h"
#include "MainTreeItem.h"

#include "dbmanager.h"

#include "../Interfaces/core/IController.h"

#include "../Interfaces/core/IExchangeMainTreeItem.h"

#include "ContainerItems.h"

class Controller : public QObject, public IController
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

public:
    void set_exchanger(IExchangeMainTreeItem *exchanger);

public:
//    void setView(MainTabView *view);
    QList<QMap<QString, QVariant> > requestdata(QStringList list);
    QList<QMap<QString, QVariant> > requestdata(QStringList list, int id);
    void get_data(QStringList list);
    virtual void setupData();
    void request_all_stations();
    void request_current_station();
//    MainTabView *get_view();
    virtual MainTreeModelManager    *get_model();
    virtual void                    reset_model(int id, int id_view);
    void update_property(int pid, int id, /*QString name,*/ QVariant value);

private:
    MainTreeModelManager    *_model;
    MainTabView             *_view;
    DBManager               _db_man;
    QSqlRelationalTableModel *model;
    int                     _id;
    IExchangeMainTreeItem   *_exchanger;
    QMap<int, ContainerItems *> *_map_items;
//    QMap<int, MainTreeItem *> *_map_items;
//    QMap<int, QVector<QMap<int, MainTreeItem *> *> > *_map_st_items; /// map of station itam - struct of QVector
                                                                    /// where Qvector[0] - rootitem, else items

//    QVector<QMap<int, MainTreeItem *> > *_vec_items;

private:
    void update_value_in_db(const QModelIndex &index);
    void _fill_model(QList<QMap<QString, QVariant> > list_of_map);

public slots:
    void slotChangeValueForDB(const QModelIndex &index1, const QModelIndex &index2);

};

#endif // CONTROLLER_H
