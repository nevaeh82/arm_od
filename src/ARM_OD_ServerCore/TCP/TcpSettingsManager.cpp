#include "TcpSettingsManager.h"

TcpSettingsManager::TcpSettingsManager(QObject* parent) :
	QObject(parent)
{
}

TcpSettingsManager::~TcpSettingsManager()
{
}

void TcpSettingsManager::setIniFile(const QString& filePath)
{
	m_settingsFile = filePath;
}

QString TcpSettingsManager::getFlakonHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("Flakon/ip", "127.0.0.1").toString();
}

QString TcpSettingsManager::getFlakonPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("Flakon/port", "0").toString();
}

QString TcpSettingsManager::getAtlantHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("ATLANT/ip", "127.0.0.1").toString();
}

QString TcpSettingsManager::getAtlantPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("ATLANT/port", "127.0.0.1").toString();
}
