#ifndef PWGISCLIENT_RECTANGLE_H
#define PWGISCLIENT_RECTANGLE_H
#include <QObject>
#include "../PwGisClient.h"
#include "PrimitiveMapObjectAbstract.h"
#include "../PwGisLonLat.h"

class PwGisLonLat;
class IMapFunctions;
class MapObjectsFactory;

namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Rectangle: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{837915eb-5c1c-4f5d-95d2-406735ebb3f9}")
	Q_CLASSINFO("InterfaceID", "{a14b957b-2bc8-4154-be8d-91c390f86f71}")
	Q_CLASSINFO("CoClassAlias", "PwGisRectangle")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(PwGisLonLat* bottomLeft READ bottomLeft WRITE setBottomLeft)
	Q_PROPERTY(PwGisLonLat* topRight READ topRight WRITE setTopRight)

	friend class ::MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _bottomLeft;
	PwGisLonLat* _topRight;

protected:
	explicit Rectangle(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:
	explicit Rectangle(QString id, QString name = "", QObject* parent = NULL);
	explicit Rectangle(QObject* parent);

	virtual ~Rectangle();
	virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* topRight() const;
	PwGisLonLat* bottomLeft() const;

	void setToolTip(QString arg);
	void setTopRight(PwGisLonLat* topRight);
	void setBottomLeft(PwGisLonLat* bottomLeft);

//public slots:

};

} //Gis
} //Pw

#endif // PWGISCLIENT_RECTANGLE_H
