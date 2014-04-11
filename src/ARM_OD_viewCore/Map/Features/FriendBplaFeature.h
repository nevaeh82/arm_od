#ifndef BLAFEATURE_H
#define BLAFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Friend BPLA feature draw in map
class FriendBpla : public Marker
{
	friend class FeaturesFactory;

protected:
	Path* m_tail;

	FriendBpla(IObjectsFactory* factory, const QString& id, int bplaId , QPointF position);

public:
	virtual ~FriendBpla() {}

	virtual void setName(const QString &name);
	virtual void setPosition(const QPointF &position);

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // BLAFEATURE_H
