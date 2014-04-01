#include "MainTreeItem.h"
#include "MainTreeModelManager.h"

MainTreeModelManager::MainTreeModelManager(const QStringList &headers, QObject *parent)
     : QAbstractItemModel(parent)
	 , _id(0)
 {
     QVector<QVariant> rootHeaders;
     foreach (QString header, headers)
     {
         rootHeaders << header;
     }

     _rootItem = new MainTreeItem(rootHeaders);
 }

 MainTreeModelManager::~MainTreeModelManager()
 {
     delete _rootItem;
 }

// void MainTreeModelManager::set_exchanger(IExchangeMainTreeItem *exchanger)
// {
//     _exchanger = exchanger;

//     _rootItem->setExchanger(exchanger);
// }

// /// set id of punkt
// void MainTreeModelManager::set_id(unsigned int id)
// {
//     _id = id;
//     _rootItem->set_id(_id);
// }

 int MainTreeModelManager::columnCount(const QModelIndex & /* parent */) const
 {
     return _rootItem->columnCount();
 }

 QVariant MainTreeModelManager::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
     {
         return QVariant();
     }

     int col = index.column();
     MainTreeItem *item = static_cast<MainTreeItem *>(index.internalPointer());
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
                 return item->data(0);
             }
            else if(col == 1)
             {
                return item->data(1);
             }
            else
            {
                return QVariant();
            }
            break;
        }

        case Qt::CheckStateRole:
        {
             if(!item->isCheckable(col))
             {
                 return QVariant();
             }

            if(col == 1)
            {
                 QModelIndex ind = index.sibling(index.row(), 0);
                 if(ind.data().isNull())
                 {
                     return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);
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
                     return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);
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
                return item->data(0);
            }
            else if(col == 1)
            {
                return item->data(1);
            }
            else
            {
                return QVariant();
            }
            qDebug() << "EDITROLE!";
        break;
         case Qt::BackgroundColorRole:
              return QColor(Qt::red);
         break;

         case Qt::UserRole+1:
         if(!item)
         {
             return QVariant();
         }
         return item->data(1);
         break;
    }
    return QVariant();
 }

 Qt::ItemFlags MainTreeModelManager::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

     //if(!index.data(Qt::DisplayRole).isNull())

//         flags |= Qt::ItemIsUserCheckable;

         MainTreeItem *item = this->getItem(index);
         ///only 1 column will be editable
         if(item->isEditable(index.column()) == true)
         {
            flags |= Qt::ItemIsEditable;
         }

         if(item->isCheckable(index.column()) == true)
         {
//            flags |= Qt::ItemIsUserCheckable;
         }

     return flags;
 }

 MainTreeItem *MainTreeModelManager::getItem(const QModelIndex &index) const
 {
     if (index.isValid()) {
         MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
         if (item) return item;
     }
     return _rootItem;
 }

 bool MainTreeModelManager::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
 {
     bool b = true;
     for (QMap<int, QVariant>::ConstIterator it = roles.begin(); it != roles.end(); ++it)
         b = b && setData(index, it.value(), it.key());
     return b;
 }

// QMap<int, QVariant> MainTreeModelManager::itemData(const QModelIndex &index) const
// {

// }

 QVariant MainTreeModelManager::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return _rootItem->data(section);

     return QVariant();
 }

 QModelIndex MainTreeModelManager::index(int row, int column, const QModelIndex &parent) const
 {
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     MainTreeItem *parentItem = getItem(parent);

     MainTreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 bool MainTreeModelManager::insertColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success;

     beginInsertColumns(parent, position, position + columns - 1);
     success = _rootItem->insertColumns(position, columns);
     endInsertColumns();

     return success;
 }

 bool MainTreeModelManager::insertRows(int position, int rows, const QModelIndex &parent)
 {
     MainTreeItem *parentItem = getItem(parent);
     bool success;

     beginInsertRows(parent, position, position + rows - 1);
     success = parentItem->insertChildren(position, rows, _rootItem->columnCount());
     endInsertRows();

     return success;
 }

 QModelIndex MainTreeModelManager::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     MainTreeItem *childItem = getItem(index);
     MainTreeItem *parentItem = childItem->parent();

     if (parentItem == _rootItem)
         return QModelIndex();

     return createIndex(parentItem->childNumber(), 0, parentItem);
 }

 bool MainTreeModelManager::removeColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success;

     beginRemoveColumns(parent, position, position + columns - 1);
     success = _rootItem->removeColumns(position, columns);
     endRemoveColumns();

     if (_rootItem->columnCount() == 0)
         removeRows(0, rowCount());

     return success;
 }

 bool MainTreeModelManager::removeRows(int position, int rows, const QModelIndex &parent)
 {
     MainTreeItem *parentItem = getItem(parent);

     beginRemoveRows(parent, position, position + rows - 1);
     bool success = parentItem->removeChildren(position, rows);
     endRemoveRows();

     return success;
 }

 int MainTreeModelManager::rowCount(const QModelIndex &parent) const
 {
     MainTreeItem *parentItem = getItem(parent);

     return parentItem->childCount();
 }

 bool MainTreeModelManager::setData(const QModelIndex &index, const QVariant &value,
                         int role)
 {
//     if (role != Qt::EditRole)
//         return false;

     MainTreeItem *item = getItem(index);
     bool result = item->setData(index.column(), value);

     if (result)
     {
         emit dataChanged(index, index);
     }

     return result;
 }

 bool MainTreeModelManager::setHeaderData(int section, Qt::Orientation orientation,
                               const QVariant &value, int role)
 {
     if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
         return false;

     bool result = _rootItem->setData(section, value);

     if (result)
         emit headerDataChanged(orientation, section, section);

     return result;
 }

