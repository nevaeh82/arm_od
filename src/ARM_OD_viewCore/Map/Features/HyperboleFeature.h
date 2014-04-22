#ifndef HYPERBOLEFEATURE_H
#define HYPERBOLEFEATURE_H

#include <QTime>
#include "Map/Features/PolylineAbstract.h"


namespace MapFeature {

class FeaturesFactory;

class HyperboleFeature : public PolylineAbstract
{
	friend class FeaturesFactory;

protected:
	QTime m_timeMeasure;

	HyperboleFeature( IObjectsFactory* factory, const QString& id,
		const QString& name, const Path* polyline,
		QTime timeMeasure );

public:
	virtual ~HyperboleFeature();

	virtual void updateMap();
	virtual void removeFromMap();

	QTime timeMeasure() const { return m_timeMeasure; }
};

} // namespace MapFeature

#endif // HYPERBOLEFEATURE_H
