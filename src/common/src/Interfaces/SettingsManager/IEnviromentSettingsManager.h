#ifndef IENVIROMENTSETTINGSMANAGER_H
#define IENVIROMENTSETTINGSMANAGER_H

class QString;

class IEnviromentSettingsManager
{

public:
	virtual ~IEnviromentSettingsManager(){}

	virtual void setIniFile(const QString& filename)	= 0;
	virtual QString getArmrConnectionMode()	= 0;
};

#endif // IENVIROMENTSETTINGSMANAGER_H