// bool MainTreeModelManager::addChild(const QModelIndex &index, const QVariant &value, int role)
// {
//     QVector<QVariant> rootData1;
//     rootData1 << QVariant() << value;
//     MainTreeItem *new_item = getItem(index);
//     addItemChild(new_item, new_item->childCount(), 1, new_item->columnCount(), rootData1, role);
//     emit dataChanged(index, index);
//     return true;
// }

// ///added id and property into 2 columns
// bool MainTreeModelManager::addChildProperty(const QModelIndex &index, const QVariant &ID, const QVariant &Property, int role)
// {
//     QVector<QVariant> rootData1;
//     rootData1 << ID << Property;
//     MainTreeItem *new_item = getItem(index);

//     addItemChild(new_item, new_item->childCount(), 1, new_item->columnCount(), rootData1, role);
//     emit dataChanged(index, index);
//     if(role == Qt::EditRole)
//     {
//         new_item->child(new_item->childCount()-1)->setEditable(1, true);
//     }
//     if(role == Qt::CheckStateRole)
//     {
//         new_item->child(new_item->childCount()-1)->setCheckable(index.column(), true);
//     }
//     return true;
// }

// void MainTreeModelManager::addItemChild(MainTreeItem *parent, int position, int count, int columns, QVector<QVariant> columnData, int role)
// {
//     parent->insertChildren(position, count, columns);
//     for (int column = 0; column < columnData.size(); ++column)
//     {
//         parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
//         if(role == Qt::CheckStateRole)
//            parent->child(parent->childCount() - 1)->setCheckable(column, true);
//     }
// }

// void MainTreeModelManager::setupModelData(const QStringList &lines, MainTreeItem *parent)
// {
//     QList<MainTreeItem*> parents;
//     QList<int> indentations;
//     parents << parent;
//     indentations << 0;

//     int number = 0;

//     while (number < lines.count()) {
//         int position = 0;
//         while (position < lines[number].length()) {
//             if (lines[number].mid(position, 1) != " ")
//                 break;
//             position++;
//         }

//         QString lineData = lines[number].mid(position).trimmed();

//         if (!lineData.isEmpty()) {
//             // Read the column data from the rest of the line.
//             QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
//             QVector<QVariant> columnData;
//             for (int column = 0; column < columnStrings.count(); ++column)
//                 columnData << columnStrings[column];

//             if (position > indentations.last()) {
//                 // The last child of the current parent is now the new parent
//                 // unless the current parent has no children.

//                 if (parents.last()->childCount() > 0) {
//                     parents << parents.last()->child(parents.last()->childCount()-1);
//                     indentations << position;
//                 }
//             } else {
//                 while (position < indentations.last() && parents.count() > 0) {
//                     parents.pop_back();
//                     indentations.pop_back();
//                 }
//             }

//             // Append a new item to the current parent's list of children.
//             parents.last()->appendChild(new MainTreeItem(columnData, parents.last()/*, _exchanger*/));
////             QVariant var = _exchanger->get_data();
////             parents.last()->child(parents.last()->childCount() - 1)->setExchanger(_exchanger);
//         }
//         number++;
//     }
// }

 MainTreeItem *MainTreeModelManager::get_root_item()
 {
     return _rootItem;
 }

// void MainTreeModelManager::updateItem(QString name, QVariant value)
// {
//     QModelIndexList items = this->match(this->index(0, 0), Qt::DisplayRole, QVariant::fromValue(name), 2, Qt::MatchRecursive);

//     qDebug() << "ITEMS COUNT = " << items.count();
//     if(items.count() > 0)
//     {
//         QModelIndex index = items.last();
//         QModelIndex index2 = index.sibling(index.row(), 1);
//         this->setData(index2, value, Qt::DisplayRole);
//     }
// }

// QModelIndex MainTreeModelManager::get_toot_index(QString id_name)
// {
//     QModelIndex index;
//     QModelIndexList items = this->match(this->index(0, 0), Qt::DisplayRole, QVariant::fromValue(id_name));

//     qDebug() << "ITEMS COUNT = " << items.count();
//     if(items.count() > 0)
//     {
//         index = items.last();
//     }
//     return index;
// }

 void MainTreeModelManager::refreshModel()
 {
     this->reset();
 }
