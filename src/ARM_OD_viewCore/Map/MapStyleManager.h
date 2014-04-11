#ifndef MAPSTYLEFACTORY_H
#define MAPSTYLEFACTORY_H

#include <QMap>
#include <QString>

#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/IStyleFactory.h>

#include "Map/IMapStyleManager.h"

class MapStyleManager : public IMapStyleManager
{
private:
	IStyleFactory* m_factory;
	QMap<QString, PwGisStyle*> m_styles;

public:
	MapStyleManager(IStyleFactory *factory);

	virtual PwGisStyle* createAisStyle(const QString& layerName);
	virtual PwGisStyle* createFriendBplaStyle(const QString& layerName);
	virtual PwGisStyle* createEnemyBplaStyle(const QString& layerName);
	virtual PwGisStyle* createInterceptionStyle(const QString& layerName);
	virtual PwGisStyle* createNiippStyle(const QString& layerName);
	virtual PwGisStyle* createNiippPointsStyle(const QString& layerName);
	virtual PwGisStyle* createPelengatorStyle(const QString& layerName);
	virtual PwGisStyle* createPelengatorPointsStyle(const QString& layerName);
	virtual PwGisStyle* createStationStyle(const QString& layerName);

	inline virtual PwGisStyle* getAisStyle() { return getStyle( MAP_STYLE_NAME_AIS ); }
	inline virtual PwGisStyle* getFriendBplaStyle() { return getStyle( MAP_STYLE_NAME_FRIEND_BPLA ); }
	inline virtual PwGisStyle* getEnemyBplaStyle() { return getStyle( MAP_STYLE_NAME_ENEMY_BPLA ); }
	inline virtual PwGisStyle* getInterceptionStyle() { return getStyle( MAP_STYLE_NAME_INTERCEPTION ); }
	inline virtual PwGisStyle* getNiippStyle() { return getStyle( MAP_STYLE_NAME_NIIPP ); }
	inline virtual PwGisStyle* getNiippPointsStyle() { return getStyle( MAP_STYLE_NAME_NIIPP_POINTS ); }
	inline virtual PwGisStyle* getPelengatorStyle() { return getStyle( MAP_STYLE_NAME_PELENGATOR ); }
	inline virtual PwGisStyle* getPelengatorPointsStyle() { return getStyle( MAP_STYLE_NAME_PELENGATOR_POINTS ); }
	inline virtual PwGisStyle* getStationStyle() { return getStyle( MAP_STYLE_NAME_STATION ); }

protected:
	/// Creates style with \a name
	/// \return false, is style was created before
	PwGisStyle* createStyle(const QString& name, bool* exists);

	/// Returns style from style map, associated with \a name,
	/// otherwise NULL
	PwGisStyle* getStyle(const QString& name, bool log = true);

	/// Adds \a style to map, associated with \a name
	void setStyle(const QString& name, PwGisStyle* style);
};

#endif // MAPSTYLEFACTORY_H
