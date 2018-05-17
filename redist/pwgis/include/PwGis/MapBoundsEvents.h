#ifndef MAPBOUNDSEVENTS_H
#define MAPBOUNDSEVENTS_H

#include <QObject>
#include "PwGisClient.h"

namespace Pw { namespace Gis {

class GISCLIENT_EXPORT MapBoundsEvents : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID","{0dd44197-f9ed-42fe-9bf6-bcc52a5e0709}")
	Q_CLASSINFO("InterfaceID","{7504f94a-de2a-4516-80fd-5a716b9b691f}")
	Q_CLASSINFO("EventsID","{b2770f73-8a0f-4252-8ada-2063684501ff}")

public:
	explicit MapBoundsEvents(QObject *parent = 0);

signals:
	///  Signal Caused when map was zoomed
	void zoomChanged(int);
};

} //namespace Gis
} //namespace Pw

#endif // MAPBOUNDSEVENTS_H
