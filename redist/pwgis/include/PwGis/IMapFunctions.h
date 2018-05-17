#ifndef IMAPFUNCTIONS_H
#define IMAPFUNCTIONS_H

class PwGisStyle;
class QVariant;
class QString;
class PwGisPointList;
class PwGisLonLat;
class QStringList;

class IMapFunctions{
public:
	/// Add Marker to markers layer.
	/// @param id             marker id
	/// @param lon            point longitude
	/// @param lat            point latitude
	/// @param caption        caption displayed in popup window
	/// @param tooltip        tooltip displayed in popup window, text can be presented as HTML
	/// @see html
	/// @param rotateAngle    rotate angle of icons
	/// @param aStyle         style names string splitted by " ". ex: "style1 style2"
	virtual void addMarker( const QString& id, double lon, double lat, const QString& caption, const QString& tooltip, double rotateAngle, const QString& aStyle ) = 0;

	/// Add vector point to vector layer.
	/// @param id         feature id.
	/// @param lon        point longitude
	/// @param lat        point latitude
	/// @param caption    caption displayed in popup window
	/// @param tooltip    tooltip displayed in popup window, text can be presented as HTML
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	virtual void addPoint( const QString& id, double lon, double lat, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add linestring feature to vector layer.
	/// @param id   feature id.
	/// @param lonStart   start point longitude
	/// @param latStart   start point latitude
	/// @param lonEnd     end point longitude
	/// @param latEnd     end point latitude
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	virtual void addLine( const QString& id, double lonStart, double latStart, double lonEnd, double latEnd, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add rectangle feature to vector layer.
	/// @param id         feature id.
	/// @param left       left side longitude
	/// @param bottom     bottom side latitude
	/// @param right      right side longitude
	/// @param top        top side latitude.
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle    style names string splitted by " ". ex: "style1 style2"
	virtual void addRectangle( const QString& id, double left, double bottom, double right, double top, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add circle feature to vector layer.
	/// @param id   feature id.
	/// @param originLon  circle origin longitute
	/// @param originLat  circle origin latitude
	/// @param radius     radius of circle in map units, such as map we have in UTM projection , then radius should be in metres.
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	virtual void addCircle( const QString& id, double originLon, double originLat, double radius, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add classicPolygon feature to vector layer.
	/// @param id         feature id.
	/// @param originLon  classicPolygon origin longitute
	/// @param originLat  classicPolygon origin latitude
	/// @param radius     radius of circle-classicPolygon in map units, such as map we have in UTM projection , then radius should be in metres.
	/// @param sides      number of sides of the polygon
	/// @param rotation   angle of rotation
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	virtual void addClassicPolygon( const QString& id, double originLon, double originLat, double radius, int sides, double rotation, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add path feature to vector layer.
	/// @param id        feature id.
	/// @param points    array of points.
	/// @param caption   caption displayed in popup window
	/// @param tooltip   object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle    style names string splitted by " ". ex: "style1 style2"
	virtual void addPath( const QString& id, PwGisPointList* points, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add polygon feature to vector layer.
	/// @param id         feature id.
	/// @param points     array of points.
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	virtual void addPolygon( const QString& id, PwGisPointList* points, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Remove points from path. Remember that type of geometry must be PwGisWidget::Path
	/// @param id feature id.
	/// @param from index from .
	/// @param to last index of point to delete
	virtual void pathRemovePoints(const QString& id, int from, int to ) = 0;

	/// Add points to path .
	/// @param id   feature id. Remember that type of geometry must be PwGisWidget::Path
	/// @param points  array of points.
	virtual void pathAddPoints(const QString& id, PwGisPointList* points ) = 0;

	/// Add pie slice on map
	/// @param id               feature id.
	/// @param originLon lon    coordinate of the pie center
	/// @param originLat lat    coordinate of the pie center
	/// @param radius           radius of the pie
	/// @param startAngle       starting angle of the slice
	/// @param endAngle         finishing angle of the slice
	/// @param caption          caption displayed in popup window
	/// @param tooltip          object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle           style names string splitted by " ". ex: "style1 style2"
	virtual void addSlice( const QString& id, double originLon, double originLat, double radius, double startAngle, double endAngle, const QString& caption, const QString& tooltip, const QString& aStyle ) = 0;

	/// Add ring on map
	/// @param id               feature id.
	/// @param originLon        lon coordinate of the ring center
	/// @param originLat        lat coordinate of the ring center
	/// @param minRadius        inner ring radius
	/// @param maxRadius        outer ring radius
	/// @param startAngle       starting angle of the ring
	/// @param endAngle         finishing angle of the ring
	/// @param caption          caption displayed in popup window
	/// @param tooltip          object tooltip. Text can be presented as HTML.
	/// @param aStyle           style names string splitted by " ". ex: "style1 style2"
	virtual void addRing(const QString& id, double originLon, double originLat, double minRadius, double maxRadius, double startAngle, double endAngle, const QString& caption, const QString& tooltip, const QString& aStyle) = 0;

	/// Moves any object created with add... methods to specified location on map
	/// @param id feature id
	/// @param lon point longitude
	/// @param lat point latitude
	virtual void moveFeature( const QString& id, double lon, double lat ) = 0;

	/// Search specified feture and retrieves its coordinates
	/// @returns feature coordinates
	virtual PwGisLonLat* getFeatureLonLat(const QString& id) = 0;

	/// Starts add/remove transaction. Enqueues your single add/remove requests and completes them only on commitTransaction().
	/// Use it on massive objects add/remove operations to enhance renderer speed.
	virtual void startTransaction() = 0;
	/// Commits started add/remove transaction. Pushes equeued add/remove requests downto OpenLayer.
	virtual void commitTransaction() = 0;


	/// remove vector object specified by id.
	/// @param id  id of item which need to remove.
	virtual void removeObject( const QString& id) = 0;

	/// Removes all object from all map layers
	virtual void removeAllObjects() = 0;

	/// Get geometry for specified feature id.
	/// @param id  feature id which was created by user.
	/// @return  points array.
	virtual PwGisPointList* getGeometry( const QString& id) = 0;

	/// Method retrieves fetaure props such as type (line, poly, circle, circle radius, rotation ...)
	/// More suitable type to return is Map, but it's not supported by qax (method will be inaccessible from .NET)
	/// implementation of custom container will solve the problem
	/// @param id feature id
	/// @returns pairs (name value) represented in string array
	virtual QStringList getFeatureProperties(QString id) = 0;

	virtual ~IMapFunctions(){}
};

#endif // IMAPFUNCTIONS_H
