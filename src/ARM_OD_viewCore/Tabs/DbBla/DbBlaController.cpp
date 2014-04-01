#include "DbBlaController.h"

DbBlaController::DbBlaController(QObject *parent) :
	QObject(parent)
{
	if(QSqlDatabase::contains("DBBLACONNECTION"))
	{
		m_db =  QSqlDatabase::database("DBBLACONNECTION");
	}
	else
	{
		m_db = QSqlDatabase::addDatabase("QMYSQL", "DBBLACONNECTION");
	}
}

DbBlaController::~DbBlaController()
{
	if(m_db.isOpen()){
		m_db.close();
	}
}

bool DbBlaController::connectToDB(const DBConnectionStruct& parameters)
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
		QString resErrorString = m_db.lastError().databaseText() + "\n" + m_db.lastError().driverText();
		qDebug() << resErrorString;
		return false;
	}

	return true;
}

bool DbBlaController::addBla(const Bla&)
{
}

bool DbBlaController::addBla(const Bla&, const BlaInfo&)
{
}

bool DbBlaController::addBlaInfo(const BlaInfo&)
{
}

bool DbBlaController::addDevice(const Device&)
{
}

bool DbBlaController::addBlaMission(const BlaMission&)
{
}

bool DbBlaController::addTarget(const Target&)
{
}
