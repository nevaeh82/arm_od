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

#include "../NIIPP/INiippController.h"


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
	virtual void setPoint() = 0;
	virtual void centerMap() = 0;
	virtual void justifyMap() = 0;

	virtual void setNiippController( INiiPPController* niippController ) = 0;

	virtual void showLayer( int index, bool state ) = 0;

	virtual void addBLA( int id, QByteArray data ) = 0;
	virtual void addEvil( int id, QByteArray data ) = 0;
	virtual void addAis( QMap<int, QVector<QString> > vec ) = 0;

	virtual void updateNiippPowerSector( int id, double radius, double bis, QByteArray ba ) = 0;
	virtual void updateNiippPowerCicle( int id, double radius, QByteArray ba ) = 0;

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction ) = 0;

	virtual void addPerehvat( int blaId, int bplaId ) = 0;
	virtual void removePerehvat( int blaId, int bplaId ) = 0;

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord, float hgt, float radius, int time, float intcCourse, float intcSpeed ) = 0;

	virtual void removePointUvoda() = 0;
};

#endif // IMAPCLIENT_H
