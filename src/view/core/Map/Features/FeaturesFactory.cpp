#include "FeaturesFactory.h"

namespace MapFeature {

FeaturesFactory::FeaturesFactory(IMapProvider* provider, IMapStyleManager* styleManager,
								 QObject* parent)
	: QObject( parent )
	, m_provider( provider )
	, m_styleManager( styleManager )
{
}

Ais* FeaturesFactory::createAis(const QString& name, const QPointF& position, double course)
{
	return new Ais( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
					name, position, course );
}

FriendBpla* FeaturesFactory::createFriendBpla(const UavInfo& uav)
{
	return new FriendBpla( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(), uav );
}

EnemyBpla* FeaturesFactory::createEnemyBpla(const UavInfo& uav)
{
	return new EnemyBpla( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(), uav );
}

Interception* FeaturesFactory::createInterception(int friendBplaId, int enemyBplaId,
												  const QPointF& position, float height,
												  float radius, float angle, float speed)
{
	return new Interception( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
							 friendBplaId, enemyBplaId, position, height, radius, angle,
							 speed);
}

Niipp* FeaturesFactory::createNiipp( const ::Niipp& niipp )
{
	return new Niipp( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(), niipp );
}

NiippPoint* FeaturesFactory::createNiippPoint(const QPointF& position)
{
	return new NiippPoint( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
						   position );
}

Station* FeaturesFactory::createStation(const QString& name, const QPointF& position)
{
	return new Station( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
						name, position );
}

Pelengator *FeaturesFactory::createPelengator(int id, const QPointF& position, double angle)
{
	return new Pelengator( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
						   id, position, angle );
}

PelengatorPoint *FeaturesFactory::createPelengatorPoint(const QString& name, const QPointF& position)
{
	return new PelengatorPoint( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
								name, position );
}

CheckPoint* FeaturesFactory::createCheckPoint(const QString& name, const QPointF& position)
{
	return new CheckPoint( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
								name, position );
}

Hyperbole* FeaturesFactory::createHyperbole(const QVector<QPointF>& polyline, const QTime time, const QColor color)
{
	return new Hyperbole(
		m_provider->styleFactory(),
		m_provider->objectsFactory(),
		m_provider->idGenerator()->GetNewID(),
		polyline,
		time,
				color );
}

ADSBPlaneFeature*FeaturesFactory::createAdsbPlane(const QString& name, const QPointF& pos)
{
	return new ADSBPlaneFeature( m_provider->objectsFactory(), name, pos );
}

} // namespace MapFeature
