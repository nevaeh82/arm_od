#ifndef NIIPPSETTINGS_H
#define NIIPPSETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct NIIPPSettings : public BaseSetting
{
	NIIPPSettings() : BaseSetting()
	{
		type = TypeNIIPP;
	}

	BaseSettingsType& getType()
	{
		return type;
	}

}NIIPPSettings;

inline QDataStream& operator<<(QDataStream& out, const NIIPPSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, NIIPPSettings &object)
{
	quint32 type;
	QString name;
	QString host;
	quint32 port;

	in >> type >> name >> host >> port;
	object.name = name;
	object.host = host;
	object.port = port;

	return in;
}

inline QByteArray NIIPPSettingsToByteArray(const NIIPPSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline NIIPPSettings NIIPPSettingsFromByteArray(QByteArray& inBA)
{
	NIIPPSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // NIIPPSETTINGS_H
