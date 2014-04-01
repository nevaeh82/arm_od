#include "TreeItem.h"
#include "TreeModel.h"

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
     : QAbstractItemModel(parent)
 {
     QVector<QVariant> rootHeaders;
     foreach (QString header, headers)
     {
         rootHeaders << header;
     }

     _rootItem = new TreeItem(NULL);
 }

 TreeModel::~TreeModel()
 {
     delete _rootItem;
 }

 int TreeModel::columnCount(const QModelIndex & /* parent */) const
 {
     return 2;//_rootItem->columnCount();
 }

 QVariant TreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
     {
         return QVariant();
     }

     int col = index.column();
     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     //TreeItem
     switch(role)
     {
        case Qt::DisplayRole:
        {
            if(!item)
            {
                return QVariant();
            }
            else if(col == 0)
             {
                if(item->data()->contains("name"))
                {
                    return item->data()->value("name");
                }
                return QVariant();
             }
            else if(col == 1)
             {
                if(item->data()->contains("value"))
                {
                    return item->data()->value("value");
                }
                return QVariant();
             }
            else
            {
                return QVariant();
            }
            break;
        }

        case Qt::CheckStateRole:
        {
//             if(!item->isCheckable(col))
//             {
//                 return QVariant();
//             }
            if(item->data()->value("state").toInt() > 1)
            {
                return QVariant();
            }
            if(col == 1)
            {
                 QModelIndex ind = index.sibling(index.row(), 0);                 
                 if(ind.data().isNull())
                 {
                     return static_cast<int>(item->data()->value("state").toBool() ? Qt::Checked : Qt::Unchecked);
                 }
                 else
                 {
                     return QVariant();
                 }
            }
            else
            {
                 if(!index.data().isNull())
                 {
                     return static_cast<int>(item->data()->value("state").toBool() ? Qt::Checked : Qt::Unchecked);
                 }
                 else
                 {
                     return QVariant();
                 }
            }
            break;
        }
        case Qt::SizeHintRole:
        {
            return QSize(150, 18);
        }
         break;
        case Qt::EditRole:
            if(!item)
            {
                return QVariant();
            }
            else if(col == 0)
            {
                if(item->data()->contains("name"))
                {
                    return item->data()->value("name");
                }
                return QVariant();
            }
            else if(col == 1)
            {
                if(item->data()->contains("value"))
                {
                    return item->data()->value("value");
                }
                return QVariant();
            }
            else
            {
                return QVariant();
            }
            qDebug() << "EDITROLE!";
        break;
         case Qt::BackgroundColorRole:
//              return QColor(Qt::red);
         break;

         case Qt::UserRole+1:
         if(!item)
         {
             return QVariant();
         }
         if(item->data()->contains("value"))
         {
             return item->data()->value("value");
         }
         return QVariant();
         break;
    }
    return QVariant();
 }

 Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

     //if(!index.data(Qt::DisplayRole).isNull())

//         flags |= Qt::ItemIsUserCheckable;

     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     ///only 1 column will be editable
     if((index.column() != 0 )&&(item->data()->value("editable").toBool() == true))
     {
        flags |= Qt::ItemIsEditable;
     }
     else
     {
//         qDebug() << "here!" << index.column() << item->data()->value("editable").toBool();
     }

     if(item->data()->value("state") != 3)
     {
            flags |= Qt::ItemIsUserCheckable;
     }

     return flags;
 }

 bool TreeModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
 {
     bool b = true;
     for (QMap<int, QVariant>::ConstIterator it = roles.begin(); it != roles.end(); ++it)
         b = b && setData(index, it.value(), it.key());
     return b;
 }

 QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
     {
         switch(section)
         {
         case 0:
             return tr("Название");
         case 1:
             return tr("Свойство");
         default:
             break;
         }
     }
     return QVariant();
 }

 QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
 {
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     TreeItem* p_item = static_cast<TreeItem *>(parent.internalPointer());
     if(p_item == NULL)
     {
         p_item = _rootItem;
     }
     TreeItem *childItem = p_item->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

// bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
// {
//     bool success;

//     beginInsertColumns(parent, position, position + columns - 1);
//     success = _rootItem->insertColumns(position, columns);
//     endInsertColumns();

//     return success;
// }

// bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
// {
//     TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());
//     bool success;

//     beginInsertRows(parent, position, position + rows - 1);
////     success = parentItem->insertChildren(position, rows, _rootItem->columnCount());
//     endInsertRows();

//     return success;
// }

 QModelIndex TreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());

     if(!childItem)
         return QModelIndex();

     TreeItem *parentItem = childItem->parent();

     if (parentItem == _rootItem)
         return QModelIndex();

     return createIndex(parentItem->childCount(), 0, parentItem);
 }

 bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success = true;

     beginRemoveColumns(parent, position, position + columns - 1);
