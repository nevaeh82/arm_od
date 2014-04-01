#include "MainTabView.h"
#include "MainTreeItem.h"

MainTabView::MainTabView(QWidget *parent)
{
    QStringList headers;
    headers << tr("") << tr("");
    _model = new MainTreeModelManager(headers);

    _tree_view = new QTreeView();
    _tree_view->setSelectionMode(QAbstractItemView::SingleSelection);
    _tree_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    _tree_view->setModel(_model);
    _tree_view->expandAll();

    _tree_view->setWindowTitle(QObject::tr("MainModel"));
    _tree_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _delegate = new BLAWidgetDelegate();
    _tree_view->setItemDelegate(_delegate);



//    for(int column = 0; column < _model->columnCount(); ++column)
//    {
//        _tree_view->setColumnWidth(column, 50);
//        _tree_view->resizeColumnToContents(column);
//        qDebug() << _tree_view->columnWidth(column);
//    }

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    hbox->addWidget(_tree_view, Qt::AlignJustify);
    this->setLayout(hbox);

//    this->installEventFilter(this);
}

//void MainTabView::insertRootChild(QString name, int role)
//{
//    QVector<QVariant> rootData;
//    rootData << name;
//    _model->addItemChild(_model->get_root_item(), _model->get_root_item()->childCount(), 1, _model->get_root_item()->columnCount(), rootData, role);
////    _tree_view->resizeColumnToContents(0);
////    _tree_view->resizeColumnToContents(1);
//}

//void MainTabView::insertRootChild2Column(QString parent_name, QString name, int role)
//{
//    qDebug() << parent_name;

//    QVector<QVariant> rootData;
//    rootData << name;

//    QAbstractItemModel *model = _tree_view->model();
//    QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(parent_name), 2, Qt::MatchRecursive);

//    qDebug() << "COUNT" << items.count();
//    if(items.count() > 0)
//    {
//        QModelIndex index = items.last();

//        MainTreeItem *tr = _model->getItem(index);

//        _model->addChildProperty(index, "", name, role);

//        _tree_view->collapse(index);
//        _tree_view->expand(index);
//        qDebug() << parent_name << "END";
//    }
////    _tree_view->resizeColumnToContents(0);
////    _tree_view->resizeColumnToContents(1);
//}

QTreeView *MainTabView::get_tree()
{
    return _tree_view;
}

/////add childs
//void MainTabView::insertChild(QString value, QString parent_name)
//{
//    QAbstractItemModel *model = _tree_view->model();
//    QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(parent_name)/*, 2, Qt::MatchRecursive*/);

//    if(items.count() > 0)
//    {
//        QModelIndex index = items.last();
//        _model->addChild(index, value, Qt::CheckStateRole);
//        _tree_view->expand(index);
//    }
////    _tree_view->resizeColumnToContents(0);
////    _tree_view->resizeColumnToContents(1);
//}

// bool MainTabView::insertColumn(const QModelIndex &parent)
// {
//     QAbstractItemModel *model = _tree_view->model();
//     int column = _tree_view->selectionModel()->currentIndex().column();

//     // Insert a column in the parent item.
//     bool changed = model->insertColumn(column + 1, parent);
//     if (changed)
//         model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"),
//                              Qt::DisplayRole);

//     updateActions();

////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
//     return changed;
// }

// void MainTabView::insertRow()
// {
//     QModelIndex index = _tree_view->selectionModel()->currentIndex();
//     QAbstractItemModel *model = _tree_view->model();

//     if (!model->insertRow(index.row()+1, index.parent()))
//         return;

//     updateActions();

//     for (int column = 0; column < model->columnCount(index.parent()); ++column) {
//         QModelIndex child = model->index(index.row()+1, column, index.parent());
//         model->setData(child, QVariant("[No data]"), Qt::DisplayRole);
//     }
////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
// }

// bool MainTabView::removeColumn(const QModelIndex &parent)
// {
//     QAbstractItemModel *model = _tree_view->model();
//     int column = _tree_view->selectionModel()->currentIndex().column();

//     // Insert columns in each child of the parent item.
//     bool changed = model->removeColumn(column, parent);

//     if (!parent.isValid() && changed)
//         updateActions();

//     return changed;
// }

// void MainTabView::removeRow()
// {
//     QModelIndex index = _tree_view->selectionModel()->currentIndex();
//     QAbstractItemModel *model = _tree_view->model();
//     if (model->removeRow(index.row(), index.parent()))
//         updateActions();
// }

