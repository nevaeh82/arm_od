#include "EnviromentSettingsManager.h"

EnviromentSettingsManager::EnviromentSettingsManager(QObject* parent) :
	QObject(parent)
{
	m_settingsFile = "";
}

EnviromentSettingsManager::~EnviromentSettingsManager()
{
}

void EnviromentSettingsManager::setIniFile(const QString& filename)
{
	m_settingsFile = filename;
}

QString EnviromentSettingsManager::getArmrConnectionMode()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("Connection/Mode", "0").toString();
}
