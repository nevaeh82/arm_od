#include <QSQLiteDriver>

#include <Logger.h>

#include "DBController.h"

DBController::DBController()
{
	m_sdb = QSqlDatabase::addDatabase( "QSQLITE", "Pw.Zaviruha.ARMOD.Tabs.Tree.DB" );
	setDb();
}

DBController::~DBController()
{
	if( m_sdb.isOpen() ) {
		m_sdb.close();
	}
}

QVector<int> DBController::getBplaList( QString tableName )
{
	setTable(tableName);

	QString sql = "SELECT id FROM %1";
	QSqlQuery query( m_sdb );
	query.exec( sql.arg( tableName ) );

	QSqlRecord rec = query.record();
	QVector<int> result;
	while( query.next() ) {
		for( int i = 0; i < rec.count(); i++ ) {
			result.push_back( query.value(i).toInt() );
		}
	}

	return result;
}

QVector<int> DBController::getFriendBplaList()
{
	return getBplaList( "BLA" );
}

QVector<int> DBController::getEnemyBplaList()
{
	return getBplaList( "BPLA" );
}

QVector<QMap<QString, QVariant> >* DBController::getBplaProperty(QString tableName, int pid)
{
	setTable(tableName);

	QVector<QMap<QString, QVariant> >* result = new QVector<QMap<QString, QVariant> >;

	QString sql = "SELECT * FROM %1 WHERE pid = %2";
	QSqlQuery query( m_sdb );
	query.exec( sql.arg( tableName, QString::number(pid) ) );

	QSqlRecord rec = query.record();
	while( query.next() ) {
		QMap<QString, QVariant> field;
		for( int i = 0; i < rec.count(); i++ ) {
			field.insert( rec.fieldName(i), query.value(i) );
		}

		result->push_back( field );
	}

	return result;
}

QVector<QMap<QString, QVariant> >* DBController::getFriendBplaProperty(int pid)
{
	return getBplaProperty( "BLA_Property", pid );
}

QVector<QMap<QString, QVariant> > *DBController::getEnemyBplaPropert(int pid)
{
	return getBplaProperty( "BPLA_Property", pid );
}

void DBController::setBpla(QString tableName, QMap<QString, QVariant> *data)
{
	QString id = data->value("id").toString();

	if( id.isNull() ) return;

	QString sql;
	QSqlQuery query( m_sdb );

	setTable(tableName);

	sql = "SELECT id FROM %1 WHERE id = %2";
	query.exec( sql.arg( tableName, id ) );

	// if record already exists, just update it
	if( query.next() ) {
		sql = "UPDATE %1 SET %2 WHERE id = %3";

		// collect fields to update
		QStringList fields;
		QMap<QString, QVariant>::iterator it;
		for( it = data->begin(); it != data->end(); ++it ) {
			if( it.key() == "id" ) continue;

			fields << QString( "\"%1\" = \"%2\"" ).arg( it.key(), it.value().toString() );
		}

		query.exec( sql.arg( tableName, fields.join(", "), id ) );
	}
	// else we should create new one
	else {
		sql = "INSERT INTO %1 (%2) VALUES (%3)";

		QStringList keys = data->keys();

		// collect values to insert
		QStringList values;
		QMap<QString, QVariant>::iterator it;
		for( it = data->begin(); it != data->end(); ++it ) {
			values << QString( "\"%1\"" ).arg( it.value().toString() );
		}

		query.exec( sql.arg( tableName, keys.join(", "), values.join(", ") ) );
	}
}

void DBController::setFriendBpla(QMap<QString, QVariant> *data)
{
	setBpla( "BLA", data );
}

void DBController::setEnemyBpla(QMap<QString, QVariant> *data)
{
	setBpla( "BPLA", data );
}

void DBController::setBplaProperty(QString tableName, QMap<QString, QVariant> *data)
{


	QString pid = data->value("pid").toString();
	QString id = data->value("id").toString();

	if( pid.isNull() || id.isNull() ) return;

	QString sql;
	QSqlQuery query( m_sdb );

	setTable(tableName);

	// check if property already exists in DB
	sql = "SELECT id FROM %1 WHERE pid = %2 AND id = %3";
	query.exec( sql.arg( tableName, pid, id ) );

	// if record already exists, just update it
	if( query.next() ) {
		sql = "UPDATE %1 SET %2 WHERE pid = %3 AND id = %4";

		// collect fields to update
		QStringList fields;
		QMap<QString, QVariant>::iterator it;
		for( it = data->begin(); it != data->end(); ++it ) {
			if( it.key() == "id" || it.key() == "pid" ) continue;

			fields << QString( "\"%1\" = \"%2\"" ).arg( it.key(), it.value().toString() );
		}

		query.exec( sql.arg( tableName, fields.join(", "), pid, id ) );
	}
	// else we should create new one
	else {
		sql = "INSERT INTO %1 (%2) VALUES (%3)";

		QStringList keys = data->keys();

		// collect values to insert
		QStringList values;
		QMap<QString, QVariant>::iterator it;
		for( it = data->begin(); it != data->end(); ++it ) {
			values << QString( "\"%1\"" ).arg( it.value().toString() );
		}

		query.exec( sql.arg( tableName, keys.join(", "), values.join(", ") ) );
	}
}

