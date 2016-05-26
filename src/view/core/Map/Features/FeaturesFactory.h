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
	virtual FriendBpla*   createFriendBpla( const UavInfo& uav );
	virtual EnemyBpla*    createEnemyBpla( const UavInfo& uav );
	virtual Interception* createInterception( int friendBplaId, int enemyBplaId, const QPointF& position, float height, float radius, float angle, float speed );
	virtual Niipp*        createNiipp( const ::Niipp& niipp );
	virtual NiippPoint*   createNiippPoint( const QPointF& position = QPointF() );
	virtual Station*      createStation( const QString& name, const QPointF& position = QPointF() );
	virtual Pelengator*   createPelengator(int id, const QPointF& position, double angle );
	virtual PelengatorPoint* createPelengatorPoint( const QString& name, const QPointF& position );
	virtual CheckPoint*   createCheckPoint( const QString& name, const QPointF& position );
	virtual Hyperbole*    createHyperbole( const QVector<QPointF>& polyline, const QTime time, const QColor color = QColor::Invalid );
	virtual ADSBPlaneFeature* createAdsbPlane(const QString& name, const QPointF& pos);
};

} // namespace MapFeature

#endif // FEATURESFACTORY_H
