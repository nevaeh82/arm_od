#ifndef PWGISWIDGET_H
#define PWGISWIDGET_H

#include <QList>
#include <QVariant>
#include <QMutex>
#include <QTranslator>

#include <QWidget>

#include <QtNetwork/QNetworkRequest>

#include "PwGisClient.h"
#include "IMapProvider.h"
#include "IMapManager.h"
#include "MapBoundsEvents.h"
#include "MapObjectEvents.h"
#include "IMapFunctions.h"

class QWebView;
class QWebInspector;
class QNetworkReply;
class TestJsCodeGeneration;

namespace Pw{ namespace Gis { namespace Common { namespace ServerControl {
class ServerHandlerAbstract;
class ServerRepository;
class IProxyServerHandlerFactory;
}}}}

namespace Pw{ namespace Gis { namespace Common {
class ICache;
}}}

namespace Pw { namespace Logger{
class ILogger;
}}

namespace Pw { namespace Gis{
class MapManager;
class MapBounds;
class MapObjectEvents;
}}
namespace Pw { namespace Gis { namespace Client {
class GisControl;
class IGisControl;
}}}

class PwGisStyle;
class PwGisLonLat;
class PwGisPointList;
class MapFormatInfo;
class MapFormatsCollection;
class MapProvider;
class GeoRectangle;

using namespace Pw::Gis::Common::ServerControl;

class GISCLIENT_EXPORT  PwGisWidget : public QWidget, public IMapFunctions
{
	Q_OBJECT
	Q_ENUMS(MeasureState)
	Q_ENUMS(Tools)
	Q_ENUMS(FeatureType)
	Q_CLASSINFO("ClassID", "{651e85c0-d78b-11e0-9572-0800200c9a66}")
	Q_CLASSINFO("InterfaceID", "{725194d0-d78b-11e0-9572-0800200c9a66}")
	Q_CLASSINFO("EventsID","{a1d47400-d78b-11e0-9572-0800200c9a66}")
	//Q_CLASSINFO("RegisterObject", "yes")

	friend class Pw::Gis::Client::GisControl;
	friend class ::TestJsCodeGeneration;
public:

	Q_PROPERTY(double lastMeasure READ getLastMeasure())
	//Q_PROPERTY(MapProvider* mapProvider READ mapProviderClass)
	Q_PROPERTY(QObject* mapEvents READ getMapEvents())
	Q_PROPERTY(QObject* mapBoundsEvents READ getMapBoundsEvents())
	Q_PROPERTY(QObject* mapObjectEvents READ getMapObjectEvents() )

	/// This enum of tools
	enum Tools{
		ToolDrawCircle = 1,
		ToolDrawRectangle,
		ToolNavigation,
		ToolRuler,
		ToolCurvometr,
		ToolDrawPoint,
		ToolDrawPath,
		ToolChangeGeometry
	};

	/// This enum of measure stat
	enum MeasureState{
		Pending = 1,
		Finished,
		CurvometrPending,
		CurvometrFinished
	};

	enum Progress {
		/// Initialization map opening
		Initialization = 0,

		/// Server started
		ServerStarted,

		/// Map loaded.
		MapLoaded,

		/// No map loaded
		NoMapLoaded,

		///
		Error
	};

public:

	/// Initializes a new instance of PwGisWidget.
	PwGisWidget(QWidget *parent = 0);

	/// Destructor.
	~PwGisWidget();

private: //data
	QWebView* _webView;
	//    Ui::PwGisWidget *ui;

	int _styleId;

	///Last measure
	double m_lastMeasure;

	/// coordinates convertation presicion.
	const int c_coordinatePrecision;

	QWebInspector *_inspector;
	Pw::Logger::ILogger* _logger;
	MapProvider* _mapProvider;
	Pw::Gis::Common::ICache* _cache;
	bool _isInitialized;
	bool _isDraggableMap;
	bool _isDebuggerEnabled;

	Pw::Gis::Client::IGisControl* _gisControl;
	QTranslator* _translator;
	QTranslator* _commonTranslator;

	QMutex _syncRoot;

	QString _javaScriptLocale;
	bool _isVisibleBaseLayerInSwitcher;

private: //methods

	/// convert coordinate to string.
	/// @param value  value which should be converted to string.
	QString coordinate( double value );

	void onPointAdded(const QString &id);

	QRadialGradient parseGradient(QVariant var);

	void initialize();

	void lazyInitialize();

