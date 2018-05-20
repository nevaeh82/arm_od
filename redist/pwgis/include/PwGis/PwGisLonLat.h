#ifndef PWGISLONLAT_H
#define PWGISLONLAT_H
#include <QObject>
#include "PwGisClient.h"


/** this structure describes geothesic coordinates of point.
  */
class GISCLIENT_EXPORT PwGisLonLat : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{c9c0f2a6-4de1-4e33-8b30-776aa61b2037}")
	Q_CLASSINFO("InterfaceID", "{0a50715e-764a-48da-99cf-ad8450af44bf}")

public:
	Q_PROPERTY(double lon READ getLon WRITE setLon)
	Q_PROPERTY(double lat READ getLat WRITE setLat)

	/// Point longitude.
	double lon;

	/// Point latitude.
	double lat;

	/// Copy construct
	PwGisLonLat(const PwGisLonLat& ll) : QObject(ll.parent()), lon(ll.lon), lat(ll.lat){}

	/// Construct class
	PwGisLonLat(QObject* parent = 0) : QObject(parent), lon(0), lat(0){}

	/// Construct structure with specified latitude and longitude,
	/// @param lon point longitude.
	/// @param lat point latitude.
	PwGisLonLat( double lon, double lat, QObject* parent=0): QObject(parent), lon(lon), lat(lat){}

	PwGisLonLat& operator=(const PwGisLonLat& ll);
	//bool operator==(const PwGisLonLat ll) const { return lon == ll.lon && lat == ll.lat; }
	double getLon() const { return lon; }
	double getLat() const { return lat; }

	void setLon(double arg) { lon = arg; }
	void setLat(double arg) { lat = arg; }

public slots:
};

#endif // PWGISLONLAT_H