// void MainTabView::updateActions()
// {
//     bool hasSelection = !_tree_view->selectionModel()->selection().isEmpty();
//     bool hasCurrent = _tree_view->selectionModel()->currentIndex().isValid();
//     if (hasCurrent) {
//         _tree_view->closePersistentEditor(_tree_view->selectionModel()->currentIndex());
//         int row = _tree_view->selectionModel()->currentIndex().row();
//         int column = _tree_view->selectionModel()->currentIndex().column();
//     }
////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
// }

 void MainTabView::resizeEvent(QResizeEvent *event)
 {
     emit signalResize(this->size().width(), this->size().height());
 }

 void MainTabView::setModel(MainTreeModelManager *model)
 {
     _model = model;
     _tree_view->setModel(_model);
 }

// ///update item (change value)
// void MainTabView::updateItem(QModelIndex start_index, QString value, QString parent_name)
// {
//     QAbstractItemModel *model = _tree_view->model();
//     QModelIndexList items = model->match(start_index, Qt::DisplayRole, QVariant::fromValue(parent_name), 2, Qt::MatchRecursive);

//     qDebug() << "ITEMS COUNT = " << items.count();
//     if(items.count() > 0)
//     {
//         QModelIndex index = items.last();
//         QModelIndex index2 = index.sibling(index.row(), 1);
//         qDebug() << _model->setData(index2, value, Qt::DisplayRole);
//         _tree_view->collapse(index);
//         _tree_view->expand(index);
//     }
////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
// }

// ///update item (change value)
// void MainTabView::insertRowProperties(QString parent_name, QString ID, QString property)
// {
//     qDebug() << parent_name;
//     QAbstractItemModel *model = _tree_view->model();
////     qDebug() << "COUNT" << model->rowCount();
//     QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(parent_name)/*, 2, Qt::MatchRecursive*/);

//     qDebug() << "COUNT" << items.count();
//     if(items.count() > 0)
//     {
//         qDebug() << parent_name << "gdefs";
//         QModelIndex index = items.last();
////         qDebug() << _model->setData(index, value, Qt::CheckStateRole);
//         _model->addChildProperty(index, ID, property, Qt::CheckStateRole);
//         _tree_view->collapse(index);
//         _tree_view->expand(index);
//         qDebug() << parent_name << "END";
//     }
////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
// }

// ///check is item present
// bool MainTabView::itemPresent(QString id, QString name, QModelIndex &index)
// {
//     QAbstractItemModel *model = _tree_view->model();
//     QModelIndexList items_id = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(id)/*, 2, Qt::MatchRecursive*/);
//     qDebug() << "items_id.count()" << items_id.count();
//     if(items_id.count() > 0)
//     {
//         qDebug() << "FIND!!!!!" << items_id.last().isValid();
//         QModelIndexList items = model->match(items_id.last(), Qt::DisplayRole, QVariant::fromValue(name), 1, Qt::MatchRecursive);


//         qDebug() << "items.count()" << items.count();
//         if(items.count() > 0)
//         {
//             for (int i = 0; i < items.count(); i++)
//             {
//                 if(items.at(i).parent() == items_id.last())
//                 {
//                     index = items.at(i);
//                     return true;
//                 }
//             }
//         }
////         if(items.count() > 0)
////         {
////             index = items.last();
////             return true;
////         }
//     }
//     return false;
// }

// ///update item settings(change value)
// void MainTabView::insertRowSettingProperties(QString parent_name, QString ID, QString property)
// {
//     qDebug() << parent_name;
//     QAbstractItemModel *model = _tree_view->model();
////     qDebug() << "COUNT" << model->rowCount();
//     QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(parent_name)/*, 2, Qt::MatchRecursive*/);

//     qDebug() << "COUNT" << items.count();
//     if(items.count() > 0)
//     {
//         QModelIndex index = items.last();

//         _model->addChildProperty(index, ID, property, Qt::EditRole);

//         _tree_view->collapse(index);
//         _tree_view->expand(index);
//         qDebug() << parent_name << "END";
//     }
////     _tree_view->resizeColumnToContents(0);
////     _tree_view->resizeColumnToContents(1);
// }

// void MainTabView::updateState(QString name, bool state)
// {
//     QAbstractItemModel *model = _tree_view->model();
////     qDebug() << "COUNT" << model->rowCount();
//     QModelIndexList items = model->match(model->index(0, 0), Qt::CheckStateRole, QVariant::fromValue(name), 2, Qt::MatchRecursive);

//     qDebug() << "COUNT" << items.count() << name << _model->rowCount();
//     if(items.count() > 0)
//     {
//         QModelIndex index = items.last();

//         _model->getItem(index)->setChecked(state);

//         _tree_view->collapse(index);
//         _tree_view->expand(index);

////         qDebug() << parent_name << "END";
//     }
// }
