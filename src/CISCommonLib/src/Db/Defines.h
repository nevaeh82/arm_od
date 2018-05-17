#ifndef DBDEFINES_H
#define DBDEFINES_H

#include <QtGlobal>
#include <QDataStream>

/// parameters for connect to mysql
typedef struct DBConnectionStruct
{
	DBConnectionStruct()
	{
		host		= "";
		port		= 0;
		login		= "";
		password	= "";
		dbName		= "";
	}

	DBConnectionStruct(const DBConnectionStruct& object)
	{
		host		= object.host;
		port		= object.port;
		login		= object.login;
		password	= object.password;
		dbName		= object.dbName;
	}

	QString host;
	quint16 port;
	QString login;
	QString password;
	QString dbName;
} DBConnectionStruct;

inline QDataStream& operator<<(QDataStream& out, const DBConnectionStruct& object)
{
	return out << object.host
			   << object.port
			   << object.login
			   << object.password
			   << object.dbName;
}

inline QDataStream& operator>>(QDataStream& in, DBConnectionStruct& object)
{
	in >> object.host
	   >> object.port
	   >> object.login
	   >> object.password
	   >> object.dbName;

	return in;
}

#endif // DBDEFINES_H
