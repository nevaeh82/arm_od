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

FriendBpla*FeaturesFactory::createFriendBpla(int id, const QPointF& position)
{
	return new FriendBpla( m_provider->objectsFactory(), m_provider->idGenerator()->GetNewID(),
					id, position );
}

} // namespace MapFeature
