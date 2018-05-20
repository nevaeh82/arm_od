#ifndef MARKER_H
#define MARKER_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisLonLat;
class MapObjectsFactory;

namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Marker: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{58ecee45-b58b-4944-853e-512f44dce349}")
	Q_CLASSINFO("InterfaceID", "{14b1fc3d-4c54-4d23-a1ff-795b43f5d00b}")
	Q_CLASSINFO("CoClassAlias", "PwGisMarker")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(double rotate READ rotate WRITE setRotate)
	Q_PROPERTY(PwGisLonLat* position READ position WRITE setPosition)

	friend class ::MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _position;
	double _rotate;

protected:
	explicit Marker(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:
	explicit Marker(QString id, QString name = "", QObject* parent = NULL);
	explicit Marker(QObject* parent = NULL);

	virtual ~Marker();
	virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* position() const;
	void setPosition(const PwGisLonLat* position);
	double rotate() const;

public slots:

	void setToolTip(QString arg);
	void setRotate(double arg);
};

} //Gis
} //Pw

#endif // MARKER_H
