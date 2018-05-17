#ifndef PRIMITIVEMAPOBJECTABSTRACT_H
#define PRIMITIVEMAPOBJECTABSTRACT_H

#include <QStringList>

#include "MapObjectAbstract.h"

class GISCLIENT_EXPORT PrimitiveMapObjectAbstract: public MapObjectAbstract
{
	Q_OBJECT

private:
	MapObjectStyle* _style;
	QStringList _styleIds;

protected:

	QString styleName();

	explicit PrimitiveMapObjectAbstract(IMapFunctions *map, QString id, QString name, QObject *parent = 0);

public:
	explicit PrimitiveMapObjectAbstract(QString id, QString name, QObject *parent = 0);
	explicit PrimitiveMapObjectAbstract(QObject *parent = 0);

	virtual void updateMap() = 0;

public slots:
	void setStyle(MapObjectStyle* style);
	void addStyle( PwGisStyle* style );
	void addStyleByName( const QString& name );
	void removeStyle( PwGisStyle* style );
	void removeStyleByName( const QString& name );
};

#endif // PRIMITIVEMAPOBJECTABSTRACT_H
