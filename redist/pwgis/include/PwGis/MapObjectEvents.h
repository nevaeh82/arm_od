#ifndef MAPOBJECTEVENTS_H
#define MAPOBJECTEVENTS_H

#include <QObject>
#include <PwGisClient.h>

namespace Pw
{
namespace Gis
{

class GISCLIENT_EXPORT MapObjectEvents : public QObject
{
	Q_OBJECT
	Q_CLASSINFO( "ClassID", "{3f3b14af-7179-4bf4-93c8-55ab556c96cd}" )
	Q_CLASSINFO( "InterfaceID", "{cf22a20c-37c7-44fe-8469-04af109822cb}" )
	Q_CLASSINFO( "EventsID","{e9f504b1-fe7d-42e1-babf-413e85e286eb}" )

public:
	explicit MapObjectEvents( QObject *parent = 0 );

signals:
	/// Signal caused when marker selected by use via mouse over action.
	/// @param id  target marker ID.
	void markerSelected( const QString& id, const QString& type );

	/// Signal caused when marker unselected by use via mouse over action.
	/// @param id  target marker ID.
	void markerUnselected( const QString& id, const QString& type );

	/// Signal caused when feature selected by use via mouse over action.
	/// @param id  target marker ID.
	void featureSelected( const QString& id, const QString& type );

	/// Signal caused when feature unselected by use via mouse away action.
	/// @param id  target marker ID.
	void featureUnselected( const QString& id, const QString& type );

	/// Emited when feature drag initiated
	/// @param id of a feature.
	void featureDragStart( const QString& id, const QString& type );

	/// Emited while drag in progress
	/// @param id of a feature.
	void featureDrag( const QString& id, const QString& type );

	/// Emited when drag finished
	/// @param id of a feature.
	void featureDragFinish( const QString& id, const QString& type );
	/// Emited when feature clicked
	/// @param id of a feature.
	void featureClicked( const QString& id, const QString& type );

	/// Emited when feature added
	/// @param id of a feature.
	void featureAdded(const QString& id, const QString& type );

	/// Emited when feature was modified by user
	/// @param id of a feature.
	void featureModified( const QString& id, const QString& type );


public slots:
	/// Slot calls by JS when new object added
	/// @param type  objects type
	/// @param id  objects id
	virtual void onFeatureAdded(const QString& id, const QString& type);

	/// Slot calls by JS when object modified
	/// @param id  objects id
	virtual void onFeatureModified(const QString& id, const QString& type);
};

} //namespace Gis
} //namespace Pw

#endif // MAPOBJECTEVENTS_H
