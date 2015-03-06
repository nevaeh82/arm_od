#include "DbController.h"

#include <QDebug>

#include <QtSql>

DbController::DbController(const QString& dbFile, QObject *parent) :
	QObject(parent)
{
	bool isOpened = createConnection(dbFile);

	qDebug() << "Is DB opened: " << isOpened;

	QSqlQuery("PRAGMA page_size = 4096", m_sdb);
	QSqlQuery("PRAGMA cache_size = 16384", m_sdb);
	QSqlQuery("PRAGMA temp_store = MEMORY", m_sdb);
	QSqlQuery("PRAGMA journal_mode = OFF", m_sdb);
	QSqlQuery("PRAGMA locking_mode = EXCLUSIVE", m_sdb);
	QSqlQuery("PRAGMA synchronous = OFF", m_sdb);
}

DbController::~DbController()
{
	closeConnection();
}

PropertiesList DbController::getProperties(const uint objectID)
{
	PropertiesList propsList;

	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("SELECT * FROM Properties WHERE OBJECT_ID = :objectID;");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return propsList;
	}

	query.bindValue(":objectID", objectID);

	succeeded = query.exec();

	if (!succeeded) {
		return propsList;
	}

	while(query.next()) {

		Property prop;

		prop.id = query.value(0).toUInt();
		prop.pid = query.value(1).toUInt();
		prop.name = getPropNameById(query.value(2).toUInt());
		prop.value = query.value(3);
		prop.state = query.value(4).toInt();
		prop.isEditable= query.value(5).toBool();

		propsList.append(prop);
	}


	return propsList;
}

Property DbController::getProperty(const uint propID)
{
	Property prop;

	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("SELECT * FROM Properties WHERE ID = :id;");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return prop;
	}

	query.bindValue(":id", propID);

	succeeded = query.exec();

	if (!succeeded) {
		return prop;
	}

	if(!query.next()) {
		return prop;
	}

	prop.id = query.value(0).toUInt();
	prop.pid = query.value(1).toUInt();
	prop.name = getPropNameById(query.value(2).toUInt());
	prop.value = query.value(3);
	prop.state = query.value(4).toInt();
	prop.isEditable= query.value(4).toBool();

	return prop;
}

Object DbController::getObject(const uint objectId)
{
	return getObjectByParam("ID", objectId);
}

Object DbController::getObject(const QString &objectName)
{
	return getObjectByParam("OBJECT_NAME", objectName);
}

ObjectsList DbController::getAllObjects()
{
	ObjectsList objectsList;

	QSqlQuery query(m_sdb);
	QString sql = QString("SELECT * FROM Objects");
	bool succeeded = query.prepare(sql);

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return objectsList;
	}

	succeeded = query.exec();

	if (!succeeded) {
		return objectsList;
	}

	while(query.next()) {

		Object obj;

		obj.id = query.value(0).toUInt();
		obj.pid = query.value(2).toUInt();
		obj.name = query.value(1).toString();
		obj.state = query.value(3).toInt();
		obj.isEditable= query.value(4).toBool();

		objectsList.append(obj);
	}

	return objectsList;
}

