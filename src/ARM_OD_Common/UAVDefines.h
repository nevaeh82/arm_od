#ifndef UAVDEFINES_H
#define UAVDEFINES_H

#include <QPointF>
#include <QVector>
#include <QtGlobal>
#include <QDateTime>
#include <QDataStream>
#include <QSharedPointer>

#define UAV_AUTOPILOT_SOURCE		1

#define UAV_SOLVER_AUTO_SOURCE		100
#define UAV_SOLVER_MANUAL_SOURCE	101
#define UAV_SOLVER_SINGLE_1_SOURCE	102
#define UAV_SOLVER_SINGLE_2_SOURCE	103

#define UAV_SLICES_SOURCE			622

typedef struct UAVPositionData {

	UAVPositionData()
	{
		boardID		= 0;
		device		= 0;
		latitude	= 0.0;
		longitude	= 0.0;
		latitudeStddev	= 0.0;
		longitudeStddev	= 0.0;
		altitude	= 0.0;
		speed		= 0.0;
		course		= 0.0;
		frequency	= -1;
		state		= 0;
		dateTime	= QDateTime::currentDateTime();
		sourceType	= UAV_AUTOPILOT_SOURCE;
	}

	UAVPositionData(const UAVPositionData& data)
	{
		boardID		= data.boardID;
		device		= data.device;
		latitude	= data.latitude;
		longitude	= data.longitude;
		latitudeStddev	= data.latitudeStddev;
		longitudeStddev	= data.longitudeStddev;
		altitude	= data.altitude;
		speed		= data.speed;
		course		= data.course;
		frequency	= data.frequency;
		state		= data.state;
		dateTime	= data.dateTime;
		sourceType  = data.sourceType;
	}

	quint16		boardID;
	quint32		device;
	double		latitude;
	double		longitude;
	double		altitude;
	double		latitudeStddev;
	double		longitudeStddev;
	double		speed;
	double		course;
	double		frequency;
	quint32		state;
	QDateTime	dateTime;
	uint		sourceType;
} UAVPositionData;

inline QDataStream& operator<<(QDataStream& out, const UAVPositionData& object)
{
	return out << object.boardID << object.device << object.latitude
			   << object.longitude << object.altitude
			   << object.speed << object.course << object.frequency
			   << object.state << object.dateTime << object.sourceType;
}

inline QDataStream& operator>>(QDataStream& in, UAVPositionData& object)
{
	in >> object.boardID >> object.device >> object.latitude
	   >> object.longitude >> object.altitude
	   >> object.speed >> object.course >> object.frequency
	   >> object.state >> object.dateTime >> object.sourceType;

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
		frequency	= -1;
		time	= QTime::currentTime();
		pointStdDev	= QPointF(0.0, 0.0);
		sourceType  = 0;
	}

	UAVPositionDataEnemy(const UAVPositionDataEnemy& object)
	{
		altitude	= object.altitude;
		speed		= object.speed;
		course		= object.course;
		state		= object.state;
		frequency	= object.frequency;
		time		= object.time;
		pointStdDev	= object.pointStdDev;
		track		= object.track;
		sourceType  = object.sourceType;
	}

	double		altitude;
	double		speed;
	double		course;
	int			state;
	double		frequency;
	QTime		time;
	QPointF		pointStdDev;
	QVector<QPointF>	track;
	uint		sourceType;

} UAVPositionDataEnemy;

inline QDataStream& operator<<(QDataStream& out, const UAVPositionDataEnemy& object)
{
	return out << object.altitude << object.speed
			   << object.course << object.state << object.frequency
			   << object.time << object.pointStdDev
			   << object.track;
}

inline QDataStream& operator>>(QDataStream& in, UAVPositionDataEnemy& object)
{
	in >> object.altitude >> object.speed
	   >> object.course >> object.state >> object.frequency
	   >> object.time >> object.pointStdDev
	   >> object.track;
	return in;
}

#endif // UAVDEFINES_H