//     success = _rootItem->removeColumns(position, columns);
     endRemoveColumns();

//     if (_rootItem->columnCount() == 0)
//         removeRows(0, rowCount());

     return success;
 }

 bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
     TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());

     beginRemoveRows(parent, position, position + rows - 1);
     bool success = parentItem->removeChildren(position, rows);
     endRemoveRows();

     return success;
 }

 int TreeModel::rowCount(const QModelIndex &parent) const
 {
//     qDebug() << parent.isValid();
//     if(!parent.isValid())
//     {
//         return _rootItem->childCount();
//     }
     TreeItem *parentItem;
     if (parent.isValid())
     {
         parentItem = static_cast<TreeItem *>(parent.internalPointer());
     }
     else
     {
         parentItem = _rootItem;
     }

     if(parentItem == 0)
     {
         return 0;
     }

     return parentItem->childCount();
 }

 bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                         int role)
 {
     if (role != Qt::EditRole)
         return false;

     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     QMap<QString, QVariant>* data = item->data();
     data->insert("value", value);
     bool result = item->updateData(data);
     if (result)
     {
         emit dataChanged(index, index);
     }
//     qDebug() << "write new data to db = " << _db_manager->set((data);

     return result;
//     return false;
 }

 bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                               const QVariant &value, int role)
 {
     if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
         return false;

     bool result = true;// = _rootItem->setData(section, value);

     if (result)
         emit headerDataChanged(orientation, section, section);

     return result;
 }


 void TreeModel::_refreshModel()
 {
     int index = _rootItem->childCount();

     for (int i = 0; i < _rootItem->childCount(); i++)
     {
         index += _rootItem->child(i)->childCount();

     }

     emit dataChanged(createIndex(0,0), createIndex(index,columnCount()-1));
 }


 bool TreeModel::addChild(QMap<QString, QVariant> *data, int role)
 {
//     qDebug() << data->value("id");
     int id = data->value("id").toInt();
     int pid = data->value("pid").toInt();
     TreeItem *item;
     QMap<QString, QVariant>* item_data;
     TreeItem* parent_item;
//     if(pid == 0)
//     {
         parent_item = _rootItem;
//     }
//     else
//     {
         for (int i = 0; i < _rootItem->childCount(); i++)
         {
             item = _rootItem->child(i);
             item_data = item->data();
             if(item_data->value("id").toInt() == pid)
             {
                 parent_item = item;
                 break;
             }
         }
//     }

     for (int i = 0; i < parent_item->childCount(); i++)
     {
         item = parent_item->child(i);
         item_data = item->data();
//         qDebug() << i << item_data->value("id").toInt() << id;
//         qDebug() << i << item_data->value("pid").toInt() << pid;

         if(item_data->value("id").toInt() == id)
         {
             if(item_data->value("pid").toInt() == pid)
             {
                 item->updateData(data);
                 _refreshModel();
//                 qDebug() << "Here";
                 return false;
             }
         }
     }
//     qDebug() << "Exit";

     emit layoutAboutToBeChanged();
//     if(data->value("value").toString() == "")
//     {
         item = new TreeItem(data, parent_item);
         parent_item->appendChild(item);
//     }
//     else
//     {
//         TreeItem* item_ch = new TreeItem(data, item);
//         parent_item->appendChild(item_ch);
//     }
//     _refreshModel();
     emit layoutChanged();
//     qDebug() << "model changed";
     return true;
 }

 void TreeModel::removeChild(int pid, int id, int role)
 {
//     int id = data->value("id").toInt();
//     int pid = data->value("pid").toInt();
     TreeItem *item;
     QMap<QString, QVariant>* item_data;
     TreeItem* parent_item;
     if(pid == 0)
     {
         parent_item = _rootItem;
     }
     else
     {
         for (int i = 0; i < _rootItem->childCount(); i++)
         {
             item = _rootItem->child(i);
             item_data = item->data();
             if(item_data->value("id").toInt() == pid)
             {
                 parent_item = item;
                 break;
             }
         }
     }

     for (int i = 0; i < parent_item->childCount(); i++)
     {
         item = parent_item->child(i);
         item_data = item->data();
         if(item_data->value("id").toInt() == id)
         {
             emit layoutAboutToBeChanged();
             parent_item->removeChild(item);

             _refreshModel();

             emit layoutChanged();
             return;

         }
     }
 }

// void TreeModel::fill_model(int pid)
// {
//     this->reset();
//     delete _rootItem;

//     _rootItem = new TreeItem(NULL);
//     QMap<QString, QVariant>* data;
//     int i = 0;
//     do
//     {
//         data = _db_manager->get(i++, pid);
//         if(data->count() == 0)
//             break;
//         addChild(data, Qt::DisplayRole);
//     }while(data->count());
// }
