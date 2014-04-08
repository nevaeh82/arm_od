#ifndef UAVDEFINES_H
#define UAVDEFINES_H

#include <QPointF>
#include <QVector>
#include <QtGlobal>
#include <QDateTime>
#include <QDataStream>
#include <QSharedPointer>

typedef struct UAVPositionData {

	UAVPositionData()
	{
		boardID		= 0;
		device		= 0;
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
		device		= data.device;
		latitude	= data.latitude;
		longitude	= data.longitude;
		altitude	= data.altitude;
		speed		= data.speed;
		course		= data.course;
		state		= data.state;
		dateTime	= data.dateTime;
	}

	quint16		boardID;
	quint32		device;
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
	return out << object.boardID << object.device << object.latitude
			   << object.longitude << object.altitude
			   << object.speed << object.course
			   << object.state << object.dateTime;
}

inline QDataStream& operator>>(QDataStream& in, UAVPositionData& object)
{
	in >> object.boardID >> object.device >> object.latitude
	   >> object.longitude >> object.altitude
	   >> object.speed >> object.course
	   >> object.state >> object.dateTime;
	return in;
}

typedef struct UAVPositionDataEnemy
{
	UAVPositionDataEnemy()
	{
		altitude	= 0.0;
		speed		= 0.0;
		course		= 0.0;
		state		= 0;
		time	= QTime::currentTime();
		pointStdDev	= QPointF(0.0, 0.0);
	}

	UAVPositionDataEnemy(const UAVPositionDataEnemy& object)
	{
		altitude	= object.altitude;
		speed		= object.speed;
		course		= object.course;
		state		= object.state;
		time	= object.time;
		pointStdDev	= object.pointStdDev;
		track		= object.track;
	}

	double		altitude;
	double		speed;
	double		course;
	int			state;
	QTime		time;
	QPointF		pointStdDev;
	QVector<QPointF>	track;

} UAVPositionDataEnemy;

inline QDataStream& operator<<(QDataStream& out, const UAVPositionDataEnemy& object)
{
	return out << object.altitude << object.speed
			   << object.course << object.state
			   << object.time << object.pointStdDev
			   << object.track;
}

inline QDataStream& operator>>(QDataStream& in, UAVPositionDataEnemy& object)
{
	in >> object.altitude >> object.speed
	   >> object.course >> object.state
	   >> object.time >> object.pointStdDev
	   >> object.track;
	return in;
}

#endif // UAVDEFINES_H
