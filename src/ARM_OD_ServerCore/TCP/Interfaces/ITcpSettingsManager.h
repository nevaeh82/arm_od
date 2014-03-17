#ifndef ITCPSETTINGSMANAGER_H
#define ITCPSETTINGSMANAGER_H

class QString;

class ITcpSettingsManager
{

public:
	virtual ~ITcpSettingsManager(){}

	virtual void setIniFile(const QString& filePath)	= 0;

	virtual QString getFlakonHost()		= 0;
	virtual QString getFlakonPort()		= 0;

	virtual QString getAtlantHost()		= 0;
	virtual QString getAtlantPort()		= 0;
};

#endif // ITCPSETTINGSMANAGER_H