	void replaceGisControl(Pw::Gis::Client::IGisControl *control);

public slots:
	IMapProvider*  mapProvider();

	MapProvider* mapProviderClass();

	QObject* getMapEvents();

	QObject* getMapBoundsEvents();

	Pw::Gis::MapObjectEvents* getMapObjectEvents();

	Pw::Gis::Client::IGisControl* webControl();

public slots:

	/// JS callback on user click on map
	void onMapClick(double lon, double lat);

	/// JS callback on measure bu ruler or curvometr
	void onMeasure(const QVariantList &params, double aMeasure, int aState);

	/// JS callback on user moves cursor
	void onMapMouseMove(double lon, double lat);

public slots:

	PwGisStyle* createStyle(const QString& styleName);

	/// EXecute custom script on JavaScript side.
	/// @param script  script which need to execute.
	QVariant executeScript( const QString& script );

	/// Remove marker from markers layer

	/// @param id   marker ID which should be removed.
	void removeMarker( const QString& id );

	/// Add Marker to markers layer.

	/// @param id             marker id
	/// @param lon            point longitude
	/// @param lat            point latitude
	/// @param caption        caption displayed in popup window
	/// @param tooltip        tooltip displayed in popup window, text can be presented as HTML
	/// @see html
	/// @param rotateAngle    rotate angle of icons
	/// @param aStyle         style names string splitted by " ". ex: "style1 style2"
	void addMarker( const QString& id,
					double lon, double lat,
					const QString& caption, const QString& tooltip,
					double rotateAngle, const QString& aStyle  );

	/// Add vector point to vector layer.
	/// @param id         feature id.
	/// @param lon        point longitude
	/// @param lat        point latitude
	/// @param caption    caption displayed in popup window
	/// @param tooltip    tooltip displayed in popup window, text can be presented as HTML
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	void addPoint( const QString& id, double lon, double lat,
				   const QString& caption , const QString& tooltip , const QString& aStyle  );

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
	void addLine( const QString& id, double lonStart, double latStart, double lonEnd, double latEnd,
				  const QString& caption , const QString& tooltip , const QString& aStyle );

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
	void addRectangle( const QString& id, double left, double bottom, double right, double top,
					   const QString& caption, const QString& tooltip, const QString& aStyle );

	/// Add circle feature to vector layer.
	/// @param id   feature id.
	/// @param originLon  circle origin longitute
	/// @param originLat  circle origin latitude
	/// @param radius     radius of circle in map units, such as map we have in UTM projection , then radius should be in metres.
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	void addCircle( const QString& id, double originLon, double originLat, double radius,
					const QString& caption, const QString& tooltip, const QString& aStyle );

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
	void addClassicPolygon( const QString& id, double originLon, double originLat,
							double radius, int sides, double rotation,
							const QString& caption, const QString& tooltip, const QString& aStyle);

	/// Add path feature to vector layer.
	/// @param id        feature id.
	/// @param points    array of points.
	/// @param caption   caption displayed in popup window
	/// @param tooltip   object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle    style names string splitted by " ". ex: "style1 style2"
	void addPath( const QString& id, PwGisPointList* points,
				  const QString& caption, const QString& tooltip, const QString& aStyle );

	/// Add polygon feature to vector layer.
	/// @param id         feature id.
	/// @param points     array of points.
	/// @param caption    caption displayed in popup window
	/// @param tooltip    object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle     style names string splitted by " ". ex: "style1 style2"
	void addPolygon( const QString& id, PwGisPointList* points,
					 const QString& caption, const QString& tooltip, const QString& aStyle );

	/// Remove points from path. Remember that type of geometry must be PwGisWidget::Path
	/// @param id feature id.
	/// @param from index from .
	/// @param to last index of point to delete
	void pathRemovePoints(const QString& id, int from, int to);

	/// Add points to path .
	/// @param id   feature id. Remember that type of geometry must be PwGisWidget::Path
	/// @param points  array of points.
	void pathAddPoints(const QString& id, PwGisPointList* points);

	/// Add pie slice on map.
	///  This method uses trigonometric angels:
	///  * angle changes counterclockwise
	///  * 0 degrees - direction to the East
	///  * 90 degrees - direction to the North
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
	void addSlice( const QString& id, double originLon, double originLat,
				   double radius, double startAngle, double endAngle,
				   const QString& caption, const QString& tooltip, const QString& aStyle );

