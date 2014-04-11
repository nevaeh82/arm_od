#ifndef IFEATURESFACTORY_H
#define IFEATURESFACTORY_H

#include <PwGis/objects/MapObjectsFactory.h>

#include "Map/Features/AisFeature.h"
#include "Map/Features/EnemyBplaFeature.h"
#include "Map/Features/FriendBplaFeature.h"
#include "Map/Features/InterceptionFeature.h"
#include "Map/Features/NiippFeature.h"
#include "Map/Features/PelengatorFeature.h"
#include "Map/Features/StationFeature.h"

namespace MapFeature {

class IFeaturesFactory
{
public:
	virtual ~IFeaturesFactory() {}

	virtual Ais* createAis(const QString& name, const QPointF& position, double course) = 0;
	virtual FriendBpla* createFriendBpla(int id, const QPointF& position) = 0;
};

} // namespace MapFeature

#endif // IFEATURESFACTORY_H