uint DbController::addObject(const Object &object)
{
	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("INSERT INTO Objects VALUES (NULL, :objectName, :pid, :state, :editable);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectName", object.name);
	query.bindValue(":state", object.state);
	query.bindValue(":editable", object.isEditable);

	if (object.pid != 0) {
		query.bindValue(":pid", object.pid);
	}
	else {
		query.bindValue(":pid", QVariant());
	}

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

uint DbController::addProperty(const Property& property)
{
	uint propNameIndex = isPropInDictionary(property.name) ;

	if (propNameIndex == INVALID_INDEX) {
		propNameIndex = addPropToDictionaty(property.name);
	}

	if (propNameIndex == INVALID_INDEX) {
		return INVALID_INDEX;
	}

	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("INSERT INTO Properties VALUES (NULL, :objectId, :propId, :propValue, :state, :isEditable);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectId", property.pid);
	query.bindValue(":propId", propNameIndex);
	query.bindValue(":propValue", property.value);
	query.bindValue(":state", property.state);
	query.bindValue(":isEditable", property.isEditable);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	qDebug() << query.lastError().text();

	return INVALID_INDEX;
}

bool DbController::setPropertyState(const uint propId, const int state)
{
	return setPropertyParam(propId, "STATE", state);
}

bool DbController::setPropertyEditable(const uint propId, const bool isEditable)
{
	return setPropertyParam(propId, "EDITABLE", isEditable);
}

bool DbController::setPropertyValue(const uint propId, const QVariant propValue)
{
	return setPropertyParam(propId, "VALUE", propValue);
}

bool DbController::createConnection(const QString& dbFile)
{
	bool isOpened = false;

	if(QSqlDatabase::contains("SETTINGSCONNECTION"))
	{
		m_sdb =  QSqlDatabase::database("SETTINGSCONNECTION");
	}
	else
	{
		m_sdb = QSqlDatabase::addDatabase("QSQLITE", "SETTINGSCONNECTION");
	}
	
	m_sdb.setDatabaseName(dbFile);

	isOpened = m_sdb.open();

	if (!isOpened) {
		qDebug() << m_sdb.lastError().text();
	}

	return isOpened;
}

void DbController::closeConnection()
{
	if ( m_sdb.isOpen()) {
		m_sdb.close();
	}
}

uint DbController::addPropToDictionaty(const QString &propName)
{
	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("INSERT INTO PropertyDictionary VALUES (NULL, :propName);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":propName", propName);

	succeeded = query.exec();

	if (!succeeded) {
		return INVALID_INDEX;
	}

	return query.lastInsertId().toUInt();
}

uint DbController::isPropInDictionary(const QString &propName)
{
	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("SELECT ID FROM PropertyDictionary WHERE PROP_NAME = :propName;");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":propName", propName);

	succeeded = query.exec();

	if (!succeeded) {
		return INVALID_INDEX;
	}

	if (query.next()) {
		return query.value(0).toUInt();
	}

	return INVALID_INDEX;
}

QString DbController::getPropNameById(const uint propId)
{
	QSqlQuery query(m_sdb);
	bool succeeded = query.prepare("SELECT PROP_NAME FROM PropertyDictionary WHERE ID = :id;");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return QString();
	}

	query.bindValue(":id", propId);

	succeeded = query.exec();

	if (!succeeded) {
		return QString();
	}

	if (query.next()) {
		return query.value(0).toString();
	}

	return QString();
}

bool DbController::setPropertyParam(const uint propId, const QString &paramName, const QVariant value)
{
	QSqlQuery query(m_sdb);
	QString sql = QString("UPDATE Properties SET %1 = :value WHERE ID = :id").arg(paramName);
	bool succeeded = query.prepare(sql);

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return false;
	}

	query.bindValue(":value", value);
	query.bindValue(":id", propId);

	succeeded = query.exec();

	if (!succeeded) {
		return false;
	}

	return true;
}

Object DbController::getObjectByParam(const QString &paramName, const QVariant value)
{
	Object obj;

	QSqlQuery query(m_sdb);
	QString sql = QString("SELECT * FROM Objects WHERE %1 = :value;").arg(paramName);
	bool succeeded = query.prepare(sql);

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return obj;
	}

	query.bindValue(":value", value);

	succeeded = query.exec();

	if (!succeeded) {
		return obj;
	}

	if(!query.next()) {
		return obj;
	}

	obj.id = query.value(0).toUInt();
	obj.pid = query.value(2).toUInt();
	obj.name = query.value(1).toString();
	obj.state = query.value(3).toInt();
	obj.isEditable= query.value(4).toBool();

	return obj;
}



