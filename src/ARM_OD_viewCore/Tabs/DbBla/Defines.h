#ifndef DBBLADEFINES_H
#define DBBLADEFINES_H

#include <QtGlobal>
#include <QTime>
#include <QDateTime>

typedef struct Bla
{
	uint blaId;
	QString ip;
	uint type; // FK
	QString name;
} Bla;

typedef struct BlaInfo
{
	uint bladId; // FK
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

typedef struct Device
{
	uint deviceId; // FK
	uint port;
	uint blaId;
} Device;

typedef struct BlaMission
{
	uint bladId; // FK
	uint targetId; // FK
	double regionCenterLat;
	double regionCenterLon;
	double regionCenterAlt;
	double regionRadius;
	QTime timeToTarget;
} BlaMission;

typedef struct Target
{
	QString ip;
	uint port;
	uint targetId;
	uint type; // FK
} Target;

// Dictionaries
typedef struct Status
{
	QString status;
} Status;

typedef struct TargetType
{
	QString name;
} TargetType;

typedef struct BlaType
{
	QString name;
} BlaType;

typedef struct Device
{
	QString name;
} Device;

/// parameters for connect to mysql
typedef struct DBConnectionStruct
{
	QString host;
	unsigned short port;
	QString login;
	QString password;
	QString dbName;
}DBConnectionStruct;

#endif // DBBLADEFINES_H
