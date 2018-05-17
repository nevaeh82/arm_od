#ifndef RADIOSOURCESTYLEE_H
#define RADIOSOURCESTYLEE_H
#include <QObject>
#include "TracedObjectStyle.h"
#include "../TacticMap_global.h"

class MapObjectStyle;
class PwGisWidget;
class IStyleFactory;

/**
 * @brief The RadioSourceStyle class holds all styles for Radio source object.
 */
class TACTICMAP_EXPORT RadioSourceStyle: public TracedObjectStyle
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{b9688ae0-d031-4f07-99ea-ed8f5b4d71be}")
	Q_CLASSINFO("InterfaceID", "{4e070872-507a-4859-8f4c-2ccece6c279c}")


public:

	/**
	 * @brief Initializes a new instance of RadioSourceStyle
	 * @param parent   Qt parent object for memory management.
	 */
	explicit RadioSourceStyle(IStyleFactory* styleFactory, QObject* parent);

	/**
	 * @brief Default consyructor for AxFactory.
	 * @param parent Qt parent object.
	 */
	explicit RadioSourceStyle(QObject* parent = NULL);
};

#endif // RADIOSOURCESTYLEE_H
