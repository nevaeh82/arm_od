#ifndef DBBLADEFINES_H
#define DBBLADEFINES_H

#include <QtGlobal>
#include <QTime>
#include <QDateTime>

typedef struct Uav
{
	uint id; // PK
	uint uavId;
	QString ip;
	uint uavTypeId; // FK
	uint roleId; // FK
	QString name;
	uint freqId;

	Uav(){
		freqId = 0;
	}

} Uav;

typedef struct UavRole
{
	uint id; // PK
	QString code;
	QString name;
} UavRole;

typedef struct UavInfo
{
	uint id; // PK
	uint uavId; // FK
	uint device; // FK
	double lat;
	double lon;
	double alt;
	double speed;
	double yaw;
	QTime restTime;
	uint statusId; // FK
	QDateTime dateTime;
} UavInfo;

typedef struct Devices
{
	uint id; // PK
	uint deviceId; // FK
	uint port;
	uint uavId;
} Devices;

typedef struct UavMission
{
	uint id; // PK
	uint uavId; // FK
	uint targetId; // FK
	double regionCenterLat;
	double regionCenterLon;
	double regionCenterAltitude;
	double regionRadius;
	QTime timeToTarget;
} UavMission;

typedef struct Target
{
	uint id; // PK
	QString ip;
	uint port;
	uint type; // FK
} Target;

// Dictionaries
typedef struct Status
{
	uint id; // PK
	QString status;
} Status;

typedef struct TargetType
{
	uint id; // PK
	QString name;
} TargetType;

typedef struct UavType
{
	uint id; // PK
	QString name;
} UavType;

typedef struct DeviceType
{
	uint id; // PK
	QString name;
} DeviceType;

#endif // DBBLADEFINES_H
