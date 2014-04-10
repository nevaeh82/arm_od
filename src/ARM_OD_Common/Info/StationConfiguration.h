#ifndef STATIONCONFIGURATION_H
#define STATIONCONFIGURATION_H

#include <QtGlobal>

struct StationConfiguration
{
	StationConfiguration()
	{
		id			= 0;
		name		= "Unknown";
		latitude	= 0.0;
		longitude	= 0.0;
	}

	StationConfiguration(const StationConfiguration& object)
	{
		id			= object.id;
		name		= object.name;
		latitude	= object.latitude;
		longitude	= object.longitude;
	}

	quint32		id;
	QString		name;
	double		latitude;
	double		longitude;
};

inline QDataStream& operator<<(QDataStream& out, const StationConfiguration& object)
{
	return out << object.id
			   << object.name
			   << object.latitude
			   << object.longitude;
}

inline QDataStream& operator>>(QDataStream& in, StationConfiguration& object)
{
	in >> object.id
	   >> object.name
	   >> object.latitude
	   >> object.longitude;

	return in;
}

#endif // STATIONCONFIGURATION_H
