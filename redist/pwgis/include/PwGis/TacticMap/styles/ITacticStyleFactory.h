#ifndef ITACTICSTYLEFACTORY_H
#define ITACTICSTYLEFACTORY_H
#include "../TacticMap_global.h"
#include <QObject>

class BearingStyle;
class SectorStyle;
class PostStyle;
class RadioSourceStyle;
class MapObjectStyle;

/// This interfaces provides tactic objects styles factory
class TACTICMAP_EXPORT ITacticStyleFactory
{
public:
	virtual ~ITacticStyleFactory(){}

	/// Method performs Bearing style creation
	/// @return Bearing style pointer
	virtual BearingStyle* createBearingStyle() = 0;

	/// Method performs Post style creation
	/// @return Post style pointer
	virtual PostStyle* createPostStyle() = 0;

	/// Method performs RadioSourceStyle style creation
	/// @return RadioSourceStyle style pointer
	virtual RadioSourceStyle* createRadioSourceStyle() = 0;


	/// Method performs MapObjectStyle style creation
	/// @return RadioSourceStyle style pointer
	virtual MapObjectStyle* createMapObjectStyle() = 0;
};
Q_DECLARE_INTERFACE(ITacticStyleFactory, "com.pw.gis.ITacticStyleFactory/1.0")


#endif // ITACTICSTYLEFACTORY_H
