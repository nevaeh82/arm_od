#ifndef MAPBOUNDS_H
#define MAPBOUNDS_H
#include <QString>
#include "IMapBounds.h"
#include "MapBoundsEvents.h"
class IMapProvider;
class GeoRectangle;

namespace Pw { namespace Gis {

class GISCLIENT_EXPORT MapBounds : public MapBoundsEvents, public IMapBounds
{
	Q_OBJECT
	Q_CLASSINFO("ClassID","{19938dcb-2a69-475a-ad22-6baf354c0583}")
	Q_CLASSINFO("InterfaceID","{d3030d6d-3fb9-48c3-871b-110738c41ae0}")
	Q_CLASSINFO("CoClassAlias", "MapBounds")

	IMapProvider* _gis;

private:
	static QString coordinate(double value);

public:
	MapBounds(QObject* parent);
	MapBounds(IMapProvider* gis, QObject* parent);

	/// Returns an event handler object
	/// @return QObject reference
	virtual MapBoundsEvents&  events() { return *this; }

public slots:
	/// Zoom map to bounds.
	/// @param left  left bound longitude.
	/// @param bottom   bottom bound  latitude.
	/// @param right  right bound longitude.
	/// @param top    top bound latitude.
	virtual void zoomMapTo(double left, double bottom, double right, double top);

	/// Zoom map to center width offset in meters.
	/// @param lon  center longitude.
	/// @param lat   center  latitude.
	/// @param offsetB  offset to bottom.
	/// @param offsetR  offset to left.
	virtual void zoomMapToByOffset(double lon, double lat, int offsetB, int offsetR = 0);

	/// Set map center.
	/// @param lon   map center longitude.
	/// @param lat   map center latitude.
	virtual void setMapCenter( double lon, double lat);

	/// Get map center.
	/// @return  map center coordinates
	virtual PwGisLonLat* getMapCenter();

	/// Get map bounds.
	/// @return current map bounds in geographical rectangle format
	virtual GeoRectangle* getMapBounds();

	/// Get view bounds.
	/// @return current view bounds in geographical rectangle format
	virtual GeoRectangle* getViewBounds();

	/// Get map zoom.
	/// @return current map zoom
	virtual int getCurrentZoom();

	/// Set map zoom.
	/// @param zoomLevel  map zoom
	virtual void setCurrentZoom( int zoomLevel );

	/// Get map zoom info.
	/// @return current map zoom info
	virtual MapZoomInfo* getMapZoomInfo();
};

} // namespace Gis
} // namespace Pw

#endif // MAPBOUNDS_H
