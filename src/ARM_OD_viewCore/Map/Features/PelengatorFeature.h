#ifndef PELENGATORFEATURE_H
#define PELENGATORFEATURE_H

#include <QString>
#include <QMap>

#include <PwGis/pwgiswidget.h>
#include <PwGis/PwGisLonLat.h>
#include <PwGis/PwGisPointList.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// Pelengator feature draws in PwGis map
class Pelengator
{

public:
	Pelengator( PwGisWidget* pwwidget, QMap< int, PwGisPointList* > *lastCoord,
		QString pelengatorLayerId, QString pelengatorPointLayerId );
	~Pelengator();

	//radius - in projection EPSG:900913 is pseudo meters
	//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
	void updatePeleng( int postId, double lat, double lon, double direction );
	void setPointEvilPeleng( int id, QPointF point );

private:
	PwGisWidget* m_pwwidget;
	QMap< int, PwGisPointList* > *m_lastCoord;
	QMap<QString, PwGisLonLat *> m_mapPeleng;
	QMap<int, int> m_mapPelengPoint;
};

} // namespace MapFeature

#endif // PELENGATORFEATURE_H
