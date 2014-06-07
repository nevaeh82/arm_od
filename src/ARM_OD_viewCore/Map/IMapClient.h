#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QObject>
#include <MapManager.h>
#include <pwgiswidget.h>
#include <IMapBounds.h>
#include <GeoRectangle.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGisPointList.h>
#include <QPointF>
#include <QByteArray>

#include "NIIPP/Niipp.h"
#include "Tabs/DbBla/Defines.h"

#include <PwGis/PwGisPointList.h>
#include <QTime>
#include <QColor>


struct DataFly {
	QString height;
	QString latitute;
	QString longinude;
	QString speed;
	QString namePlane;
	QString course;
};


class IMapClient
{
public:
	virtual void init() = 0;
	virtual void setPoint() = 0;
	virtual void centerMap() = 0;
	virtual void justifyMap() = 0;

	virtual void showLayer( int index, bool state ) = 0;

	virtual void addFriendBpla( const UavInfo& uav ) = 0;
	virtual void addEnemyBpla( const UavInfo& uav ) = 0;
	virtual void removeBpla( const Uav& uav ) = 0;

	virtual void addAis( QMap<int, QVector<QString> > vec ) = 0;

	virtual void addNiippPoint( const QPointF& point ) = 0;
	virtual void removeNiippPoint() = 0;

	virtual void updateNiippPowerZone( const Niipp& niipp ) = 0;

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction ) = 0;

	virtual void addInterception( int blaId, int bplaId, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList ) = 0;
	virtual void removeInterception( int blaId, int bplaId ) = 0;

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord, float hgt, float radius, int time, float intcCourse, float intcSpeed ) = 0;

	virtual void addHyperbole( QByteArray data, const QTime time, const QColor color = QColor::Invalid ) = 0;

	virtual void removeAll() = 0;
};

#endif // IMAPCLIENT_H
