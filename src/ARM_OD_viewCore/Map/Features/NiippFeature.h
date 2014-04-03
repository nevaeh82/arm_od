#ifndef NIIPPFEATURE_H
#define NIIPPFEATURE_H

#include <QString>
#include <QMap>
#include <QDebug>

#include <PwGis/pwgiswidget.h>
#include <PwGis/PwGisLonLat.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// Niipp feature draws in PwGis map
class Niipp
{

public:
	Niipp( PwGisWidget* pwwidget, QString niippLayerId, QString niippPointLayerId );
	~Niipp();

	//radius - in projection EPSG:900913 is pseudo meters
	//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
	void updateSector( int id, double radius, double bis, QByteArray ba );
	void updateCicle( int id, double radius, QByteArray ba );

	void addPoint( double lon, double lat );

private:
	PwGisWidget* m_pwwidget;
	QMap< QString, PwGisLonLat* > m_mapNiippSector;
	QMap< QString, PwGisLonLat* > m_mapNiippCircle;
};

} // namespace MapFeature

#endif // NIIPPFEATURE_H
