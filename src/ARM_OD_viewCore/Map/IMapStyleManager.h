#ifndef IMAPSTYLEMANAGER_H
#define IMAPSTYLEMANAGER_H

#include <QString>

#include <PwGis/objects/PwGisStyle.h>

#define MAP_STYLE_NAME_AIS					"AIS"
#define MAP_STYLE_NAME_FRIEND_BPLA			"FriendBPLA"
#define MAP_STYLE_NAME_ENEMY_BPLA			"EmenyBPLA"
#define MAP_STYLE_NAME_INTERCEPTION			"Interception"
#define MAP_STYLE_NAME_NIIPP				"Niipp"
#define MAP_STYLE_NAME_NIIPP_POINTS			"NiippPoints"
#define MAP_STYLE_NAME_PELENGATOR			"Pelengator"
#define MAP_STYLE_NAME_PELENGATOR_POINTS	"PelengatorPoints"
#define MAP_STYLE_NAME_STATION				"Station"

class IMapStyleManager
{
public:
	virtual ~IMapStyleManager() {}

	virtual PwGisStyle* createAisStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createFriendBplaStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createEnemyBplaStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createInterceptionStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createNiippStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createNiippPointsStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createPelengatorStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createPelengatorPointsStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createStationStyle(const QString& layerName) = 0;

	virtual PwGisStyle* getAisStyle() = 0;
	virtual PwGisStyle* getFriendBplaStyle() = 0;
	virtual PwGisStyle* getEnemyBplaStyle() = 0;
	virtual PwGisStyle* getInterceptionStyle() = 0;
	virtual PwGisStyle* getNiippStyle() = 0;
	virtual PwGisStyle* getNiippPointsStyle() = 0;
	virtual PwGisStyle* getPelengatorStyle() = 0;
	virtual PwGisStyle* getPelengatorPointsStyle() = 0;
	virtual PwGisStyle* getStationStyle() = 0;
};

#endif // IMAPSTYLEMANAGER_H
