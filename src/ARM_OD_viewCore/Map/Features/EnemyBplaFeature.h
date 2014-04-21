#ifndef BPLAFEATURE_H
#define BPLAFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/BplaAbstractFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Enemy BPLA feature representation on map
class EnemyBpla : public BplaAbstract
{
	friend class FeaturesFactory;

protected:
	QString m_originName;

	inline void updateName() { setName( m_originName ); }
	virtual inline QString getStyleName() const { return MAP_STYLE_NAME_ENEMY_BPLA; }
	virtual inline QString getTrackStyleName() const { return MAP_STYLE_NAME_ENEMY_BPLA_TRACK; }
	virtual inline QString getSliceStyleName() const { return MAP_STYLE_NAME_ENEMY_BPLA_TRACK; }

	EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

public:
	virtual ~EnemyBpla() {}

	virtual void setName(const QString &name);
};

} // namespace MapFeature

#endif // BPLAFEATURE_H
