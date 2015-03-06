#ifndef ITCPSETTINGMANAGER_H
#define ITCPSETTINGMANAGER_H

#include <QMap>

class ITcpSettingsManager
{

public:
	virtual ~ITcpSettingsManager(){}

	virtual void setIniFile(const QString& filePath)	= 0;
	virtual QMap<QString, quint32> getAllInfo()			= 0;
};

#endif // ITCPSETTINGMANAGER_H
