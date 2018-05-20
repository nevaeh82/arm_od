#ifndef ISTYLEFACTORY_H
#define ISTYLEFACTORY_H

class PwGisStyle;
class MapObjectStyle;


/// This interface provides map style factory functionality
class IStyleFactory
{
public:
	/// Method performs new Style object creation
	/// @param name Name of style
	virtual PwGisStyle* createStyle() = 0;

	/// Creates named style for map objects
	/// @param style style name for new style
	/// @return Created style.
	virtual PwGisStyle* createNamedStyle(const QString& styleName)=0;

	/// Creates unnamed style for map objects
	/// @return Created style.
	virtual MapObjectStyle* createObjectStyle() =0;
	
	virtual ~IStyleFactory() {}
};


#endif // ISTYLEFACTORY_H
