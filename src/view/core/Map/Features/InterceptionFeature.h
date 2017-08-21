#ifndef PEREHVATFEATURE_H
#define PEREHVATFEATURE_H

#include <PwGis/objects/Circle.h>

#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

class FeaturesFactory;

/// Interseption feature representation on map
class Interception : public FeatureAbstract
{
	friend class FeaturesFactory;

protected:
	Pw::Gis::Circle* m_circle;

	const int m_friendBplaId;
	const int m_enemyBplaId;

	float m_height;
	float m_radius;
	float m_course;
	float m_speed;
    int m_time;

    Interception(IObjectsFactory* factory, const QString& id,
                 int friendBplaId, int enemyBplaId, const QPointF& position,
                 float height, float radius, float course, float speed, int time);

	void updatePresentation();

public:
	virtual ~Interception();

	virtual void setPosition(const QPointF &position);

	/// \param radius In projection EPSG:900913 is pseudo meters. \
	///					Must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	/// \link http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
	void setRadius(float value);
	void setHeight(float value);
	void setCourse(float value);
	void setSpeed(float value);

	inline float radius() { return m_radius; }
	inline float height() { return m_height; }
	inline float course() { return m_course; }
	inline float speed() { return m_speed; }

	void update(const QPointF& position, float height, float radius, float course, float speed);

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // PEREHVATFEATURE_H
