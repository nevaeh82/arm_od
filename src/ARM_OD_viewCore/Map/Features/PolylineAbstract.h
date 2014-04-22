#ifndef POLYLINEABSTRACT_H
#define POLYLINEABSTRACT_H

#include <QString>
#include <QPointF>

#include <PwGis/PwGisLonLat.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/MapObjectAbstract.h>

namespace MapFeature {

class FeaturesFactory;

class PolylineAbstract : public MapObjectAbstract
{
	Q_OBJECT
	friend class FeaturesFactory;

protected:
	IObjectsFactory* m_factory;
	//PwGisLonLat m_position;

	PolylineAbstract( IObjectsFactory* factory, const QString& id,
		const QString& name, QObject* parent = 0 );

public:
	virtual ~PolylineAbstract() {}

	//virtual void setPosition(const QPointF& position);
	//virtual QPointF position();

public slots:
	virtual void updateMap() = 0;
	virtual void removeFromMap() = 0;
};

} // namespace MapFeature

#endif // POLYLINEABSTRACT_H
