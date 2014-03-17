#ifndef TCPSETTINGSMANAGER_H
#define TCPSETTINGSMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>

#include "Interfaces/ITcpSettingsManager.h"
#include "SingletonTemplate.h"

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

	virtual QString getFlakonHost();
	virtual QString getFlakonPort();
	virtual QString getAtlantHost();
	virtual QString getAtlantPort();
};

#endif // TCPSETTINGSMANAGER_H
