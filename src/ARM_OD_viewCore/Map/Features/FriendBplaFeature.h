#ifndef BLAFEATURE_H
#define BLAFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/BplaAbstractFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Friend BPLA feature representation on map
class FriendBpla : public BplaAbstract
{
	friend class FeaturesFactory;

protected:
	inline void updateName() {}
	virtual inline QString getStyleName() const { return MAP_STYLE_NAME_FRIEND_BPLA; }
	virtual inline QString getTrackStyleName() const { return MAP_STYLE_NAME_FRIEND_BPLA_TRACK; }
	virtual inline QString getSliceStyleName() const { return MAP_STYLE_NAME_FRIEND_BPLA_SLICE; }

	FriendBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

public:
	virtual ~FriendBpla() {}

	virtual void setName(const QString &name);
};

} // namespace MapFeature

#endif // BLAFEATURE_H
