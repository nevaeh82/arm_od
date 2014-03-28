#ifndef BASESETTINGS_H
#define BASESETTINGS_H

#include <QString>

enum BaseSettingsType
{
	TypeNIIPP = 1,
	TypeKTR = 2,
	TypeAIS = 3,
	TypeArmrTcpClient = 4,
	typeUnknown = 0
};

typedef struct BaseSetting
{
	BaseSetting()
	{
		name = QString();
		host = "127.0.0.1";
		port = 1024;
	}

	QString name;
	QString host;
	quint32 port;

protected:
	BaseSettingsType type;

}BaseSetting;

#endif // BASESETTINGS_H
