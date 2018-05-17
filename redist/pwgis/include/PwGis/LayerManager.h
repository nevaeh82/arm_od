#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <ILayerManager.h>
#include <QString>
#include <QStringList>
#include <QHash>
#include "PwGisClient.h"

class PwGisWidget;

namespace Pw { namespace Gis {

/**
 * @brief The LayerManager class provides service for map layers control
 */
class GISCLIENT_EXPORT LayerManager : public QObject, public ILayerManager
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{d8ea4533-8e4e-44ec-800b-bf53aeef1006}")
	Q_CLASSINFO("InterfaceID", "{23d1fa19-a5f2-4fac-ae7d-fd603c51610f}")
	Q_CLASSINFO("CoClassAlias", "LayerManager")

private:
	PwGisWidget* _gis;

public:
	/// Creates a new instance of LayerManager class with default enviroment.
	/// @param parent  parent QObject.
	LayerManager(QObject* parent=0);

	/// Creates a new instance of LayerManager class.
	/// @param gis  PwGisWidget
	/// @param parent  parent QObject.
	LayerManager(PwGisWidget* gis, QObject* parent);

	virtual ~LayerManager();

public slots:
	/// adds new markers layers to map
	/// @param layerId  unique layer id
	/// @param layerName  layer name showing on map
	virtual void addMarkerLayer(const QString& layerId, const QString& layerName);

	/// adds new vectors layers to map
	/// @param layerId  unique layer id
	/// @param layerName  layer name showing on map
	virtual void addVectorLayer(const QString& layerId, const QString& layerName);

	/// toggle layer visibility
	/// @param layerId  unique layer id
	/// @param visible  visibility key
	virtual void setLayerVisibility(const QString& layerId, bool visible);

	/// gets layer visibility
	/// @param layerId  unique layer id
	virtual bool isVisible(const QString& layerId);

	/// Gets layers list
	/// @return list of layers existing on map control panel (id-name pairs)
	virtual QHash<QString,QString> getLayersList();

	/// Gets layers id list
	/// @return list of layers existing on map control panel (ids only)
	virtual QStringList getLayersIds();

	/// Gets layers names list
	/// @return list of layers existing on map control panel (names only)
	virtual QStringList getLayersNames();
};


} // namespace Gis
} // namespace Pw

#endif // LAYERMANAGER_H
