#ifndef SECTOR_H
#define SECTOR_H
#include <QString>
#include <QtGui/QPainter>

#include "PrimitiveMapObjectAbstract.h"

class IMapProvider;
class IMapObject;
class PwGisLonLat;
class MapObjectsFactory;

/// Sector tactic map object
namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Sector : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{5eeb7c7c-d5ce-4a94-88f5-de08670f957d}")
	Q_CLASSINFO("InterfaceID", "{8bb52679-f263-4141-b73e-f1f0b9ffba30}")
	Q_CLASSINFO("CoClassAlias", "PwGisSector")

	Q_PROPERTY (PwGisLonLat* originPoint READ originPoint WRITE setOriginPoint)
	Q_PROPERTY (double radius READ radius WRITE setRadius)
	Q_PROPERTY (double startAngle READ startAngle WRITE setStartAngle)
	Q_PROPERTY (double endAngle READ endAngle WRITE setEndAngle)
	Q_PROPERTY (QString toolTip READ toolTip WRITE setToolTip)

	friend class ::MapObjectsFactory;

private:
	PwGisLonLat* _originPoint;
	double _radius;
	double _startAngle;
	double _endAngle;
	QString _toolTip;

	inline QRadialGradient parseGradient(QVariant var);

protected:
	explicit Sector(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Sector(QString id, QString name = "", QObject *parent = NULL);
	explicit Sector(QObject *parent);

	virtual void updateMap();

	PwGisLonLat* originPoint() const;

	double radius() const;

	double startAngle() const;

	double endAngle() const;

	QString toolTip() const;

	~Sector();

public slots:

	void setOriginPoint(const PwGisLonLat* originPoint);

	void setRadius(double radius);

	void setStartAngle(double startAngle);

	void setEndAngle(double endAngle);

	void setToolTip(QString toolTip);
};

} //Gis
} //Pw

#endif // SECTOR_H
