#include "SatelliteSpot.h"

#include <cmath>
#include <stdexcept>


SatelliteSpot::SatelliteSpot( const Geo3DPoint& satellitePosition )
throw( std::exception )
{
	setObserver( satellitePosition );
}

void SatelliteSpot::setObserver( const Geo3DPoint& observer )
throw( std::exception )
{
	if ( observer.Altitude <= 0 ) {
		throw std::runtime_error( "invalid satelite altitude" );
	}

	if ( !_geometricFunc.isLatitudeCorrect( observer.Latitude ) ) {
		throw std::runtime_error( "invalid observer latitude" );
	}

	double const halfVisibleAngle = getHalfVisibleAngle( observer.Altitude );

	double fixLon = _geometricFunc.fixLongitude( observer.Longitude );
	if ( observer.Latitude > 0.0 ) {
		_northHemisphere = true;
	}
	else {
		_northHemisphere = false;
	}
	Geo2DPoint const geo( fixLon, observer.Latitude );
	double const latitude = _geometricFunc.deg2rad( geo.Latitude );
	double const longitude = _geometricFunc.deg2rad( geo.Longitude );

	double const sinLatitude = sin( latitude );
	double const cosLatitude = cos( latitude );
	double const sinLongitude = sin( longitude );
	double const cosLongitude = cos( longitude );

	double const zeroVectorLatitude = latitude + halfVisibleAngle;
	double const sinZero = sin( zeroVectorLatitude );
	double const cosZero = cos( zeroVectorLatitude );

	//static double const EARTH_APROX_RADIUS = 6371;
	//const double GOOGLE_EARTH_RADIUS = 6378137.0;
	double eRadius = GOOGLE_EARTH_RADIUS / 1000.0;
	axis = SpaceModel::toVector( eRadius, sinLatitude, cosLatitude, sinLongitude, cosLongitude );
	zero = SpaceModel::toVector( eRadius, sinZero, cosZero, sinLongitude, cosLongitude );
}

Geo2DPoint SatelliteSpot::getPoint( double azimuth )
{
	double fixAzimuth = _geometricFunc.fixLongitude( azimuth );
	double angleAzimuth = _geometricFunc.deg2rad( fixAzimuth );
	SpaceModel::Vector const vector = SpaceModel::rotate( zero, axis, angleAzimuth );

	double const lat = SpaceModel::getLatitude( vector );
	double const lon = SpaceModel::getLongitude( vector );

	return Geo2DPoint( _geometricFunc.rad2deg( lon ), _geometricFunc.rad2deg( lat ) );
}

double SatelliteSpot::getHalfVisibleAngle( double altitude )
{
	//static double const EARTH_APROX_RADIUS = 6371;
	//const double GOOGLE_EARTH_RADIUS = 6378137.0;
	double eRadius = GOOGLE_EARTH_RADIUS / 1000.0;
	double const distance = sqrt( altitude*altitude + 2*altitude*eRadius);
	return asin( distance / ( altitude + eRadius ) );
}

Geo2DList SatelliteSpot::getEarthSpotNodes( double nodesStep )
{
	Geo2DList retVal;
	if ( ( nodesStep <= 0.0 ) || ( nodesStep > 180.0 ) ) {
		return retVal;
	}

	for( double azimuth = 0.0; azimuth < 360.0; azimuth += nodesStep ) {
		Geo2DPoint spotsNode = this->getPoint( azimuth );
		if ( _geometricFunc.isLatitudeCorrect( spotsNode.Latitude ) ) {
			retVal.append( spotsNode );
		}
	}

	return retVal;
}

SpotsList SatelliteSpot::getEarthSpots( double nodesStep )
{
	SpotsList retVal;
	Geo2DList earthSpot = this->getEarthSpotNodes( nodesStep );
	retVal = _geometricFunc.getEarthMercatorSpots( earthSpot, _northHemisphere );
	return retVal;
}