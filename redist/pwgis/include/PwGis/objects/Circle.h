#ifndef CIRCLE_H
#define CIRCLE_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisLonLat;
class MapObjectsFactory;

namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Circle: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{de8cd2dd-31c7-49c4-9a28-cb37a7f09e07}")
	Q_CLASSINFO("InterfaceID", "{1870b9b3-5908-462a-a498-fdc0de1686f8}")
	Q_CLASSINFO("CoClassAlias", "PwGisCircle")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(PwGisLonLat* originPoint READ originPoint WRITE setOriginPoint)
	Q_PROPERTY(int radius READ radius WRITE setRadius)

	friend class ::MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _center;
	int _radius;

protected:
	explicit Circle(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Circle(QString id, QString name = "", QObject* parent = NULL);
	explicit Circle(QObject* parent = NULL);

	virtual ~Circle();
	virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* originPoint() const;
	void setOriginPoint(const PwGisLonLat* point);

	int radius() const;

	void setToolTip(QString arg);
	void setRadius(int radius);
public slots:

};

} //Gis
} //Pw

#endif // CIRCLE_H
