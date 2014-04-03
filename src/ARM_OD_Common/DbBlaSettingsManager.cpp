#include "DbBlaSettingsManager.h"
#include <QString>
#include <QSettings>
#include <QMutexLocker>

DbBlaSettingsManager::DbBlaSettingsManager(QObject *parent) :
	QObject(parent)
{
}

DbBlaSettingsManager::~DbBlaSettingsManager()
{
}

void DbBlaSettingsManager::setIniFile(const QString& filePath)
{
	m_settingsFile = filePath;
}

QString DbBlaSettingsManager::getBlaDbHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BLADB/Host", "127.0.0.1").toString();
}

unsigned short DbBlaSettingsManager::getBlaDbPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BLADB/Port", "3306").toUInt();
}

QString DbBlaSettingsManager::getBlaDbLogin()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BLADB/Login", "root").toUInt();
}

QString DbBlaSettingsManager::getBlaDbPassword()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BLADB/Password", "qwerty12345").toUInt();
}

QString DbBlaSettingsManager::getBlaDbName()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BLADB/Name", "bla").toUInt();
}

QString DbBlaSettingsManager::getBplaDbHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BPLADB/Host", "127.0.0.1").toString();
}

unsigned short DbBlaSettingsManager::getBplaDbPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BPLADB/Port", "3306").toUInt();
}

QString DbBlaSettingsManager::getBplaDbLogin()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BPLADB/Login", "root").toUInt();
}

QString DbBlaSettingsManager::getBplaDbPassword()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BPLADB/Password", "qwerty12345").toUInt();
}

QString DbBlaSettingsManager::getBplaDbName()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat);
	return settings.value("BPLADB/Name", "bpla").toUInt();
}
