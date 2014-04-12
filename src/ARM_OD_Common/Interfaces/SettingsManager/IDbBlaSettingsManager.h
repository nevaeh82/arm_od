#ifndef IDBBLASETTINGSMANAGER_H
#define IDBBLASETTINGSMANAGER_H

class QString;

class IDbBlaSettingsManager
{
	public:
		virtual ~IDbBlaSettingsManager(){}

		virtual void setIniFile(const QString&) = 0;

		//FRIEND
		virtual QString getBlaDbHost() = 0;
		virtual unsigned short getBlaDbPort() = 0;
		virtual QString getBlaDbLogin() = 0;
		virtual QString getBlaDbPassword() = 0;
		virtual QString getBlaDbName() = 0;

		// ENEMY
		virtual QString getBplaDbHost() = 0;
		virtual unsigned short getBplaDbPort() = 0;
		virtual QString getBplaDbLogin() = 0;
		virtual QString getBplaDbPassword() = 0;
		virtual QString getBplaDbName() = 0;
};

#endif // IDBBLASETTINGSMANAGER_H
