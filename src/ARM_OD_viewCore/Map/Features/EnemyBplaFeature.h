#ifndef BPLAFEATURE_H
#define BPLAFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Enemy BPLA feature in map
class EnemyBpla : public Marker
{
	friend class FeaturesFactory;

protected:
	Path* m_tail;
	QString m_originName;
	QVector<QPointF> m_track;
	double m_altitude;
	QPointF m_speed;

	EnemyBpla(IObjectsFactory* factory, const QString& id, int bplaId);

	void inline updateName() { setName( m_originName ); }

public:
	virtual ~EnemyBpla();

	virtual void setName(const QString &name);
	virtual void setPosition(const QPointF &position);

	void setAltitude(double altitude);
	inline double altitude() { return m_altitude; }

	void setTrack(const QVector<QPointF>& track);
	inline const QVector<QPointF>& track() { return m_track; }

	void setSpeed(const QPointF& speed);
	inline const QPointF& speed() { return m_speed; }

	void update(double altitude, const QPointF& speed, const QVector<QPointF>& track);

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // BPLAFEATURE_H
