#ifndef GEOMATH_H
#define GEOMATH_H
#include <PwGisLonLat.h>
#include "TacticMap_global.h"

class TACTICMAP_EXPORT GeoMath
{
public:
	GeoMath();


	/** Gets distance between to points.
	 * @brief GeoMath::getDistance
	 * @param A PwGisLonLat  point in degrees.
	 * @param B PwGisLonLat  point in degrees.
	 * @return  distanse in metres.
	 */
	static double getDistance(const PwGisLonLat &A, const PwGisLonLat &B);
};

#endif // GEOMATH_H
