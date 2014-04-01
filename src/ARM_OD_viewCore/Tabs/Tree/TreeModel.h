#ifndef TreeModel_H
#define TreeModel_H

#include <QtGui>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

#include "ITreeModel.h"

//#include "IModelController.h"

 class TreeModel : public QAbstractItemModel, public ITreeModel
 {
     Q_OBJECT

 public:
     TreeModel(const QStringList &headers, QObject *parent = 0);
     ~TreeModel();

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

//     bool insertColumns(int position, int columns,
//                        const QModelIndex &parent = QModelIndex());
     bool removeColumns(int position, int columns,
                        const QModelIndex &parent = QModelIndex());
//     bool insertRows(int position, int rows,
//                     const QModelIndex &parent = QModelIndex());
     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
//     QMap<int, QVariant>  itemData(const QModelIndex &index) const;
     virtual bool addChild(QMap<QString, QVariant> *data, int role);

     virtual void removeChild(int pid, int id, int role);
//     void addItemChild(MainTreeItem *parent, int position, int count, int columns, QVector<QVariant> columnData, int role);

//     bool addChildProperty(const QModelIndex &index, const QVariant &ID, const QVariant &Property, int role);

//     TreeItem *get_root_item();

//     void set_id(unsigned int id);
//     MainTreeItem *getItem(const QModelIndex &index) const;
//      void fill_model(int pid);
 private:
//     void _updateItem(QVector<QVariant> *data);
//     QModelIndex get_toot_index(QString id_name);

     void _refreshModel();

 private:
     void setupModelData(const QStringList &lines, TreeItem *parent);


     TreeItem*      _rootItem;
 };

#endif // TreeModel_H
