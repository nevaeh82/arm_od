#ifndef GEOCOMMONSTRUCTS_H
#define GEOCOMMONSTRUCTS_H

#include <qlist.h>
#include "GeoMathematics_global.h"


const double GOOGLE_EARTH_RADIUS = 6378137.0;
//equator = 360degree or 2*3.14159*GOOGLE_EARTH_RADIUS = 40 075 016.686 meters
const double ERROR_COORDINATE = 100000000;
const double LATITUDE_MERCATOR_LIMIT = 85.0; //85.05112878
//1 degree = 111 319.491 meters
//0.000 000 000 01 degree = 0.000 001 meters = 1 micrometer
const double ERROR_MACHINE_COORDINATE = 0.00000000001;


struct GEOMATHEMATICS_EXPORT Geo2DPoint
{
	double Longitude;
	double Latitude;

	Geo2DPoint( double longitude = 0, double latitude = 0 );
};

struct GEOMATHEMATICS_EXPORT Geo3DPoint
{
	double Longitude;
	double Latitude;
	double Altitude;

	Geo3DPoint( double longitude = 0, double latitude = 0, double altitude = 0 );
};

struct GEOMATHEMATICS_EXPORT XYPoint
{
	double X;
	double Y;

	XYPoint( double x = 0, double y = 0 );
};

struct GEOMATHEMATICS_EXPORT XYZPoint
{
	double X;
	double Y;
	double Z;

	XYZPoint( double x = 0, double y = 0, double z = 0 );
};

struct GEOMATHEMATICS_EXPORT GeoBorder
{
	double MinLongitude;
	double MinLatitude;
	double MaxLongitude;
	double MaxLatitude;

	GeoBorder( double minLon = 0, double minLat = 0, double maxLon = 0, double maxLat = 0 );
};

struct GEOMATHEMATICS_EXPORT IndexesAndNodes
{
	QList<int> Indexes;
	QList<Geo2DPoint> Nodes;
};

//list of points defining the spot
typedef QList<Geo2DPoint> Geo2DList;

//list of spots
typedef QList<Geo2DList> SpotsList;

#endif // GEOCOMMONSTRUCTS_H
