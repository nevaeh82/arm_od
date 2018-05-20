#ifndef IMAPBOUNDS_H
#define IMAPBOUNDS_H

#include <QString>
#include <QObject>

class GeoRectangle;
class MapZoomInfo;
class PwGisLonLat;

namespace Pw { namespace Gis {

class MapBoundsEvents;

class IMapBounds
{
public:
	/// Virtual destructor
	virtual ~IMapBounds(){}

	/// Returns an event handler object
	/// @return QObject reference
	virtual MapBoundsEvents& events() = 0;

	/// Zoom map to bounds.
	/// @param left  left bound longitude.
	/// @param bottom   bottom bound  latitude.
	/// @param right  right bound longitude.
	/// @param top    top bound latitude.
	virtual void zoomMapTo(double left, double bottom, double right, double top) = 0;

	/// Zoom map to center width offset in meters.
	/// @param lon  center longitude.
	/// @param lat   center  latitude.
	/// @param offsetB  offset to bottom.
	/// @param offsetR  offset to left.
	virtual void zoomMapToByOffset(double lon, double lat, int offsetB, int offsetR = 0) = 0;

	/// Set map center.
	/// @param lon   map center longitude.
	/// @param lat   map center latitude.
	virtual void setMapCenter( double lon, double lat) = 0;

	/// Get map center.
	/// @return  map center coordinates
	virtual PwGisLonLat* getMapCenter() = 0;

	/// Get map bounds.
	/// @return current map bounds in geographical rectangle format
	virtual GeoRectangle* getMapBounds() = 0;

	/// Get view bounds.
	/// @return current view bounds in geographical rectangle format
	virtual GeoRectangle* getViewBounds() = 0;

	/// Get map zoom.
	/// @return current map zoom
	virtual int getCurrentZoom() = 0;

	/// Set map zoom.
	/// @param zoomLevel  map zoom
	virtual void setCurrentZoom( int zoomLevel ) = 0;

	/// Get map zoom info.
	/// @return current map zoom info
	virtual MapZoomInfo* getMapZoomInfo() = 0;
};

} // namespace Gis
} // namespace Pw

#endif // IMAPBOUNDS_H
