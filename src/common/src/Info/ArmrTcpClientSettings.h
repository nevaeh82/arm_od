#ifndef ARMRTCPCLIENTSETTINGS_H
#define ARMRTCPCLIENTSETTINGS_H
#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct ArmrTcpClientSettings : public BaseSetting
{
	ArmrTcpClientSettings() : BaseSetting()
	{
		type = TypeAIS;
	}

	BaseSettingsType& getType()
	{
		return type;
	}

}ArmrTcpClientSettings;

inline QDataStream& operator<<(QDataStream& out, const ArmrTcpClientSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, ArmrTcpClientSettings &object)
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

inline QByteArray ArmrTcpClientSettingsToByteArray(const ArmrTcpClientSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline ArmrTcpClientSettings ArmrTcpClientSettingsFromByteArray(QByteArray& inBA)
{
	ArmrTcpClientSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // ARMRTCPCLIENTSETTINGS_H
