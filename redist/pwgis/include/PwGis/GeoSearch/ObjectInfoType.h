#ifndef GEOSEARCH_OBJECTINFOTYPE_H
#define GEOSEARCH_OBJECTINFOTYPE_H

#include <QString>
#include <QObject>

namespace Pw
{
namespace Gis
{
namespace GeoSearch
{

class ObjectInfoType : public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{026C9F8F-69BD-4538-8FFB-E0FF5141B9EC}")
	Q_CLASSINFO("InterfaceID", "{5FD4616B-A867-4CE0-A2B4-3FD3F1CC5498}")
	Q_CLASSINFO("CoClassAlias", "PwGisSearchObjectInfoType")

public:
	enum Enum {
		NotDefined,			// object type not defined in current map
		Custom,				// object type not defined in current enum
		Country,
		Region,
		District,
		House,
		City,
		Town,				//less than city
		Settlement,			// less than town
		Station,			// contains big station like railway terminal
		Airport,
		MetroStation,
		StationPlatform,	// contains station platforms
		Dock,
		BusStop,
		Parking,
		Street,
		LivingArea,			// microdistrict or housing estate
		Place,
		Sight,				// monument, fountain, etc.
		Stadium,
		Cemetry,
		Beach,
		Church,
		Park,
		Forest,
		Crossroad,
		Metro,
		Railway,
		Tramway,
		Highway,
		Embankment,
		Road,
		Bridge,
		Ocean,
		Sea,
		Lake,
		River,
		Channel,
		Island
	};

	Q_ENUMS(Enum)

	ObjectInfoType(QObject* parent = 0) : QObject(parent) {}

	static QString enumToString( ObjectInfoType::Enum val )
	{
		switch(val) {
			case NotDefined:		return "NotDefined";
			case Custom:			return "Custom";
			case Country:			return "Country";
			case Region:			return "Region";
			case District:			return "District";
			case House:				return "House";
			case City:				return "City";
			case Town:				return "Town";
			case Settlement:		return "Settlement";
			case Station:			return "Station";
			case Airport:			return "Airport";
			case MetroStation:		return "MetroStation";
			case StationPlatform:	return "StationPlatform";
			case Dock:				return "Dock";
			case BusStop:			return "BusStop";
			case Parking:			return "Parking";
			case Street:			return "Street";
			case LivingArea:		return "LivingArea";
			case Place:				return "Place";
			case Sight:				return "Sight";
			case Stadium:			return "Stadium";
			case Cemetry:			return "Cemetry";
			case Beach:				return "Beach";
			case Church:			return "Church";
			case Park:				return "Park";
			case Forest:			return "Forest";
			case Crossroad:			return "Crossroad";
			case Metro:				return "Metro";
			case Railway:			return "Railway";
			case Tramway:			return "Tramway";
			case Highway:			return "Highway";
			case Embankment:		return "Embankment";
			case Road:				return "Road";
			case Bridge:			return "Bridge";
			case Ocean:				return "Ocean";
			case Sea:				return "Sea";
			case Lake:				return "Lake";
			case River:				return "River";
			case Channel:			return "Channel";
			case Island:			return "Island";
		}
		return QString("undefined (%1)").arg(val);
	}
};

} // namespace GeoSearch
} // namespace Gis
} // namespace Pw

#endif // GEOSEARCH_OBJECTINFOTYPE_H
