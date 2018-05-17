#ifndef ILAYERMANAGER_H
#define ILAYERMANAGER_H

#include <QHash>

class QString;

namespace Pw { namespace Gis {

/**
 * @brief The ILayerManager class provides service for map layers control
 */
 
class ILayerManager
{
public:
	/// Virtual destructor
	virtual ~ILayerManager(){}

	/// adds new markers layers to map
	/// @param layerId  unique layer id
	/// @param layerName  layer name showing on map
	virtual void addMarkerLayer(const QString& layerId, const QString& layerName) = 0;

	/// adds new vectors layers to map
	/// @param layerId  unique layer id
	/// @param layerName  layer name showing on map
	virtual void addVectorLayer(const QString& layerId, const QString& layerName) = 0;

	/// toggle layer visibility
	/// @param layerId  unique layer id
	/// @param visible  visibility key
	virtual void setLayerVisibility(const QString& layerId, bool visible) = 0;

	/// gets layer visibility
	/// @param layerId  unique layer id
	virtual bool isVisible(const QString& layerId) = 0;

	/// Gets layers list
	/// @return list of layers existing on map control panel (id-name pairs)
	virtual QHash<QString,QString> getLayersList() = 0;

	/// Gets layers id list
	/// @return list of layers existing on map control panel (ids only)
	virtual QStringList getLayersIds() = 0;

	/// Gets layers names list
	/// @return list of layers existing on map control panel (names only)
	virtual QStringList getLayersNames() = 0;
};

} // namespace Gis
} // namespace Pw

#endif // ILAYERMANAGER_H
