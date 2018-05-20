#ifndef PATH_H
#define PATH_H
#include <QMutex>
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisPointList;
class MapObjectsFactory;

namespace Pw {
namespace Gis {
class GISCLIENT_EXPORT Path : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{cc47d774-0169-4aa4-abc2-5e879dac7a17}")
	Q_CLASSINFO("InterfaceID", "{03fae852-773c-4f52-8648-351a75baeeb1}")
	Q_CLASSINFO("CoClassAlias", "PwGisPath")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)

private:
	QString _toolTip;
	PwGisPointList* _path;
	QMutex _mutex;

	friend class ::MapObjectsFactory;

protected:
	explicit Path(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Path(QString id, QString name = "", QObject* parent = NULL);
	explicit Path(QObject* parent);

	virtual ~Path();
	virtual void updateMap();
	QString toolTip() const;

public slots:
	PwGisPointList* points() const;

	void setToolTip(QString arg);
};

} //Gis
} //Pw

#endif // PATH_H
