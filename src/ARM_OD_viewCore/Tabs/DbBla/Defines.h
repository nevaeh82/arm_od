#ifndef DBBLADEFINES_H
#define DBBLADEFINES_H

#include <QtGlobal>
#include <QTime>
#include <QDateTime>
#include <QMetaType>
#include "UAVDefines.h"

#define FRIEND_UAV_ROLE "friendUavRole"
#define OUR_UAV_ROLE "ourUavRole"
#define ENEMY_UAV_ROLE "enemyUavRole"

#define ENEMY_UAV_ID_OFFSET 50

typedef struct Uav
{
	int id; // PK
	uint uavId;
	QString ip;
	uint uavTypeId; // FK
	uint roleId; // FK
	QString name;
	uint freqId;
	bool historical;

	Uav(){
		freqId = 0;
		historical = false;
	}

} Uav;

Q_DECLARE_METATYPE( Uav )

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
	uint source; // FK
	double lat;
	double lon;
	double alt;
	double speed;
	double yaw;
	QTime restTime;
	uint statusId; // FK
	QDateTime dateTime;
	bool historical;

	UavInfo(){
		historical = false;
	}
} UavInfo;

typedef struct Devices
{
	int id; // PK
	uint deviceId; // FK
	uint port;
	uint uavId;
} Devices;

typedef struct Source
{
	int id; // PK
	uint sourceTypeId; // FK
	uint sourceId;
} Source;

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
	int uavId;
	QString ip;
	uint port;
	int type; // FK
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

typedef struct SourceType
{
	int id; // PK
	QString name;
} SourceType;
#endif // DBBLADEFINES_H
