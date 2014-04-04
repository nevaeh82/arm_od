#include "TcpSettingsManager.h"

TcpSettingsManager::TcpSettingsManager(QObject *parent) :
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

QMap<QString, quint32> TcpSettingsManager::getAllInfo()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings m_settings(m_settingsFile, QSettings::IniFormat);
	m_settings.setIniCodec("UTF-8");

	QMap<QString, quint32> mapInfo;
	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		m_settings.beginGroup(childKey);

		QString name = m_settings.value("name", "").toString();
		int type = m_settings.value("type", 0).toInt();

		if(type > 0)
		{
			log_info(QString("Name = %1, type =%2, %3").arg(QString(name)).arg(type).arg(childKey));
			mapInfo.insert(name, type);
		}
		m_settings.endGroup();
	}

	return mapInfo;
}
