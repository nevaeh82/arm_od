#include "DBManager.h"

DBManager::DBManager(ITabManager *tab_manager)
{
    _tab_manager = tab_manager;
    _db_controller = new DBController();

//    QThread *thread_db_controller = new QThread;
//    qDebug() << "create thread for db controller ";

//    connect(_db_controller, SIGNAL(signalFinished()), thread_db_controller, SLOT(quit()));
//    connect(thread_db_controller, SIGNAL(finished()), thread_db_controller, SLOT(deleteLater()));

//    connect(_db_controller, SIGNAL(signalFinished()), _db_controller, SLOT(deleteLater()));

    connect(this, SIGNAL(signalSetModel(ITreeModel*)), this, SLOT(_slot_set_model(ITreeModel*)));
    connect(this, SIGNAL(signalSet(int,QMap<QString,QVariant>*)), this, SLOT(_slot_set(int,QMap<QString,QVariant>*)));
    connect(this, SIGNAL(signalSetPropertry(int,QMap<QString,QVariant>*)), this, SLOT(_slot_set_property(int,QMap<QString,QVariant>*)));

//    _db_controller->moveToThread(thread_db_controller);
//    thread_db_controller->start();
}

//void DBManager::set_model(ITreeModel* model/*_bla, ITreeModel* model_bpla*/)
//{
//    _model = model;

////    emit signalSetModel(model);
////    _model/*_bla*/ = model/*_bla*/;
////    _model_bpla = model_bpla;
//}

DBManager::~DBManager()
{
    delete _db_controller;
}

int DBManager::set(int group, QMap<QString, QVariant>* data)
{
 /*  emit signalSet(group, data);
    switch(group)
    {
    case 0:
//        _db_cache.set(data);
		_model->addChild(data, Qt::EditRole);
        _db_controller->set_bla(data);

        break;
    case 1:
//        _db_cache.set(data);
		_model->addChild(data, Qt::EditRole);
        _db_controller->set_bpla(data);
        break;
    default:
        return -1;
        break;
	}*/
    return 0;
}

int DBManager::set_property(int group, QMap<QString, QVariant> *data)
{
    //emit signalSetPropertry(group, data);
    QVector<QMap<QString, QVariant> >* vec;
    switch(group)
    {
    case 0:
//        _db_cache.set(data);
        _db_controller->set_bla_property(data);

        vec = _db_controller->get_property_bla(data->value("pid").toInt());
        for(int i = 0; i < vec->size(); ++i)
        {
            if(vec->at(i).value("name").toString() == data->value("name").toString())
            {
                if(vec->at(i).value("pid").toString() == data->value("pid").toString())
                {
                    data->insert("id", vec->at(i).value("id").toInt());
					//_model->addChild(data, Qt::EditRole);
                }
            }
        }
       // _model/*_bla*/->addChild(data, Qt::EditRole);

        break;
    case 1:
//        _db_cache.set(data);
        _db_controller->set_bpla_property(data);
        vec = _db_controller->get_property_bpla(data->value("pid").toInt());
        for(int i = 0; i < vec->size(); ++i)
        {
            if(vec->at(i).value("name").toString() == data->value("name").toString())
            {
                if(vec->at(i).value("pid").toString() == data->value("pid").toString())
                {
                    data->insert("id", vec->at(i).value("id").toInt());
					//_model->addChild(data, Qt::EditRole);
                }
            }
        }


        break;
    default:
        return -1;
        break;
    }
    return 0;
}

QVector<QMap<QString, QVariant> >* DBManager::get(int id, int group)
{
    switch(group)
    {
    case 0:
        return _db_controller->get_property_bla(id);
        break;
    case 1:
        return _db_controller->get_property_bpla(id);
        break;
    default:
        break;
    }

//            _db_cache.get(pid, id);//_db_controller->get_data(id, pid);
}

QVector<int> DBManager::get(int group)
{
    switch(group)
    {
    case 0:
        return _db_controller->get_list_bla();
        break;
    case 1:
        return _db_controller->get_list_bpla();
        break;
    default:
        break;
    }

//    return _db_cache.get(pid);//_db_controller->get_data(pid);
}

void DBManager::removeItem(int id, int group)
{
//    QMap<QString, QVariant>* data = get(id, group);

//    switch(group)
//    {
//    case 0:
//        _model/*_bla*/->removeChild(data, 0);
//        break;
//    case 1:
//        _model/*_bpla*/->removeChild(data, 0);
//        break;
//    default:
//        break;
//    }

    //    _model->removeChild(data, 0);

}

QMap<QString, QVariant> *DBManager::get_bla_fields(int id)
{
    return _db_controller->get_bla_fields(id);
}

QMap<QString, QVariant> *DBManager::get_bpla_fields(int id)
{
    return _db_controller->get_bpla_fields(id);
}

void DBManager::delete_bla(int id)
{
	//_model->removeChild(0, id, 0);

    _db_controller->delete_bla(id);
}

void DBManager::delete_bpla(int id)
{
	//_model->removeChild(0, id, 0);

    _db_controller->delete_bpla(id);
}

void DBManager::delete_bla_property(int pid, int id)
{
	//_model->removeChild(pid, id, 0);
    _db_controller->delete_bla_property(pid, id);
}

void DBManager::delete_bpla_property(int pid, int id)
{
	//_model->removeChild(pid, id, 0);
    _db_controller->delete_bpla_property(pid, id);
}

//void DBManager::_slot_set_model(ITreeModel *model)
//{
//    //_model = model;
//}

void DBManager::_slot_set(int group, QMap<QString, QVariant>* data)
{
    switch(group)
    {
    case 0:
//        _db_cache.set(data);
		//_model/*_bla*/->addChild(data, Qt::EditRole);
        _db_controller->set_bla(data);

        break;
    case 1:
//        _db_cache.set(data);
	   // _model/*_bpla*/->addChild(data, Qt::EditRole);
        _db_controller->set_bpla(data);
        break;
    default:
        return;
        break;
    }
}

void DBManager::_slot_set_property(int group, QMap<QString, QVariant> *data)
{
    QVector<QMap<QString, QVariant> >* vec;
    switch(group)
    {
    case 0:
//        _db_cache.set(data);
        _db_controller->set_bla_property(data);

        vec = _db_controller->get_property_bla(data->value("pid").toInt());
        for(int i = 0; i < vec->size(); ++i)
        {
            if(vec->at(i).value("name").toString() == data->value("name").toString())
            {
                if(vec->at(i).value("pid").toString() == data->value("pid").toString())
                {
                    data->insert("id", vec->at(i).value("id").toInt());
					//_model->addChild(data, Qt::EditRole);
                }
            }
        }
       // _model/*_bla*/->addChild(data, Qt::EditRole);

        break;
    case 1:
//        _db_cache.set(data);
        _db_controller->set_bpla_property(data);
        vec = _db_controller->get_property_bpla(data->value("pid").toInt());
        for(int i = 0; i < vec->size(); ++i)
        {
            if(vec->at(i).value("name").toString() == data->value("name").toString())
            {
                if(vec->at(i).value("pid").toString() == data->value("pid").toString())
                {
                    data->insert("id", vec->at(i).value("id").toInt());
					//_model->addChild(data, Qt::EditRole);
                }
            }
        }


        break;
    default:
        return;
        break;
    }
}


