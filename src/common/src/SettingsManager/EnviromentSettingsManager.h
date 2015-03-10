#ifndef ENVIROMENTSETTINGSMANAGER_H
#define ENVIROMENTSETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include <Templates/SingletonTemplate.h>

#include "Interfaces/SettingsManager/IEnviromentSettingsManager.h"

class EnviromentSettingsManager : public QObject, public IEnviromentSettingsManager, public SingletonTemplate<EnviromentSettingsManager, IEnviromentSettingsManager>
{
	Q_OBJECT

private:
	QString m_settingsFile;
	QMutex m_mutex;

public:
	explicit EnviromentSettingsManager(QObject* parent = NULL);
	virtual ~EnviromentSettingsManager();

	// IEnviromentSettingsManager interface
public:
	virtual void setIniFile(const QString& filename);
	virtual QString getArmrConnectionMode();
};

#endif // ENVIROMENTSETTINGSMANAGER_H
