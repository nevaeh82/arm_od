#ifndef FEATURESFACTORY_H
#define FEATURESFACTORY_H

#include <QObject>

#include <PwGis/IMapProvider.h>

#include "Map/Features/IFeaturesFactory.h"
#include "Map/IMapStyleManager.h"

namespace MapFeature {

class FeaturesFactory : public QObject, public IFeaturesFactory
{
	Q_OBJECT

protected:
	IMapProvider* m_provider;
	IMapStyleManager* m_styleManager;

public:
	FeaturesFactory(IMapProvider* provider, IMapStyleManager* styleManager,
					QObject* parent = NULL);

	virtual Ais*          createAis( const QString& name, const QPointF& position, double course );
	virtual FriendBpla*   createFriendBpla( int id, const QPointF& position );
	virtual EnemyBpla*    createEnemyBpla( int id, const QPointF& speed, const QVector<QPointF> track, double altitude );
	virtual Interception* createInterception( int friendBplaId, int enemyBplaId, const QPointF& position, float height, float radius, float angle, float speed );
	virtual Niipp*        createNiipp( int id, const QPointF& position, Niipp::Mode mode, double radius, double angle = 0 );
	virtual NiippPoint*   createNiippPoint( const QPointF& position = QPointF() );
	virtual Station*      createStation( const QString& name, const QPointF& position = QPointF() );
	virtual Pelengator*   createPelengator(int id, const QPointF& position, double angle );
	virtual PelengatorPoint* createPelengatorPoint( const QString& name, const QPointF& position );
	virtual CheckPoint*   createCheckPoint( const QString& name, const QPointF& position );
};

} // namespace MapFeature

#endif // FEATURESFACTORY_H
