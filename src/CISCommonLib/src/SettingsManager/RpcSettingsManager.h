#ifndef RPCSETTINGSMANAGER_H
#define RPCSETTINGSMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QString>

#include "Templates/SingletonTemplate.h"
#include "Interfaces/SettingsManager/IRpcSettingsManager.h"

class RpcSettingsManager : public QObject, public IRpcSettingsManager, public SingletonTemplate<RpcSettingsManager, IRpcSettingsManager>
{
	Q_OBJECT

private:
	QString m_settingsFile;
	QMutex m_mutex;

public:
	explicit RpcSettingsManager(QObject* parent = NULL);
	virtual ~RpcSettingsManager();

	// IRpcSettingsManager interface
public:
	virtual void setIniFile(const QString& filePath);
	virtual QString getRpcHost();
	virtual QString getRpcPort();
};

#endif // RPCSETTINGSMANAGER_H
