#include "DbControllerBase.h"

DbControllerBase::DbControllerBase(QString connectionName, QString dbType, QObject *parent) :
	QObject(parent)
{
	if(QSqlDatabase::contains(connectionName))
	{
		m_db =  QSqlDatabase::database(connectionName);
	}
	else
	{
		m_db = QSqlDatabase::addDatabase(dbType, connectionName);
	}
}

DbControllerBase::~DbControllerBase()
{
	if(m_db.isOpen()){
		m_db.close();
	}
}

bool DbControllerBase::connectToDB(const DBConnectionStruct& parameters)
{
	if(m_db.isOpen()){
		m_db.close();
	}

	m_db.setHostName(parameters.host);
	m_db.setPort(parameters.port);
	m_db.setPassword(parameters.password);
	m_db.setUserName(parameters.login);
	m_db.setDatabaseName(parameters.dbName);

	if (!m_db.open()) {
		log_error(m_db.lastError().databaseText() + "\n" + m_db.lastError().driverText());
		return false;
	}

	return true;
}

void DbControllerBase::disconnectFromDb()
{
	if (m_db.isOpen()) {
		m_db.close();
	}
}
