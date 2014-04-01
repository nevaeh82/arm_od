#include "ContainerItems.h"
#include <QDebug>

ContainerItems::ContainerItems()
{
    _pid = 0;
    _parent = NULL;
}

ContainerItems::~ContainerItems()
{
    qDebug() << "Delete Container";
}

void ContainerItems::set(int id, MainTreeItem *item)
{
    _map_items.insert(id, item);
}

MainTreeItem *ContainerItems::get(int id)
{
    if(_map_items.contains(id))
        return _map_items.value(id);
    return NULL;
}

void ContainerItems::set_parent(int pid, MainTreeItem *parent)
{
    _pid = pid;
    _parent = parent;
}

MainTreeItem *ContainerItems::get_parent()
{
    return _parent;
}
