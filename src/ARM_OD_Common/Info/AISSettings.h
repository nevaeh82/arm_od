#ifndef AISSETTINGS_H
#define AISSETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct AISSettings : public BaseSetting
{
	AISSettings() : BaseSetting()
	{
		type = TypeAIS;
	}

	BaseSettingsType& getType()
	{
		return type;
	}

}AISSettings;

inline QDataStream& operator<<(QDataStream& out, const AISSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, AISSettings &object)
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

inline QByteArray AISSettingsToByteArray(const AISSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline AISSettings AISSettingsFromByteArray(QByteArray& inBA)
{
	AISSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // AISSETTINGS_H
