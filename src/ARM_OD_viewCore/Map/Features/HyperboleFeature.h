#ifndef HYPERBOLEFEATURE_H
#define HYPERBOLEFEATURE_H

#include <QTime>
#include "Map/Features/PolylineAbstract.h"


namespace MapFeature {

class FeaturesFactory;

class Hyperbole : public PolylineAbstract
{
	friend class FeaturesFactory;

protected:
	QTime m_timeMeasure;

	Hyperbole( IObjectsFactory* factory, const QString& id,
		const QString& name, PwGisPointList* polyline,
		const QTime timeMeasure );

public:
	virtual ~Hyperbole();

	virtual void updateMap();
	virtual void removeFromMap();

	QTime timeMeasure() const { return m_timeMeasure; }
};

} // namespace MapFeature

#endif // HYPERBOLEFEATURE_H
