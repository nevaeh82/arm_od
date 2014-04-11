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

FriendBpla* FeaturesFactory::createFriendBpla(int id, const QPointF& position)
{
	return new FriendBpla( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
						   id, position );
}

EnemyBpla* FeaturesFactory::createEnemyBpla(int id, const QPointF& speed,
											const QVector<QPointF> track, double altitude)
{
	EnemyBpla* bpla = new EnemyBpla( m_provider->objectsFactory(),
									 m_provider->idGenerator()->GetNewID(), id );

	bpla->setSpeed( speed );
	bpla->setTrack( track );
	bpla->setAltitude( altitude );

	return bpla;
}

Interception* FeaturesFactory::createInterception(int friendBplaId, int enemyBplaId,
												  const QPointF& position, float height,
												  float radius, float angle, float speed)
{
	return new Interception( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
							 friendBplaId, enemyBplaId, position, height, radius, angle,
							 speed);
}

Niipp* FeaturesFactory::createNiipp(int id, const QPointF& position, Niipp::Mode mode, double radius,
									double angle)
{
	return new Niipp( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
					  id, position, mode, radius, angle );
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

} // namespace MapFeature
