#ifndef GEOMETRICFUNCTIONS_H
#define GEOMETRICFUNCTIONS_H

#include <QtCore/qmath.h>
#include "GeoCommonStructs.h"
#include "GeoMathematics_global.h"

/** Geometric functions for PwGis.
 */
class GEOMATHEMATICS_EXPORT GeometricFunctions
{
public:
	GeometricFunctions();

	double deg2rad( double src );
	double rad2deg( double src );

	/** Convert periodic longitude to range [-180..+180]
	  @param src - periodic longitude ( 90, 360, 720..) in degree
	  @return longitude in [-180..+180] degree
	 */
	double fixLongitude( double src );

	/** Validation of latitude [-90..+90]
	 @param src - latitude in degree
	 @return true if latitude in [-90..+90]
	 */
	bool isLatitudeCorrect( double src );

	/** Is equal mumbers, considering the machine error
	@param number1,number2 - numbers are compared for equality
	@return true if number1 == number2
	*/
	bool isEqualNumbers( double number1, double number2 );

	/** Longitude is 180 degrees, considering  the machine error
	@param src - longitude in degree [-180..+180]
	@return true if src == -180, 180
	*/
	bool is180Longitude( double src );

	/** Longitude is 0 degrees, considering  the machine error
	@param src - longitude in degree [-360..+360]
	@return true if src == 0, 360
	*/
	bool is0Longitude( double src );

	/** Convert longitude from range [-180..+180] to range [0..+360]
	 @param src - longitude in [-180..+180] degree
	 @return longitude in [0..+360] degree
	 */
	double lon180To360( double src );

	/** Shift longitude of points from range [-180..+180] to range [0..+360]
	 @param nodes - points with longitude in [-180..+180] degree
	 @return points with longitude in [0..+360] degree
	 */
	Geo2DList shiftPointsLon180To360( Geo2DList nodes );

	/** Convert longitude from range [0..+360] to range [-180..+180]
	 @param src - longitude in [0..+360] degree
	 @return longitude in [-180..+180] degree
	 */
	double lon360To180( double src );

	/** Shift longitude of points from range [0..+360] to range [-180..+180]
	 @param nodes - points with longitude in [0..+360] degree
	 @return points with longitude in [-180..+180] degree
	 */
	Geo2DList shiftPointsLon360To180( Geo2DList nodes );

	/** Polyline nodes is ascending by longitude?
	 *  It is assumed that the nodes are arranged without changing direction
	 *  Maximum longitude range of 180 degrees, spot may not exceed the equator
	 *  Longitude = 0,180 - transition point, delete them in the analysis
	  @param nodes - points of polyline, longitude in [-180..+180]
	  @return true if ascending, false if descending
	 */
	bool isAscendLongitudeInPolyline( Geo2DList nodes );

	/** Determine what is the hemisphere in longitude spot
	 *  Maximum longitude range of 180 degrees, spot may not exceed the equator
	 *  Longitude = 0,180 - transition point, delete them in the analysis
	 @param nodes - points of spot, longitude in [-180..+180]
	 @return true if longitudes nodes of spot > 0.0
	 */
	bool isPlusSignLongitude( Geo2DList nodes );

	//TODO
	//OpenLayers limit
	/** Replace point with longitude equal +180 to +179.9999, -180 to plus -179.9999
	 @param nodes - points of spot, longitude in [-180..+180]
	 @return nodes of spot nodes lon=180, replaces lon=179.9999
	 */
	Geo2DList replace180179( Geo2DList nodes );

	/** Getting the point of intersection of two segments.
		The ends of the segments are excluded.
	 @param lonStart1, latStart1, lonEnd1, latEnd1 - coordinates of first segment
	 @param lonStart2, double latStart2, double lonEnd2, double latEnd2 - coordinates of second segment
	 @return point of intersection
	*/
	Geo2DPoint getCrossPoint( double lonStart1, double latStart1, double lonEnd1, double latEnd1,
		double lonStart2, double latStart2, double lonEnd2, double latEnd2 );

	/** Get geo frame of list of geo points
	 @param nodes - list of geo points, longitude in [-180..+180]
	 @return min max coordinates of geo frame
	 */
	GeoBorder getGeoBorder( Geo2DList nodes );

