#ifndef GEOCOMMONSTRUCTS_H
#define GEOCOMMONSTRUCTS_H

#include <QVector>


struct Geo2DPoint
{
	double Latitude;
	double Longitude;

	Geo2DPoint( double latitude = 0, double longitude = 0 );
};

struct XYPoint
{
	double X;
	double Y;

	XYPoint( double x = 0, double y = 0 );
};

typedef QVector<Geo2DPoint> Geo2DVector;
typedef QVector<XYPoint> XYVector;

#endif // GEOCOMMONSTRUCTS_H
