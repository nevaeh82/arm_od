#ifndef TACTICMAPOBJECTABSTRACT_H
#define TACTICMAPOBJECTABSTRACT_H

#include <objects/MapObjectAbstract.h>
#include "TacticMap_global.h"

class PwGisLonLat;
class PwGisPointList;
class IObjectsFactory;
namespace Pw {
namespace Gis {
class Marker;
class Path;
}
}

/**
 * @brief The TacticObjectAbstract class holds basic prioperties for any tactic object.
 * such as:  current position and displayed tooltip. but without graphical presentation.
 */
class TACTICMAP_EXPORT TacticObjectAbstract: public MapObjectAbstract
{
	Q_OBJECT

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(const PwGisLonLat* position READ position WRITE setPosition)


protected:
	QString _toolTip;
	PwGisLonLat* _position;
	QList<TacticObjectAbstract*> _children;

public:
	explicit TacticObjectAbstract(QString id, QString name = "", QObject *parent = NULL);
	explicit TacticObjectAbstract(QObject* parent=NULL);

	/**
	 * @brief Gets object tooltip.
	 * @return Object current tooltip.
	 */
	QString toolTip() const;

	/**
	 * @brief Sets object tooltip.
	 * @param arg text wihch should be displayes as tooltip. Can be presented as HTML.
	 */
	virtual void setToolTip(const QString& arg);

	/**
	 * @brief Gets object position.
	 * @return object position.
	 */
	const PwGisLonLat *position() const;

	/**
	 * @brief Sets object position.
	 * @param position object position.
	 */
	virtual void setPosition(const PwGisLonLat* position);


public slots:

    void updateMap();


	/**
	 * @brief Add child (linked) object.
	 * @param child child object.
	 */
	void addChild(TacticObjectAbstract* child);

	/**
	 * @brief Remove child object.
	 * @param child target object.
	 */
	void removeChild( TacticObjectAbstract* child);
};



#endif // TACTICMAPOBJECTABSTRACT_H
