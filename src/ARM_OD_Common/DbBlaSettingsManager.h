#ifndef DBBLASETTINGSMANAGER_H
#define DBBLASETTINGSMANAGER_H

#include <QObject>
#include <QMutex>

#include "Interfaces/IDbBlaSettingsManager.h"
#include "SingletonTemplate.h"

class DbBlaSettingsManager : public QObject, public IDbBlaSettingsManager, public SingletonTemplate<DbBlaSettingsManager, IDbBlaSettingsManager>
{
	Q_OBJECT
private:
	QString m_settingsFile;
	QMutex m_mutex;

public:
	explicit DbBlaSettingsManager(QObject *parent = 0);
	virtual ~DbBlaSettingsManager();

	void setIniFile(const QString&);

	//FRIEND
	QString getBlaDbHost();
	unsigned short getBlaDbPort();
	QString getBlaDbLogin();
	QString getBlaDbPassword();
	QString getBlaDbName();

	// ENEMY
	QString getBplaDbHost();
	unsigned short getBplaDbPort();
	QString getBplaDbLogin();
	QString getBplaDbPassword();
	QString getBplaDbName();
};

#endif // DBBLASETTINGSMANAGER_H
