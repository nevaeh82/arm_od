#ifndef CHECKPOINTFEATURE_H
#define CHECKPOINTFEATURE_H

#include <PwGis/objects/Rectangle.h>

#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

class FeaturesFactory;

class CheckPoint : public FeatureAbstract
{
	friend class FeaturesFactory;

protected:
	Pw::Gis::Rectangle* m_rectangle;

	CheckPoint(IObjectsFactory* factory, const QString& id, const QString& name,
			   const QPointF& position);

public:
	virtual ~CheckPoint();

	virtual void setName(QString name);
	virtual void setPosition(const QPointF &position);

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // CHECKPOINTFEATURE_H
