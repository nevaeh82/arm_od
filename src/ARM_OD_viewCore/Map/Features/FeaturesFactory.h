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

	virtual Ais* createAis(const QString& name, const QPointF& position, double course);
	virtual FriendBpla* createFriendBpla(int id, const QPointF& position);
	virtual EnemyBpla* createEnemyBpla(int id, const QPointF& speed, const QVector<QPointF> track,
									   double altitude);
	virtual Interception* createInterception( int friendBplaId, int enemyBplaId, QPointF position,
		float height, float radius, int time, float course, float speed );
};

} // namespace MapFeature

#endif // FEATURESFACTORY_H
