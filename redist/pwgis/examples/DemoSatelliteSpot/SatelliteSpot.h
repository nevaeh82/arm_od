#ifndef SATELLITESPOT_H
#define SATELLITESPOT_H

#include "SpaceModel.h"
#include <PwGis/GeoMathematics/GeometricFunctions.h>

#include <exception>


class SatelliteSpot {

public:
	/** constructor
	 @param satellitePosition
				satelliteLongitude in degree
				satelliteLatitude in degree
				satelliteAltitude in kilometers
	*/
	explicit SatelliteSpot( const Geo3DPoint& satellitePosition )
	throw(std::exception);

	// set observer position
	// input: GeoPosition3D structure
	void setObserver( const Geo3DPoint& observer )
	throw(std::exception);

	// returns point of spot circle
	// input: azimuth, degrees
	Geo2DPoint getPoint( double azimuth );

	/** Calculate spot on earth-sphere surface
	 @param nodesStep - degree step between nodes in polygon
	 @return list of nodes polygon
	 */
	Geo2DList getEarthSpotNodes( double nodesStep = 5.0 );

	/** Get spot from satellite on the Earth-sphere surface
	 @param nodesStep - degree step between nodes in the resulting list of nodes polygon
	 @return list of spots ( spot - list of nodes polygon )
	 */
	SpotsList getEarthSpots( double nodesStep = 5.0 );


private:
	SpaceModel::Vector axis, zero;
	GeometricFunctions _geometricFunc;
	bool _northHemisphere;

	double getHalfVisibleAngle( double altitude );
};

#endif // SATELLITESPOT_H