	/// @brief Add ring or ring segment to map
	/// @param id   unique object id.
	/// @param originLon  Longitude of the ring center (degree)
	/// @param originLat  Latitude of the ring center (degree)
	/// @param minRadius  small radius (metres).
	/// @param maxRadius  large radius (metres).
	/// @param startAngle starting angle for ring segment (degrees). Must be 0 for full ring.
	/// @param endAngle   finishing angle for ring segment (desgrees). Mus be 360 for full ring.
	/// @param caption          caption displayed in popup window
	/// @param tooltip          object tooltip. Text can be presented as HTML.
	/// @see html
	/// @param aStyle           style names string splitted by " ". ex: "style1 style2"
	void addRing(const QString& id, double originLon, double originLat,
				 double minRadius, double maxRadius, double startAngle, double endAngle,
				 const QString& caption, const QString& tooltip, const QString& aStyle );

	/// Moves any object created with add... methods to specified location on map
	/// @param id feature id
	/// @param lon point longitude
	/// @param lat point latitude
	void moveFeature(const QString& id, double lon, double lat);

	/// Search specified feture and retrieves its coordinates
	/// @returns feature coordinates
	PwGisLonLat* getFeatureLonLat(const QString &id);

	/// Starts add/remove transaction. Enqueues your single add/remove requests and completes them only on commitTransaction().
	/// Use it on massive objects add/remove operations to enhance renderer speed.
	void startTransaction();
	/// Commits started add/remove transaction. Pushes equeued add/remove requests downto OpenLayer.
	void commitTransaction();


	/// remove vector object specified by id.
	/// @param id  id of item which need to remove.
	void removeObject( const QString& id);

	/// Remove specified objetc from map.
	/// @param obj target object.
	void removeObject( const IMapObject* obj);

	/// Removes all object from all map layers
	void removeAllObjects();

	/// add two dragControls to layers: `vectors`, `layerMarkers`
	void addDragControls();

	/// Get geometry for specified feature id.
	/// @param id  feature id which was created by user.
	/// @return  points array.
	PwGisPointList* getGeometry( const QString& id);

public slots: 	 //services
	void enableDebugger(bool isDebuggerEnabled);

public slots: //tool action methods.

	/// Activate map tool.

	/// @param tool id of tool enumeration PwGisWidget::Tools
	void activateTool(Tools tool);

	/// Show map tool.

	/// @param tool id of tool enumeration PwGisWidget::Tools
	void showTool(Tools tool, bool visible);

	/// Method retrieves fetaure props such as type (line, poly, circle, circle radius, rotation ...)
	/// More suitable type to return is Map, but it's not supported by qax (method will be inaccessible from .NET)
	/// implementation of custom container will solve the problem
	/// @param id feature id
	/// @returns pairs (name value) represented in string array
	QStringList getFeatureProperties(QString id);

	/** Set visible base layer switcher
	  @param visible - true\false - visible\unvisible
	 */
	void showBaseLayerSwitcher( bool visible );

public:

	double getLastMeasure();

	/** get PwGisWidget version from VERSION
	  */
	QString version();

	/** set localization
	  */
	void setLocale( const QLocale& locale );

signals:
	/// Signal caused when user clickes on the map
	/// @param lon x-axis coordinate of the mouse
	/// @param lat y-axis coordinate of the mouse
	void mapClicked(double lon, double lat);

	/// Signal caused when user moves cursor over the map
	/// @param lon x-axis coordinate of the mouse
	/// @param lat y-axis coordinate of the mouse
	void mapMouseMove(double lon, double lat);

	/// @brief  Signal emitted when user is using ruler for measure
	///
	/// @param points  ruler geometry.
	/// @param measure in meters
	/// @param aState "pending" ( 1 ) or "finished" ( 2 ) or Curvometr pedding (3) or Curvometr finished (4)
	void pathMeasured(const QVariantList &params, double aMeasure, int aState);

	/// Signal emitted when load page finished in webview
	void exLoadWebViewFinished( bool, const QUrl& );

protected slots:
	//void featureAdded( const QString& type, const QString& id );
	/// Internal slot for browser.
	void testSlot(QNetworkReply *reply);

private slots:
	///for signal exLoadWebViewFinished
	void loadWebViewFinished( bool ok );

	///map successfully loaded to widget
	void onMapReady();
};

#ifdef _MSC_VER
HRESULT GISCLIENT_EXPORT dumpIDL1(const QString& p1, const QString& p2);
#endif


#endif // PWGISWIDGET_H
