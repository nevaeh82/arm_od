#ifndef BEARING_H
#define BEARING_H
#include <QObject>
#include <QDateTime>
#include <QMutex>

#include <objects/Path.h>

#include "TacticMap_global.h"
#include "TacticObjectAbstract.h"

class Path;
class PwGisPointList;
class BearingStyle;
class BearingBehaviourStyle;
class TestBearing;

/// Bearing tactic map object
class TACTICMAP_EXPORT Bearing: public TacticObjectAbstract
{
	friend class TestBearing;
	friend class TestRemoveObject;

	Q_OBJECT

	Q_CLASSINFO("ClassID", "{0f2bfc8e-1d43-48d7-9eb3-9a42982cf8b4}")
	Q_CLASSINFO("InterfaceID", "{d9c4f242-b827-4667-b98a-defb51146961}")

private:
	/**
	 * @brief The Direction class describes one direction of bearing.
	 */
	class TACTICMAP_EXPORT Direction : public QObject
	{
	private:
		Bearing* _parent;
		Pw::Gis::Path* _beam;
		QDateTime _timestamp;

	public:

		Direction(PwGisLonLat* point, double angle, QDateTime timestamp, Bearing *parent);
		virtual ~Direction();

		/**
		 * @brief Update direction presentation on the map.
		 */
		void update();

		/**
		 * @brief Indicathes whether direction is valid now.
		 * @param minTime  bound time.
		 * @return bool if direction is valid now.
		 */
		bool isValid( const QDateTime& minTime);

		/**
		 * @brief Gets direction timestmap .
		 * @return Direction timestamp;
		 */
		QDateTime timestmap();

		/**
		 * @brief Delete direction from map and from memory.
		 * @param provider map provier.
		 */
		void deleteSelf();
	};
	friend class Direction;

private:
	IObjectsFactory* _mapFactory;
	BearingStyle* _style;
	QList<Direction*> _directions;
	QList<Direction*> _newDirections;
	QList<Direction*> _removedDirections;
	QDateTime _lastTimestmap;
	QMutex _mutex;

	IObjectsFactory* mapFactory();

	/**
	 * @brief remove oldest direction.
	 */
	void removeFirst();

	/**
	 * @brief Gets directions array. This method required only for tests.
	 * @return List of directions.
	 */
	const QList<Direction *> &directions();

public:
	explicit Bearing(IObjectsFactory* _mapFactory, const QString& id, const QString& name = "", QObject* parent = NULL);
	explicit Bearing(QObject* parent=0);



	virtual ~Bearing();
protected:
	BearingStyle* style() const;

public slots:
	virtual void updateMap();

	void setStyle(BearingStyle* arg);

	/**
	 * @brief Add direction to target object.
	 *  After adding direction, new direction will be added to map in update() method.
	 *  This method uses geodesic angels:
	 *   * angle changes clockwise
	 *   * 0 degrees - direction to the North
	 *   * 90 degrees - direction to the East
	 * @param point  direction source point. (origin)
	 * @param angle  Angle to target.
	 * @param timestamp  historian timestamp. Each timestmap must be newer then previos timestamp.
	 */
	void addDirection( PwGisLonLat* point, double angle, QDateTime timestamp );

	/**
	 * @brief Removes bearing from map (uses for ActiveX)
	 */
	void removeFromMap();

};

#endif
