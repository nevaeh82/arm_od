#ifndef IMAPSTYLEMANAGER_H
#define IMAPSTYLEMANAGER_H

#include <QString>

#include <PwGis/objects/PwGisStyle.h>

#define MAP_STYLE_NAME_AIS				"AIS"
#define MAP_STYLE_NAME_FRIEND_BPLA		"FriendBPLA"

class IMapStyleManager
{
public:
	virtual ~IMapStyleManager() {}

	virtual PwGisStyle* createAisStyle(const QString& layerName) = 0;
	virtual PwGisStyle* createFriendBplaStyle(const QString& layerName) = 0;

	virtual PwGisStyle* getAisStyle() = 0;
	virtual PwGisStyle* getFriendBplaStyle() = 0;
};

#endif // IMAPSTYLEMANAGER_H
