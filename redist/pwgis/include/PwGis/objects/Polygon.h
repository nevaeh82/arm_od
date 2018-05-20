#ifndef POLYGON_H
#define POLYGON_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisPointList;
class MapObjectsFactory;

namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Polygon : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{6bd96930-23a2-4ed3-9eb8-10625e1dca00}")
	Q_CLASSINFO("InterfaceID", "{2a3507fa-b082-4820-990d-890c1bd78f8a}")
	Q_CLASSINFO("CoClassAlias", "PwGisPolygon")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)

	friend class ::MapObjectsFactory;

private:
	QString _toolTip;
	PwGisPointList* _corners;

protected:
	explicit Polygon(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Polygon(QString id, QString name = "", QObject* parent = NULL);
	explicit Polygon(QObject* parent = NULL);

	virtual ~Polygon();
	virtual void updateMap();
	QString toolTip() const;
	void setToolTip(QString arg);

public slots:
	PwGisPointList* corners() const;

};

} //Gis
} //Pw

#endif // POLYGON_H
