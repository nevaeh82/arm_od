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
	QString m_nameTranslated;
	QString m_originName;

	QString m_styleName;
	QString m_trackStyleName;

	inline void updateName() { setName( m_originName ); }
	virtual inline QString getStyleName() const { return m_styleName; }
	virtual inline QString getTrackStyleName() const { return m_trackStyleName; }
	virtual inline QString getSliceStyleName() const { return m_trackStyleName; }

	EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

public:
	virtual ~EnemyBpla() {}

	virtual void setName(const QString &name);

	virtual void update(const UavInfo &uav);
};

} // namespace MapFeature

#endif // BPLAFEATURE_H
