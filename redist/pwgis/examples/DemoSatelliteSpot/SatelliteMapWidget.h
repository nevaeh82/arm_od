#ifndef SATELLITEMAPWIDGET_H
#define SATELLITEMAPWIDGET_H

#include <QWidget>
#include <QMap>
#include <PwGis/objects/Marker.h>
#include <PwGis/GeoMathematics/GeoCommonStructs.h>


namespace Ui {
class SatelliteMapWidget;
}
class PwGisLonLat;
class MapObjectStyle;
class PwGisPointList;

namespace Pw {
namespace Gis {
class Marker;
}
}

using namespace Pw::Gis;

typedef QString SatelliteMapHandle;

class CoverageMapType
{
public:
	enum Enum {
		LowOrbital, //< 10km
		MediumOrbital,
		HighOrbital,
		GeoStationary,
		SatelliteDefined
	};
};

struct SatellitePoint
{
	long time;
	qreal bearing;
	qreal placeAngle;
	qreal height;
	QPointF position;
};

class SatelliteMapWidget: public QWidget
{
	Q_OBJECT

public:
	explicit SatelliteMapWidget( QWidget *parent = 0 );
	~SatelliteMapWidget();

	PwGisPointList* getPointList( QPolygonF geometry );

	void drawPolygonOver180( QPolygonF poly360, QPolygonF empty, QString id, CoverageMapType::Enum satelliteType );

	QPolygonF crop82( QPolygonF poly );

	void normalize360( QPolygonF poly, QPolygonF &poly360, QPointF &min, QPointF &max );

	void addPointsPath( Geo2DList geoPoints );
	void addPolylinePath( Geo2DList geoPoints );
	void addPolygonPath( Geo2DList geoPoints );

	void removeAllObjects();

public slots:

	void openDialogMap();

	SatelliteMapHandle showCoverageMapCalc( CoverageMapType::Enum satelliteType, QPointF position,
		double altitude );

	/**
	 * @brief Add satellite to map.
	 * @param sateliteName  satellite name for displaying.
	 * @param position   satellite position.
	 * @param futureTrack  geometry (track) where satellite  will be in future.
	 * @param pastTrack geometry (track) where satellite  was in past.
	 * @return
	 */
	SatelliteMapHandle addSatellite( QString name, QString time, QPointF position,
		QPolygonF futureTrack, QPolygonF pastTrack );

	/**
	 * @brief Remove  satellite from map.
	 * @param satelliteHandle
	 */
	void removeSatellite( SatelliteMapHandle handle );

	/**
	 * @brief Update satellite position and time stamp
	 * @param handle
	 * @param time
	 * @param position
	 */
	void updateSatellitePosition( SatelliteMapHandle handle, QString time, QPointF position );

	/**
	 * @brief update satellite past and future track
	 * @param handle
	 * @param futureTrack
	 * @param pastTrack
	 */
	void updateSatelliteTrack( SatelliteMapHandle handle, QPolygonF futureTrack, QPolygonF pastTrack );


signals:
	/**
	 * @brief Signal emitted when map was fully loaded.
	 */
	void mapReady();

private:
	Ui::SatelliteMapWidget *ui;

	QMap< CoverageMapType::Enum, QString > _styleMap;
	int _nextId;
	bool _selectionModeEnabled;

	QString generateId();

	void defusePolygon( QPolygonF& polygon );
	void addPolygon( const QString& id, QPolygonF* outerGeometry, QPolygonF* innerGeometry,
		const QString& caption, const QString& tooltip, const QString& aStyle);

	QMap< SatelliteMapHandle, Marker* >_satellites;

	MapObjectStyle* _satelliteStyle;
	MapObjectStyle* _greenpointStyle;

	static bool lonMin(const QPointF &s1, const QPointF &s2)
	{
		return s1.x() < s2.x();
	}

	static bool latMin(const QPointF &s1, const QPointF &s2)
	{
		return s1.y() < s2.y();
	}

private slots:
	void onMapReady();
};

#endif // SATELLITEMAPWIDGET_H
