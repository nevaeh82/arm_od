#include "MainTreeItem.h"
#include <QDebug>

MainTreeItem::MainTreeItem(const QVector<QVariant> &data, MainTreeItem *parent/*, IExchangeMainTreeItem *exchanger*/)
{
    _parentItem = parent;
    _itemData = data;
    _isEditable[0] = false;
    _isEditable[1] = false;
    _isCheckable[0] = false;
    _isCheckable[1] = false;

//    _exchanger = exchanger;

//    if(_parentItem != NULL)
//    {
//        setExchanger(parent->_exchanger);
//    }
}

MainTreeItem::~MainTreeItem()
{
    qDeleteAll(_childItems);
}

//void MainTreeItem::setExchanger(IExchangeMainTreeItem *exchanger)
//{
//    _exchanger = exchanger;
//}

/// set id of punkt
void MainTreeItem::set_id(unsigned int id)
{
    _id = id;
}

MainTreeItem *MainTreeItem::child(int number)
{
    return _childItems.value(number);
}

int MainTreeItem::childCount() const
{
    return _childItems.count();
}

int MainTreeItem::childNumber() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<MainTreeItem*>(this));

    return 0;
}

int MainTreeItem::columnCount() const
{
    return _itemData.count();
}

QVariant MainTreeItem::data(int column) const
{
    return _itemData.value(column);
}

bool MainTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > _childItems.size())
    {
        return false;
    }

    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        MainTreeItem *item = new MainTreeItem(data, this/*, _exchanger*/);
        item->set_id(_id);
//        QVariant var = _exchanger->get_data();
//        item->setChecked(true);
        _childItems.insert(position, item);
    }

    return true;
}

void MainTreeItem::appendChild(MainTreeItem *item)
{
    item->set_id(_id);
    _childItems.append(item);
}

bool MainTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > _itemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        _itemData.insert(position, QVariant());
    }

    foreach (MainTreeItem *child, _childItems)
    {
        child->insertColumns(position, columns);
    }

    return true;
}

MainTreeItem *MainTreeItem::parent()
{
    return _parentItem;
}

bool MainTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
    {
        return false;
    }

    for (int row = 0; row < count; ++row)
    {
        delete _childItems.takeAt(position);
    }

    return true;
}

bool MainTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > _itemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        _itemData.remove(position);
    }

    foreach (MainTreeItem *child, _childItems)
    {
        child->removeColumns(position, columns);
    }

    return true;
}

bool MainTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= _itemData.size())
    {
        return false;
    }

    _itemData[column] = value;
    if(_isEditable[column])
    {
//        _exchanger->push_data(_itemData[0], value);
    }
    return true;
}

bool MainTreeItem::isChecked() const
{
    return _checked;
}

void MainTreeItem::setChecked(bool set)
{
    _checked = set;
}

void MainTreeItem::setEditable(int column, bool state)
{
    _isEditable[column] = state;
}

bool MainTreeItem::isEditable(int column)
{
    return _isEditable[column];
}

void MainTreeItem::setCheckable(int column, bool state)
{
    _isCheckable[column] = state;
}

bool MainTreeItem::isCheckable(int column)
{
    return _isCheckable[column];
}