	/** Calculation of the centroid of the object (the center of gravity,
		all nodes have the same weight).
		Center - the arithmetic mean for each coordinate.
		@param nodes - list of geo points, longitude in [-180..+180]
		@return centroid
	*/
	Geo2DPoint getCentroidOfObject( Geo2DList nodes );

	/** Get the number of intersections with the meridian 180 and convex polygon
	 *  Maximum range of 180 degrees, spot may not exceed the equator,
	 *  discard the possibility of crossing points 180 to 10 350
	 @param nodes - spot-polygon on earth surface, longitude in [-180..+180]
	 @return count point of intersections
	 */
	int getCountSplitPointSpotMeridian180( Geo2DList nodes );

	/** Adding nodes to a spot-polyline,
	 *  which are obtained by the intersection of the meridian 180
	 *  Maximum longitude range of 180 degrees, spot may not exceed the equator,
	 *  discard the possibility of crossing points 180 to 10 350
	 @param nodes - spot nodes, longitude in [-180..+180]
	 @return polygon with lon=180 nodes and indexes of these nodes
	 */
	IndexesAndNodes insertPoint180( Geo2DList nodes );

	/** Split ascending spot by one point with longitude = 180
	 *  Sort polyline nodes in longitude range from -180 to +180
	 @param indexesNodes - ascending spot nodes,
			index node of lon = 180, which should split the spot
	 @return spot with ascending longitude [-180..+180]
	 */
	Geo2DList convertSpotBy1Point180ToM180P180( IndexesAndNodes indexesNodes );

	/** Split convex polygon by meridian 180 into two parts
	 @param indexesNodes - spots nodes, indexes of nodes of lon = 180
	 @return list of split spots
	 */
	SpotsList splitConvexPolygonBy2Points180( IndexesAndNodes indexesNodes );

	/** Split convex polygon by horizontal or vertical line into two parts
	 *  Longitude nodes must be specified without
	 *  synchronous transition +180 -> -180 and +360 -> 0
	 @param nodes - spots nodes, longitude in [0..+360] or [-180..+180]
	 @param lonStart, latStart, lonEnd, latEnd - coordinates of split segment
	 @return list of split spots
	 */
	SpotsList splitConvexPolygonByHVLine( Geo2DList nodes,
		double lonStart, double latStart, double lonEnd, double latEnd );

	/**Crop convex polygon parallels +85 degree and -85 degree
	 @param nodes - spots nodes, longitude in [-180..+180]
	 @return list of nodes of crop spots
	 */
	Geo2DList cropConvexPolygonParallels85( Geo2DList nodes );

	/** Ascending order nodes in the range of [-180..+180]
	 *  One point of intersection with the meridian 180
	 @param nodes - points of spot, longitude in [-180..+180]
	 @return ascending order nodes of spot
	 */
	Geo2DList ascendSpot( Geo2DList nodes );

	/**Crop spot parallels +85 degree and -85 degree and
	 * get visible polygons on the map in the Mercator projection
	 * Spot is not a convex polygon
	 * Spot can cross the parallel 85 not once
	 * Crossing one segment 85 and -85 is not considered
	 * Step between nodes when picked spots should be small
	 @param nodes - spot nodes, longitude in [-180..+180]
	 @param northHemisphere - viewpoint ( satellite ), true if lat > 0
	 @return list of spots ( spot - list of nodes polygon )
	         visible in the Mercator projection
	 */
	SpotsList cropSpotParallels85( Geo2DList nodes, bool northHemisphere );

	/** Get earth spot on the Mercator map
	 @param earthSpot - nodes on Earth, longitude in [-180..+180]
	 @param northHemisphere - viewpoint ( satellite ), true if lat > 0
		    only makes sense if one point of intersection of the meridian 180 - one point
	 @return list of spots ( spot - list of nodes polygon ) on the map
	 */
	SpotsList getEarthMercatorSpots( Geo2DList earthSpot, bool northHemisphere = true );

private:
	double _pi;
};


#endif // GEOMETRICFUNCTIONS_H
