#ifndef UAVDEFINES_H
#define UAVDEFINES_H

#include <QtGlobal>
#include <QDateTime>
#include <QDataStream>
#include <QSharedPointer>

typedef struct UAVPositionData {

	UAVPositionData()
	{
		boardID		= 0;
		latitude	= 0.0;
		longitude	= 0.0;
		altitude	= 0.0;
		speed		= 0.0;
		course		= 0.0;
		state		= 0;
		dateTime	= QDateTime::currentDateTime();
	}

	UAVPositionData(const UAVPositionData& data)
	{
		boardID		= data.boardID;
		latitude	= data.latitude;
		longitude	= data.longitude;
		altitude	= data.altitude;
		speed		= data.speed;
		course		= data.course;
		state		= data.state;
		dateTime	= data.dateTime;
	}

	quint16		boardID;
	double		latitude;
	double		longitude;
	double		altitude;
	double		speed;
	double		course;
	quint32		state;
	QDateTime	dateTime;
} UAVPositionData;

inline QDataStream& operator<<(QDataStream& out, const UAVPositionData& object)
{
	return out << object.boardID << object.latitude
			   << object.longitude << object.altitude
			   << object.speed << object.course
			   << object.state << object.dateTime;
}

inline QDataStream& operator>>(QDataStream& in, UAVPositionData& object)
{
	quint16		boardID;
	double		latitude;
	double		longitude;
	double		altitude;
	double		speed;
	double		course;
	quint32		state;
	QDateTime	dateTime;


	in >> boardID >> latitude
		>> longitude >> altitude
		>> speed >> course
		>> state >> dateTime;

	object.boardID		= boardID;
	object.latitude		= latitude;
	object.longitude	= longitude;
	object.altitude		= altitude;
	object.speed		= speed;
	object.course		= course;
	object.state		= state;
	object.dateTime		= dateTime;

	return in;
}

#endif // UAVDEFINES_H
