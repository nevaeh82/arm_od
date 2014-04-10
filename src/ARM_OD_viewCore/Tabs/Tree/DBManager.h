#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>

#include "IDBManager.h"
#include "IDBController.h"
#include "DBController.h"

#include "Common/CommandMessage.h"
#include "Tabs/ITabManager.h"

class DBManager : public QObject, public IDBManager
{
	Q_OBJECT
public:
	DBManager(ITabManager* tab_manager);
	~DBManager();

public:
	virtual int set(int group, QMap<QString, QVariant>* data);
	virtual int setProperty(int group, QMap<QString, QVariant>* data);

	virtual QVector<QMap<QString, QVariant> >* get(int id, int group);
	virtual QVector<int> get(int group);

	virtual void removeItem(int id, int group);

	virtual QMap<QString, QVariant>* getFriendBplaFields(int id);
	virtual QMap<QString, QVariant>* getEnemyBplaFields(int id);

	virtual void deleteFriendBpla(int id);
	virtual void deleteEnemyBpla(int id);
	virtual void deleteFriendBplaProperty(int pid, int id);
	virtual void deleteEnemyBplaProperty(int pid, int id);

private:
	ITabManager*        m_tabManager;
	DBController*       m_dbController;
};

#endif // DBMANAGER_H
