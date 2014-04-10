#include "DBManager.h"

DBManager::DBManager(ITabManager *tab_manager)
{
	m_tabManager = tab_manager;
	m_dbController = new DBController();
}

DBManager::~DBManager()
{
	delete m_dbController;
}

int DBManager::set(int group, QMap<QString, QVariant>* data)
{
	switch(group)
	{
	case 0:
//		m_model->addChild(data, Qt::EditRole);
		m_dbController->setFriendBpla(data);

		break;
	case 1:
//		m_model->addChild(data, Qt::EditRole);
		m_dbController->setEnemyBpla(data);
		break;

	default:
		return -1;
		break;
	}
	return 0;
}

int DBManager::setProperty(int group, QMap<QString, QVariant> *data)
{
	QVector<QMap<QString, QVariant> >* vec;
	switch(group)
	{
	case 0:
		m_dbController->setFriendBplaProperty(data);

		vec = m_dbController->getFriendBplaProperty(data->value("pid").toInt());
		for(int i = 0; i < vec->size(); ++i)
		{
			if(vec->at(i).value("name").toString() == data->value("name").toString())
			{
				if(vec->at(i).value("pid").toString() == data->value("pid").toString())
				{
					data->insert("id", vec->at(i).value("id").toInt());
//					m_model->addChild(data, Qt::EditRole);
				}
			}
		}

		break;
	case 1:
		m_dbController->setEnemyBplaProperty(data);
		vec = m_dbController->getEnemyBplaPropert(data->value("pid").toInt());
		for(int i = 0; i < vec->size(); ++i)
		{
			if(vec->at(i).value("name").toString() == data->value("name").toString())
			{
				if(vec->at(i).value("pid").toString() == data->value("pid").toString())
				{
					data->insert("id", vec->at(i).value("id").toInt());
//					m_model->addChild(data, Qt::EditRole);
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
		return m_dbController->getFriendBplaProperty(id);
		break;
	case 1:
		return m_dbController->getEnemyBplaPropert(id);
		break;
	default:
		break;
	}

//            _db_cache.get(pid, id);//_db_controller->get_data(id, pid);
	return NULL;
}

QVector<int> DBManager::get(int group)
{
	switch(group)
	{
	case 0:
		return m_dbController->getFriendBplaList();
		break;
	case 1:
		return m_dbController->getEnemyBplaList();
		break;
	default:
		break;
	}

//    return _db_cache.get(pid);//_db_controller->get_data(pid);
	return QVector<int>();
}

void DBManager::removeItem(int id, int group)
{
	Q_UNUSED( id );
	Q_UNUSED( group );
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

QMap<QString, QVariant> *DBManager::getFriendBplaFields(int id)
{
	return m_dbController->getFriendBplaFields(id);
}

QMap<QString, QVariant> *DBManager::getEnemyBplaFields(int id)
{
	return m_dbController->getEnemyBplaFields(id);
}

void DBManager::deleteFriendBpla(int id)
{
//	m_model->removeChild(0, id, 0);
	m_dbController->deleteFriendBpla(id);
}

void DBManager::deleteEnemyBpla(int id)
{
//	m_model->removeChild(0, id, 0);

	m_dbController->deleteEnemyBpla(id);
}

void DBManager::deleteFriendBplaProperty(int pid, int id)
{
//	m_model->removeChild(pid, id, 0);
	m_dbController->deleteFriendBplaProperty(pid, id);
}

void DBManager::deleteEnemyBplaProperty(int pid, int id)
{
//	m_model->removeChild(pid, id, 0);
	m_dbController->deleteEnemyBplaProperty(pid, id);
}
