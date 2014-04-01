#include "Controller.h"

Controller::Controller():
    _id(0)
{
//    _map_root_items = new QMap<int, MainTreeItem *>;
    _map_items = new QMap<int, ContainerItems *>;
//    _map_st_items = new QMap<int, QVector<QMap<int, MainTreeItem *> > >;
    _db_man.set_db("my_db.sqlite");
    _db_man.set_table("Stations");
    QStringList headers_sp;
    headers_sp << "Свойство" << "Значение";
    _model = new MainTreeModelManager(headers_sp);
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(slotChangeValueForDB(QModelIndex,QModelIndex)));
}

Controller::~Controller()
{
    delete _model;
}

/// set interface for exchange with tcp clients
void Controller::set_exchanger(IExchangeMainTreeItem *exchanger)
{
    _exchanger = exchanger;
}

void Controller::slotChangeValueForDB(const QModelIndex &index1, const QModelIndex &index2)
{
    qDebug() << "Here";
    QModelIndex ind = index1.sibling(index1.row(), 0);
    qDebug() << ind.data();
    qDebug() << index1.data();
    qDebug() << ind.parent().data();
    update_value_in_db(index1);

    _exchanger->push_data(ind.parent().data(), ind.data(), index1.data());
}

void Controller::update_value_in_db(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0);
    _db_man.update("SpectrumProperties", ind.data().toString(), index.data().toString(), index.parent().data().toString());
}

void Controller::update_property(int pid, int id,/*QString name,*/ QVariant value)
{
    ContainerItems *c_items = _map_items->value(pid);
    MainTreeItem *item = c_items->get(id);
    item->setData(1, value);
//    _db_man.update("SpectrumProperties", item->data(0).toString(), value.toString(), item->parent()->data(0).toString());

    if(item->isEditable(1))
        _exchanger->push_data(item->parent()->data(0), item->data(0), value);
    _model->refreshModel();
}

MainTreeModelManager *Controller::get_model()
{
    return _model;
}

void Controller::reset_model(int id, int id_view)
{
    _id = id;
    disconnect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(slotChangeValueForDB(QModelIndex,QModelIndex)));
    delete _model;
    QStringList headers;
    switch(id_view)
    {
    case 0:
        headers << "Свойство" << "Значение";
        break;
    case 1:
        headers << "Свойство" << "Значение";
        break;
    case 2:
        headers << "Свойство" << "Значение";
        break;
    case 3:
        headers << "Свойство" << "Значение";
        break;
    default:
        break;
    }

    _model = new MainTreeModelManager(headers);
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(slotChangeValueForDB(QModelIndex,QModelIndex)));
}

QList<QMap<QString, QVariant> > Controller::requestdata(QStringList list)
{
    QList<QMap<QString, QVariant> > list_of_map = _db_man.get_data(list);

    return list_of_map;
}

QList<QMap<QString, QVariant> > Controller::requestdata(QStringList list, int id)
{
    QList<QMap<QString, QVariant> > list_of_map = _db_man.get_data_cur_station(list, _id);

    return list_of_map;
}

void Controller::request_all_stations()
{
    QList<QMap<QString, QVariant> > list_of_map;
    QStringList list;
    list.push_back("level");
    list.push_back("pid");
    list.push_back("id");
    list.push_back("name");
    list.push_back("property");
    list.push_back("state");
    list.push_back("editable");
    _db_man.set_table("SpectrumProperties");
    list_of_map = requestdata(list);
    _fill_model(list_of_map);
}

