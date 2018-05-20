#ifndef ITACTICMAPFACTORY_H
#define ITACTICMAPFACTORY_H
#include "TacticMap_global.h"
#include <QObject>

class Bearing;
class Post;
class RadioSource;
class Sector;
class Bearing;


/// This interface provides tactic map objects factory functionality
class TACTICMAP_EXPORT ITacticObjectsFactory
{
public:
	virtual ~ITacticObjectsFactory(){}

	/// Method performs Post map object creation
	/// @return Post map object pointer
	virtual Post* createPost() = 0;

	/// Method performs RadioSource map object creation
	/// @return RadioSource map object pointer
	virtual RadioSource* createRadioSource() = 0;

	/// Method performs Bearing map object creation
	/// @return Bearing map object pointer
	virtual Bearing* createBearing() = 0;
};
Q_DECLARE_INTERFACE(ITacticObjectsFactory, "com.pw.gis.ITacticObjectsFactory/1.0")

#endif // ITACTICMAPFACTORY_H
