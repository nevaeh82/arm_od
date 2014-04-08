#ifndef DBBLADEFINES_H
#define DBBLADEFINES_H

#include <QtGlobal>
#include <QTime>
#include <QDateTime>

typedef struct Uav
{
	int id; // PK
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
	int id; // PK
	QString code;
	QString name;
} UavRole;

typedef struct UavInfo
{
	int id; // PK
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
	int id; // PK
	uint deviceId; // FK
	uint port;
	uint uavId;
} Devices;

typedef struct UavMission
{
	int id; // PK
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
	int id; // PK
	QString ip;
	uint port;
	uint type; // FK
} Target;

// Dictionaries
typedef struct Status
{
	int id; // PK
	QString status;
} Status;

typedef struct TargetType
{
	int id; // PK
	QString name;
} TargetType;

typedef struct UavType
{
	int id; // PK
	QString name;
} UavType;

typedef struct DeviceType
{
	int id; // PK
	QString name;
} DeviceType;

#endif // DBBLADEFINES_H
