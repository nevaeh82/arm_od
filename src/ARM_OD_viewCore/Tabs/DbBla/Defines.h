#ifndef DBBLADEFINES_H
#define DBBLADEFINES_H

#include <QtGlobal>
#include <QTime>
#include <QDateTime>

typedef struct Bla
{
	int id; // PK
	uint blaId;
	QString ip;
	uint type; // FK
	QString name;
} Bla;

typedef struct BlaInfo
{
	uint id; // PK
	uint blaId; // FK
	uint device; // FK
	double lat;
	double lon;
	double alt;
	double speed;
	double yaw;
	QTime restTime;
	uint statusId; // FK
	QDateTime dateTime;
} BlaInfo;

typedef struct Devices
{
	uint id; // PK
	uint deviceId; // FK
	uint port;
	uint blaId;
} Devices;

typedef struct BlaMission
{
	uint id; // PK
	uint blaId; // FK
	uint targetId; // FK
	double regionCenterLat;
	double regionCenterLon;
	double regionCenterAtitude;
	double regionRadius;
	QTime timeToTarget;
} BlaMission;

typedef struct Target
{
	uint id; // PK
	QString ip;
	uint port;
	uint targetId;
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

typedef struct BlaType
{
	uint id; // PK
	QString name;
} BlaType;

typedef struct DeviceType
{
	uint id; // PK
	QString name;
} DeviceType;

#endif // DBBLADEFINES_H