void Controller::_fill_model(QList<QMap<QString, QVariant> > list_of_map)
{
    QList<QMap<QString, QVariant> >::const_iterator it;
    for(it = list_of_map.begin(); it != list_of_map.end(); ++it)
    {
        QMap<QString, QVariant> map = *it;
        qDebug() << *it;

        if(map.value("pid").toInt() == 0)
        {
            ContainerItems *c_item;
            if(!_map_items->contains(map.value("id").toInt()))
            {
                c_item = new ContainerItems();
                c_item->set_parent(map.value("pid").toInt(), _model->get_root_item());
                _map_items->insert(map.value("id").toInt(), c_item);

            }
            c_item = _map_items->value(map.value("id").toInt());
            QVector<QVariant> data;
            data.push_back(map.value("name"));
            MainTreeItem *rootItem = c_item->get_parent();
            MainTreeItem *newItem = new MainTreeItem(data, rootItem);
            rootItem->appendChild(newItem);
            c_item->set(map.value("id").toInt(), newItem);
        }
        else
        {
            _db_man.set_table("Stations");
            QStringList list1;
            list1.push_back("level");
            list1.push_back("pid");
            list1.push_back("id");
            list1.push_back("name");
            list1.push_back("state");
            QList<QMap<QString, QVariant> > list_of_map = requestdata(list1);
            QList<QMap<QString, QVariant> >::const_iterator itt;
            for(itt = list_of_map.begin(); itt != list_of_map.end(); ++itt)
            {
                QMap<QString, QVariant> map1 = *itt;
                qDebug() << *itt;

                if(map1.value("id") != map.value("pid"))
                  continue;

                if(_map_items->contains(map1.value("id").toInt()))
                    continue;
                QVector<QVariant> data;
                data << map1.value("name");
                ContainerItems *c_item;
                if(!_map_items->contains(map.value("pid").toInt()))
                {
                    c_item = new ContainerItems();
//                    c_item->set_pid(map.value("pid"));
                    c_item->set_parent(map1.value("pid").toInt(), _model->get_root_item());
                    _map_items->insert(map1.value("pid").toInt(), c_item);
                }
                c_item = _map_items->value(map1.value("pid").toInt());
                MainTreeItem *rootItem = c_item->get_parent();
                MainTreeItem *newItem = new MainTreeItem(data, rootItem);
                rootItem->appendChild(newItem);
                c_item->set(map1.value("id").toInt(), newItem);
                newItem->setCheckable(0, true);
                if(map1.value("state") == 1)
                {
                    newItem->setChecked(true);
                }
                else
                {
                    newItem->setChecked(false);
                }
            }

            ContainerItems *c_item;
            if(!_map_items->contains(map.value("pid").toInt()))
            {
                c_item = new ContainerItems();
                MainTreeItem *parentItem = _map_items->value(0)->get(map.value("pid").toInt());
                c_item->set_parent(map.value("pid").toInt(), parentItem);
                _map_items->insert(map.value("pid").toInt(), c_item);
            }
            c_item = _map_items->value(map.value("pid").toInt());
            QVector<QVariant> data;
            data << map.value("name") << map.value("property");
            MainTreeItem *rootItem = c_item->get_parent();
            qDebug() << "PARENT = " << c_item->get_parent()->data(0);
            MainTreeItem *newItem = new MainTreeItem(data, rootItem);
            rootItem->appendChild(newItem);
            c_item->set(map.value("id").toInt(), newItem);

            if(map.value("editable") == 1)
            {
                newItem->setEditable(1, true);
            }
        }
    }
}

void Controller::request_current_station()
{
    QList<QMap<QString, QVariant> > list_of_map;
    QStringList list;
    list.push_back("level");
    list.push_back("pid");
    list.push_back("id");
    list.push_back("name");
    list.push_back("property");
    list.push_back("state");
    list.push_back("editable");
    _db_man.set_table("SpectrumProperties");
    list_of_map = requestdata(list, _id);
    _fill_model(list_of_map);
}

void Controller::setupData()
{
//    delete _map_root_items;
    delete _map_items;
//    _map_root_items = new QMap<int, MainTreeItem *>;
    _map_items = new QMap<int, ContainerItems *>;
    switch(_id)
    {
    case 0:
        request_all_stations();
        break;
    default:
        request_current_station();
        break;
    }
}
