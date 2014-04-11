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

	inline virtual PwGisStyle* getAisStyle() { return getStyle( MAP_STYLE_NAME_AIS ); }
	inline virtual PwGisStyle* getFriendBplaStyle() { return getStyle( MAP_STYLE_NAME_FRIEND_BPLA ); }

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
