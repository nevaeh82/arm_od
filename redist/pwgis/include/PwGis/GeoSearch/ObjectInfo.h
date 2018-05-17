#ifndef GEOSEARCH_OBJECTINFO_H
#define GEOSEARCH_OBJECTINFO_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>

#include "Common.h"
#include "ObjectInfoType.h"

namespace Pw
{
namespace Gis
{
namespace GeoSearch
{

typedef QMap<QString, QString> ObjectInfoAttributes;

class ObjectInfo : public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{14C881AB-0685-49EF-BC4D-29CD867F708F}")
	Q_CLASSINFO("InterfaceID", "{84AAA674-E975-4C69-B90C-0D3A167874D9}")
	Q_CLASSINFO("CoClassAlias", "PwGisSearchObjectInfo")

	/** @brief "Unique geoobject ID for current map" */
	AX_PROPERTY( QString, _id, id, setId )
	Q_PROPERTY( QString id READ id WRITE setId )

	/** @brief Unified PwGis type of geoobject */
	AX_PROPERTY( Pw::Gis::GeoSearch::ObjectInfoType::Enum, _type, type, setType )
	Q_PROPERTY( Pw::Gis::GeoSearch::ObjectInfoType::Enum type READ type WRITE setType )

	/** @brief Custom type of geoobject (used when TypeEnum doesn't contains suitable type) */
	AX_PROPERTY( QString, _customType, customType, setCustomType )
	Q_PROPERTY( QString	customType READ customType WRITE setCustomType )

	/** @brief Full name of geoobject (ideally contains parent locations, e.g. country & city) */
	AX_PROPERTY( QString, _name, name, setName )
	Q_PROPERTY( QString	name READ name WRITE setName )

	/** @brief Short name of geoobject */
	AX_PROPERTY( QString, _shortName, shortName, setShortName )
	Q_PROPERTY( QString	shortName READ shortName WRITE setShortName )

	/** @brief Distance in meters between request coordinates and geoobject center */
	AX_PROPERTY( double, _distance, distance, setDistance )
	Q_PROPERTY( double	distance READ distance WRITE setDistance )

	/** @brief Latitude of the object center */
	AX_PROPERTY( double, _lat, lat, setLat )
	Q_PROPERTY( double lat READ lat WRITE setLat )

	/** @brief Longitude of the object center */
	AX_PROPERTY( double, _lon, lon, setLon )
	Q_PROPERTY( double lon READ lon WRITE setLon )

	/** @brief Additional attributes of objects (depends of map driver and type of object) */
	ObjectInfoAttributes _attributes;

public:
	ObjectInfo(QObject* parent = 0);
	ObjectInfo(const ObjectInfo& copy);
	ObjectInfo(const QString& id, const Pw::Gis::GeoSearch::ObjectInfoType::Enum type, const QString& name,
			   double lat, double lon, const QString& customType = "",
			   const QString& shortName = "", double distance = 0);

	bool operator ==( const ObjectInfo& other );
	bool operator !=( const ObjectInfo& other );
	ObjectInfo& operator =( const ObjectInfo& other );

public slots:
	void setAttribute( const QString& name, const QString& value );
	QString getAttribute( const QString& name ) const;
	bool hasAttribute( const QString& name ) const;
	QStringList getAttributeKeys() const;
};

} // namespace GeoSearch
} // namespace Gis
} // namespace Pw

#endif // GEOSEARCH_OBJECTINFO_H
