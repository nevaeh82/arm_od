#ifndef TRACEDTACTICMAPOBJECT_H
#define TRACEDTACTICMAPOBJECT_H

#include <QObject>
#include <QDateTime>
#include <QMutex>

#include <objects/MapObjectAbstract.h>
#include <objects/Marker.h>
#include <objects/Path.h>

#include "TacticObjectAbstract.h"
#include "styles/TracedObjectStyle.h"
#include "TacticMap_global.h"

class PwGisStyle;
class MapObjectStyle;
class IObjectsFactory;

/**
 * @brief The TracedObjectAbstract class provides movement history services.
 */
class TACTICMAP_EXPORT TracedObjectAbstract: public TacticObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{f38560dc-cc70-41af-93e1-7cc5626e8422}")
	Q_CLASSINFO("InterfaceID", "{7dd8b180-a8a1-4d72-b2b6-5218ea8f1b09}")

	Q_PROPERTY(const PwGisPointList* history READ history)
	Q_PROPERTY(TracedObjectStyle* pathStyle READ pathStyle WRITE setPathStyle)
	Q_PROPERTY(double rotate READ rotate WRITE setCourse)

	friend class TestRemoveObject;
protected:
	/**
	 * @brief The PointData class contains trace point metadata.
	 */
	class PointData
	{
	public:
		QDateTime _timestamp;
		double    _offset;
		PointData(QDateTime timestamp, double offset)
		{
			_timestamp = timestamp;
			_offset = offset;
		}

		const QDateTime& timestamp()
		{
			return _timestamp;
		}

		double offset()
		{
			return _offset;
		}
	};


	PwGisPointList*    _history;
	TracedObjectStyle* _style;

	Pw::Gis::Marker* _marker;
	Pw::Gis::Path*   _tail;

	QMutex _mutex;

	QList<PointData>* _pointData;

	/// Latest point timestamp.
	QDateTime _traceLastTimestamp;

	/// Current trace length.
	double    _traceLength;

	void removeFirstPoint();
public:
	explicit TracedObjectAbstract(IObjectsFactory* factory, QString id, QString name = "", QObject *parent = 0);
	explicit TracedObjectAbstract(QObject *parent=0);
	virtual ~TracedObjectAbstract();

	/**
	 * @brief Set rotation angle.
	 * @param angle  Rotation angle value.
	 */
	void setCourse(double angle);

	/**
	 * @brief Gets rotation angle value.
	 * @return Rotation angle value.
	 */
	double rotate() const;

	TracedObjectStyle* pathStyle() const;

	void  setPathStyle(TracedObjectStyle* style);

	/**
	 * @brief Sets object tooltip.
	 * @param arg text wihch should be displayes as tooltip. Can be presented as HTML.
	 */
	virtual void setToolTip(const QString& arg);


	const PwGisPointList* history() const;

public slots:

	/**
	 * @brief Update object presentation on map.
	 */
	virtual void updateMap();

	/**
	 * @brief Add point to trace.
	 * @param point trace point.
	 * @param pointTime time of point. this time MUST be over then previous points.
	 */
	void setHistoryPosition(PwGisLonLat* point, const QDateTime& pointTime);

	/**
	 * @brief Set position for AX do not use it.
	 * @param position current object position.
	 */
	void setPosition(PwGisLonLat* position);

	/**
	 * @brief Sets object position.
	 * @param position object position.
	 */
	virtual void setPosition(const PwGisLonLat* position);

	/**
	 * @brief Removes object from map (uses for ActiveX)
	 */
	virtual void removeFromMap();
};

#endif // TRACEDMAPOBJECT_H
