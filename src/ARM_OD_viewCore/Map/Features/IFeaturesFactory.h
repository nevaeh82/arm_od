#ifndef IFEATURESFACTORY_H
#define IFEATURESFACTORY_H

#include <QString>
#include <QPointF>
#include <QVector>

#include <PwGis/objects/MapObjectsFactory.h>

#include "Map/Features/AisFeature.h"
#include "Map/Features/EnemyBplaFeature.h"
#include "Map/Features/FriendBplaFeature.h"
#include "Map/Features/InterceptionFeature.h"
#include "Map/Features/NiippFeature.h"
#include "Map/Features/NiippPointFeature.h"
#include "Map/Features/PelengatorFeature.h"
#include "Map/Features/PelengatorPointFeature.h"
#include "Map/Features/StationFeature.h"
#include "Map/Features/CheckPointFeature.h"

namespace MapFeature {

class IFeaturesFactory
{
public:
	virtual ~IFeaturesFactory() {}

	virtual Ais*          createAis( const QString& name, const QPointF& position, double course ) = 0;
	virtual FriendBpla*   createFriendBpla( int id, const QPointF& position ) = 0;
	virtual EnemyBpla*    createEnemyBpla( int id, const QPointF& speed, const QVector<QPointF> track, double altitude ) = 0;
	virtual Interception* createInterception( int friendBplaId, int enemyBplaId, const QPointF& position, float height, float radius, float angle, float speed ) = 0;
	virtual Niipp*        createNiipp( int id, const QPointF& position, Niipp::Mode mode, double radius, double angle = 0 ) = 0;
	virtual NiippPoint*   createNiippPoint( const QPointF& position = QPointF() ) = 0;
	virtual Station*      createStation( const QString& name, const QPointF& position = QPointF() ) = 0;
	virtual Pelengator*   createPelengator( int id, const QPointF& position, double angle ) = 0;
	virtual PelengatorPoint* createPelengatorPoint( const QString& name, const QPointF& position ) = 0;
	virtual CheckPoint*   createCheckPoint( const QString& name, const QPointF& position ) = 0;
};

} // namespace MapFeature

#endif // IFEATURESFACTORY_H
