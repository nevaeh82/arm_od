#ifndef IRPCSETTINGSMANAGER_H
#define IRPCSETTINGSMANAGER_H

class QString;

class IRpcSettingsManager
{
public:
	virtual ~IRpcSettingsManager(){}

	virtual void setIniFile(const QString& filePath)	= 0;

	virtual QString getRpcHost()	= 0;
	virtual QString getRpcPort()	= 0;
};

#endif // IRPCSETTINGSMANAGER_H
