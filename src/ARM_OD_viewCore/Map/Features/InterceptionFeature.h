#ifndef PEREHVATFEATURE_H
#define PEREHVATFEATURE_H

#include <QString>
#include <QMap>

#include <PwGis/pwgiswidget.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// Interception feature draws in PwGis map
class Interception
{

public:
	Interception( PwGisWidget* pwwidget, QString layerId );
	~Interception();

	//radius - in projection EPSG:900913 is pseudo meters
	//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
	void addPointData( int blaId, int bplaId, QPointF coord,
		float hgt, float radius, float intcCourse, float intcSpeed );

private:
	PwGisWidget* m_pwwidget;
};

} // namespace MapFeature

#endif // PEREHVATFEATURE_H
