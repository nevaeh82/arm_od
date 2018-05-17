#ifndef POSTSTYLE_H
#define POSTSTYLE_H
#include <QObject>
#include "TracedObjectStyle.h"
#include "../TacticMap_global.h"

class MapObjectStyle;
class PwGisWidget;
class IStyleFactory;

/**
 * @brief The PostStyle class holds all styles for Post object.
 */
class TACTICMAP_EXPORT PostStyle: public TracedObjectStyle
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{e123e952-06f0-4ed7-ab5e-4b21ab5c31db}")
	Q_CLASSINFO("InterfaceID", "{d720d9c2-c5cc-4ac4-8041-516b7624eeb1}")

public:

	/**
	 * @brief Initializes a new instance of PostStyle
	 * @param parent   Qt parent object for memory management.
	 */
	explicit PostStyle(IStyleFactory* styleFactory, QObject* parent);

	/**
	 * @brief Default consyructor for AxFactory.
	 * @param parent Qt parent object.
	 */
	explicit PostStyle(QObject* parent=NULL);

};

#endif // RADIOSOURCESTYLEE_H
