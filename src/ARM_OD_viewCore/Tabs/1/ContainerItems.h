#ifndef CONTAINERITEMS_H
#define CONTAINERITEMS_H

#include <QMap>
#include "MainTreeItem.h"

class ContainerItems
{
public:
    ContainerItems();
    ~ContainerItems();

private:
    int _pid;
    MainTreeItem    *_parent;
    QMap<int, MainTreeItem *>    _map_items;

public:
    void set(int id, MainTreeItem *item);
    MainTreeItem *get(int id);
//    void set_pid(int pid);
    void set_parent(int pid, MainTreeItem *parent);
    MainTreeItem *get_parent();
};

#endif // CONTAINERITEMS_H
