#ifndef HYPERBOLEFEATURE_H
#define HYPERBOLEFEATURE_H

#include <QTime>
#include "Map/Features/PolylineAbstract.h"
#include <PwGis/objects/Path.h>


namespace MapFeature {

class FeaturesFactory;

class Hyperbole : public PolylineAbstract
{
	friend class FeaturesFactory;

protected:
	QString m_name;
	QTime m_timeMeasure;
	Path* m_path;

	Hyperbole( IObjectsFactory* factory, const QString& id,
		const QString& name, PwGisPointList* polyline,
		const QTime timeMeasure );

public:
	virtual ~Hyperbole();

	virtual void updateMap();
	virtual void removeFromMap();

	QString name() const { return m_name; }
	QTime timeMeasure() const { return m_timeMeasure; }

	void updatePolyline( PwGisPointList* polyline );
};

} // namespace MapFeature

#endif // HYPERBOLEFEATURE_H
