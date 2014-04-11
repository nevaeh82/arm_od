#ifndef PELENGATORPOINTFEATURE_H
#define PELENGATORPOINTFEATURE_H

#include <QVector>

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Pelengator point feature representation on map
class PelengatorPoint : public Marker
{
	friend class FeaturesFactory;

protected:
	QVector<QPointF> m_track;

	PelengatorPoint(IObjectsFactory* factory, const QString& id, const QString& name,
					const QPointF& position);

public:
	virtual ~PelengatorPoint() {}

	virtual void setPosition(const QPointF &position);
};

} // namespace MapFeature

#endif // PELENGATORPOINTFEATURE_H
