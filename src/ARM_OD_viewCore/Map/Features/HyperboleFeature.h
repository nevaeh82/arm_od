#ifndef HYPERBOLEFEATURE_H
#define HYPERBOLEFEATURE_H

#include <QTime>
#include <QColor>
#include "Map/IMapStyleManager.h"
#include "Map/Features/PolylineAbstract.h"
#include <PwGis/objects/Path.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/IStyleFactory.h>


namespace MapFeature {

class FeaturesFactory;

class Hyperbole : public PolylineAbstract
{
	friend class FeaturesFactory;

protected:
	IStyleFactory* m_styleFactory;
	QTime m_timeMeasure;

	Hyperbole(
		IStyleFactory* styleFactory,
		IObjectsFactory* factory,
		const QString& id,
		const QString& name,
		PwGisPointList* polyline,
		const QTime timeMeasure,
		const QColor color );

public:
	virtual ~Hyperbole();

	virtual void updateMap();
	virtual void removeFromMap();

	QTime timeMeasure() const { return m_timeMeasure; }

	void updatePath( PwGisPointList* polyline, const QTime timeMeasure );

};

} // namespace MapFeature

#endif // HYPERBOLEFEATURE_H
