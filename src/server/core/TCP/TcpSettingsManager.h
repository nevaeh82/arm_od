#ifndef TCPSETTINGMANAGER_H
#define TCPSETTINGMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QStringList>

#include <Templates/SingletonTemplate.h>
#include <Logger/Logger.h>

#include "Interfaces/ITcpSettingsManager.h"

class TcpSettingsManager : public QObject, public ITcpSettingsManager, public SingletonTemplate<TcpSettingsManager, ITcpSettingsManager>
{
	Q_OBJECT

private:
	QString m_settingsFile;
	QMutex m_mutex;

public:
	explicit TcpSettingsManager(QObject* parent = NULL);
	virtual ~TcpSettingsManager();
	// ITcpSettingsManager interface
public:
	virtual void setIniFile(const QString& filePath);
	virtual QMap<QString, quint32> getAllInfo();
};

#endif // TCPSETTINGMANAGER_H
