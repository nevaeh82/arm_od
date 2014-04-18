#ifndef IMAPSTYLEMANAGER_H
#define IMAPSTYLEMANAGER_H

#include <QString>

#include <PwGis/objects/PwGisStyle.h>

#define MAP_STYLE_NAME_AIS					"AIS"
#define MAP_STYLE_NAME_FRIEND_BPLA			"FriendBPLA"
#define MAP_STYLE_NAME_FRIEND_BPLA_SLICE	"FriendBPLA_Slice"
#define MAP_STYLE_NAME_ENEMY_BPLA			"EmenyBPLA"
#define MAP_STYLE_NAME_INTERCEPTION			"Interception"
#define MAP_STYLE_NAME_NIIPP				"Niipp"
#define MAP_STYLE_NAME_NIIPP_POINT			"NiippPoint"
#define MAP_STYLE_NAME_PELENGATOR			"Pelengator"
#define MAP_STYLE_NAME_PELENGATOR_POINT		"PelengatorPoint"
#define MAP_STYLE_NAME_STATION				"Station"
#define MAP_STYLE_NAME_GRID					"Grid"
#define MAP_STYLE_NAME_CHECK_POINTS			"Control_points"

class IMapStyleManager
{
public:
	virtual ~IMapStyleManager() {}

	virtual PwGisStyle* createAisStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createFriendBplaStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createFriendBplaSliceStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createEnemyBplaStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createInterceptionStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createNiippStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createNiippPointStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createPelengatorStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createPelengatorPointStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createStationStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createGridStyle(const QString& layerId) = 0;
	virtual PwGisStyle* createCheckPointStyle(const QString& layerId) = 0;

	virtual PwGisStyle* getAisStyle() = 0;
	virtual PwGisStyle* getFriendBplaStyle() = 0;
	virtual PwGisStyle* getFriendBplaSliceStyle() = 0;
	virtual PwGisStyle* getEnemyBplaStyle() = 0;
	virtual PwGisStyle* getInterceptionStyle() = 0;
	virtual PwGisStyle* getNiippStyle() = 0;
	virtual PwGisStyle* getNiippPointStyle() = 0;
	virtual PwGisStyle* getPelengatorStyle() = 0;
	virtual PwGisStyle* getPelengatorPointStyle() = 0;
	virtual PwGisStyle* getStationStyle() = 0;
	virtual PwGisStyle* getGridStyle() = 0;
	virtual PwGisStyle* getCheckpointsStyle() = 0;

};

#endif // IMAPSTYLEMANAGER_H
