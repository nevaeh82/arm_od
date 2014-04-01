#ifndef MAINTREEMODELMANAGER_H
#define MAINTREEMODELMANAGER_H

#include <QtGui>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "../Interfaces/core/IExchangeMainTreeItem.h"

 class MainTreeItem;

 class MainTreeModelManager : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     MainTreeModelManager(const QStringList &headers, QObject *parent = 0);
     ~MainTreeModelManager();

     void set_exchanger(IExchangeMainTreeItem *exchanger);

     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
     bool setHeaderData(int section, Qt::Orientation orientation,
                        const QVariant &value, int role = Qt::EditRole);

     bool insertColumns(int position, int columns,
                        const QModelIndex &parent = QModelIndex());
     bool removeColumns(int position, int columns,
                        const QModelIndex &parent = QModelIndex());
     bool insertRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());
     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
//     QMap<int, QVariant>  itemData(const QModelIndex &index) const;
//     bool addChild(const QModelIndex &index, const QVariant &value, int role);
//     void addItemChild(MainTreeItem *parent, int position, int count, int columns, QVector<QVariant> columnData, int role);

//     bool addChildProperty(const QModelIndex &index, const QVariant &ID, const QVariant &Property, int role);

     MainTreeItem *get_root_item();

//     void set_id(unsigned int id);
     MainTreeItem *getItem(const QModelIndex &index) const;

//     void updateItem(QString name, QVariant value);
//     QModelIndex get_toot_index(QString id_name);

     void refreshModel();

 private:
     void setupModelData(const QStringList &lines, MainTreeItem *parent);


     MainTreeItem *_rootItem;

//     IExchangeMainTreeItem  *_exchanger;

     unsigned int _id;

 };

#endif // MAINTREEMODELMANAGER_H