void DBController::setFriendBplaProperty(QMap<QString, QVariant> *data)
{
	setBplaProperty( "BLA_Property", data );
}

void DBController::setEnemyBplaProperty(QMap<QString, QVariant> *data)
{
	setBplaProperty( "BPLA_Property", data );
}

void DBController::deleteBpla(QString tableName, int id)
{
	setTable(tableName);

	QString sql = "DELETE FROM %1 WHERE id = %2";

	QSqlQuery query( m_sdb );
	query.exec( sql.arg( tableName, QString::number(id) ) );
}

void DBController::deleteFriendBpla(int id)
{
	deleteBpla( "BLA", id );
}

void DBController::deleteEnemyBpla(int id)
{
	deleteBpla( "BPLA", id );
}

void DBController::deleteBplaProperty(QString tableName, int pid, int id)
{
	setTable(tableName);

	QString sql = "DELETE FROM %1 WHERE id = %2 AND pid = %3";

	QSqlQuery query( m_sdb );
	query.exec( sql.arg( tableName, QString::number(id), QString::number(pid) ) );
}

void DBController::deleteFriendBplaProperty(int pid, int id)
{
	deleteBplaProperty( "BLA_Property", pid, id );
}

void DBController::deleteEnemyBplaProperty(int pid, int id)
{
	deleteBplaProperty( "BPLA_Property", pid, id );
}

int DBController::setDb()
{
	m_sdb.setDatabaseName(":memory:");
	return 0;
}

/// create table based on pid
void DBController::setTable(QString name)
{
	QString newTableProperties = "";

	if (name == "BLA" || name == "BPLA"){
		newTableProperties = "(id integer PRIMARY KEY NOT NULL, pid integer, name TEXT, state integer)";
	} else {
		newTableProperties = "(id integer PRIMARY KEY NOT NULL, pid integer, name TEXT, value integer, state integer)";
	}

	QSqlQuery query;

	bool succeeded = query.prepare(QString("CREATE TABLE %1 %2").arg(name).arg(newTableProperties));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return;
	}

	succeeded = query.exec();

	if (!succeeded){
		return;
	}

	/*if(name.length() < 5)
    {
        QString str;
        str.append("CREATE TABLE ");
        str.append(name);
        str.append("(id integer PRIMARY KEY NOT NULL, pid integer, name TEXT, state integer)");
        QSqlQuery query;
        bool err = query.exec(str);
	}
    else
    {

        QString str_prop;
        str_prop.append("CREATE TABLE ");
		str_prop.append(name);
        str_prop.append("(id integer PRIMARY KEY NOT NULL, pid integer, name TEXT, value integer, state integer)");

        QSqlQuery query1;
        bool err = query1.exec(str_prop);
	}*/
}

QMap<QString, QVariant>* DBController::getBplaFields(QString tableName, int id)
{
	setTable(tableName);

	QString sql( "SELECT * FROM %1 WHERE id = %2");
	QSqlQuery query( m_sdb );
	query.exec( sql.arg( tableName, QString::number( id ) ) );

	QSqlRecord rec = query.record();
	QMap<QString, QVariant>* fields = new QMap<QString, QVariant>;
	while( query.next() ) {
		for( int i = 0; i < rec.count(); i++ ) {
			fields->insert( rec.fieldName(i), query.value(i) );
		}
	}

	return fields;
}

QMap<QString, QVariant>* DBController::getFriendBplaFields(int id)
{
	return getBplaFields( "BLA", id );
}

QMap<QString, QVariant> *DBController::getEnemyBplaFields(int id)
{
	return getBplaFields( "BPLA", id );
}

/// read db file location from ini file
int DBController::readSettings(QString pathToIniFile)
{
    int error = -1;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings( pathToIniFile, QSettings::IniFormat );

    m_settings.setIniCodec(codec);

	QStringList childKeys = m_settings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		if( childKey.toLatin1() != "DB_file_name" ) {
			continue;
		}

		log_debug( QString( "m_settings = %1" ).arg( childKey ) );

		m_settings.beginGroup(childKey);
		m_dbName.append("DB/");
		m_dbName += m_settings.value("file", "db_tree.sqlite").toString();

        error = 0;
        m_settings.endGroup();
    }

    return error;
}
