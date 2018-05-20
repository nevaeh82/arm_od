#ifndef TACTICMARKERSTYLE_H
#define TACTICMARKERSTYLE_H

#include <QObject>
#include <QMutex>
#include "../TacticMap_global.h"

class MapObjectBehaviourStyle;
class TrackingStyle;
class MapObjectStyle;
class IStyleFactory;
class LineStyle;
class FillStyle;
class IconStyle;
class TextStyle;
class UserInteractionStyle;

/**
 * @brief The TacticMarkerStyle class provides style for TracedObjectAbstract
 */
class TACTICMAP_EXPORT TracedObjectStyle : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{9219f72b-0eee-4f30-a25d-c8c8f36fc4d5}")
	Q_CLASSINFO("InterfaceID", "{ee9add2f-4cc7-4349-8ec2-be0bfddf8ea5}")

	Q_PROPERTY (TrackingStyle* tracking READ tracking)
	Q_PROPERTY (MapObjectStyle* pathStyle READ path)
	Q_PROPERTY (LineStyle* line READ line)
	Q_PROPERTY (UserInteractionStyle* userInteraction READ userInteraction)
	Q_PROPERTY (IconStyle* icon READ icon)
	Q_PROPERTY (TextStyle* text READ text)

private:
	QMutex _mutex;

	TrackingStyle*  _tracking;
	MapObjectStyle* _path;
	MapObjectStyle* _marker;

public:
	/**
	 * @brief Initializes a new instance of TracedObjectStyle.
	 * @param parent object for memory management.
	 */
	explicit TracedObjectStyle(IStyleFactory* factory, QObject *parent);
	explicit TracedObjectStyle(QObject *parent = 0);

	virtual ~TracedObjectStyle(){}
public:
	/**
	 * @brief Gets tracking behaviour style.
	 * @return Tracking style instance.
	 */
	TrackingStyle* tracking() const;

	/**
	 * @brief Gets track presentation style.
	 * @return Track presentation style instance.
	 */
	MapObjectStyle* path() const;

	/**
	 * @brief Gets marker style.
	 * @return Marker presentation style.
	 */
	MapObjectStyle* marker() const;

	/**
	 * @brief Gets line style.
	 * @return Path line style.
	 */
	LineStyle* line() const;

	/**
	 * @brief Gets user interation style.
	 * @return user interaction style.
	 */
	UserInteractionStyle* userInteraction() const;

	/**
	 * @brief Gets marker icon style.
	 * @return Marker icon style.
	 */
	IconStyle* icon() const;

	/**
	 * @brief Gets marker text style.
	 * @return Marker text style.
	 */
	TextStyle* text() const;

};

#endif // TACTICMARKERSTYLE_H
