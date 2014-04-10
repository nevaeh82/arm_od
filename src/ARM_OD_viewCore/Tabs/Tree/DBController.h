#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QtSql>
#include <QString>
#include <QStringList>

#include <QMap>
#include <QVariant>

#include "IDBController.h"

#include "Logger.h"

class DBController : public QObject, public IDBController
{
	Q_OBJECT
public:
	DBController();
	~DBController();

public:
//    virtual QMap<QString, QVariant>* get_data(int id, int pid);
//    virtual QMap<QString, QVariant>* get_data(int pid);

//public slots:
//    virtual void set_data(QMap<QString, QVariant>* data);

	virtual QVector<int> getFriendBplaList();
	virtual QVector<int> getEnemyBplaList();

	virtual QVector<QMap<QString, QVariant> >* getFriendBplaProperty(int pid);
	virtual QVector<QMap<QString, QVariant> >* getEnemyBplaPropert(int pid);

	virtual QMap<QString, QVariant>* getFriendBplaFields(int id);
	virtual QMap<QString, QVariant>* getEnemyBplaFields(int id);

	virtual void setFriendBpla(QMap<QString, QVariant>* data);
	virtual void setFriendBplaProperty(QMap<QString, QVariant>* data);

	virtual void setEnemyBpla(QMap<QString, QVariant>* data);
	virtual void setEnemyBplaProperty(QMap<QString, QVariant>* data);

	virtual void deleteFriendBpla(int id);
	virtual void deleteEnemyBpla(int id);
	virtual void deleteFriendBplaProperty(int pid, int id);
	virtual void deleteEnemyBplaProperty(int pid, int id);


//private slots:
//    void _slot_set(QMap<QString, QVariant>* data);

protected:
	void setBpla( QString tableName, QMap<QString, QVariant> *data );
	void setBplaProperty( QString tableName, QMap<QString, QVariant> *data );


	void deleteBpla( QString tableName, int id );
	void deleteBplaProperty(QString tableName, int pid, int id);

	QVector<int> getBplaList( QString tableName );
	QMap<QString, QVariant>* getBplaFields( QString tableName, int id );
	QVector<QMap<QString, QVariant> >* getBplaProperty( QString tableName, int pid );

private:
	QSqlDatabase m_sdb;
	QString m_dbName;

	int readSettings( QString pathToIniFile );
	int setDb();
	void setTable(QString name);

signals:
	void signalFinished();
//     void signalSet(QMap<QString, QVariant> *);

};

#endif // DBCONTROLLER_H
