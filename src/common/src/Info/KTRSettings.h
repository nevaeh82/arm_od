#ifndef KTRSETTINGS_H
#define KTRSETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct KTRSettings : public BaseSetting
{
	KTRSettings() : BaseSetting()
	{
		type = TypeKTR;
	}

	BaseSettingsType& getType()
	{
		return type;
	}

}KTRSettings;

inline QDataStream& operator<<(QDataStream& out, const KTRSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, KTRSettings &object)
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

inline QByteArray KTRSettingsToByteArray(const KTRSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline KTRSettings KTRSettingsFromByteArray(QByteArray& inBA)
{
	KTRSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // KTRSETTINGS_H
