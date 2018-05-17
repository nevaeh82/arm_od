#ifndef DBCONTROLLERBASE_H
#define DBCONTROLLERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql/QSqlError>
#include "Defines.h"
#include "Interfaces/IDbControllerBase.h"

#include "Logger/Logger.h"

class DbControllerBase : public QObject, public IDbControllerBase
{
	Q_OBJECT
protected:
	QSqlDatabase m_db;
	QString     m_dbName;

public:
	explicit DbControllerBase(QString connectionName, QString dbType, QObject *parent = 0);
	virtual ~DbControllerBase();

	bool connectToDB(const DBConnectionStruct& parameters);
	void disconnectFromDb();
	
signals:
	
public slots:
	
};

#endif // DBCONTROLLERBASE_H
