#ifndef MAPSTYLEFACTORY_H
#define MAPSTYLEFACTORY_H

#include <QMap>
#include <QString>

#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/IStyleFactory.h>

#include "Map/IMapStyleManager.h"

class MapStyleManager : public IMapStyleManager
{
private:
	IStyleFactory* m_factory;
	QMap<QString, PwGisStyle*> m_styles;

public:
	MapStyleManager(IStyleFactory *factory);

	virtual PwGisStyle* createAisStyle(const QString& layerId);
	virtual PwGisStyle* createFriendBplaStyle(const QString& layerId);
	virtual PwGisStyle* createFriendBplaTrackStyle(const QString& layerId);
	virtual PwGisStyle* createFriendBplaSliceStyle(const QString& layerId);
	virtual PwGisStyle* createAdsbStyle(const QString& layerId);
	virtual PwGisStyle* createAdsbTrackStyle(const QString& layerId);
    virtual PwGisStyle* createEnemyBplaStyle(const QString& layerId, uint source, uint state);
	virtual PwGisStyle* createEnemyBplaTrackStyle(const QString& layerId, uint source);
	virtual PwGisStyle* createEnemyBplaErrorStyle(const QString& layerId, uint source);
	virtual PwGisStyle* createInterceptionStyle(const QString& layerId);
	virtual PwGisStyle* createNiippStyle(const QString& layerId);
	virtual PwGisStyle* createNiippPointStyle(const QString& layerId);
	virtual PwGisStyle* createPelengatorStyle(const QString& layerId);
	virtual PwGisStyle* createPelengatorPointStyle(const QString& layerId);
	virtual PwGisStyle* createStationStyle(const QString& layerId);
	virtual PwGisStyle* createGridStyle(const QString& layerId);
	virtual PwGisStyle* createCheckPointStyle(const QString& layerId);
	virtual PwGisStyle* createHistoryStyle(const QString& layerId);
	virtual PwGisStyle* createHyperboleStyle(const QString& layerId);
    virtual PwGisStyle* createHyperboleZoneStyle(const QString& layerId, int accur);
    virtual PwGisStyle* createKKpointStyle(const QString& layerId, const double& opacity);
    virtual PwGisStyle* createWorkAreaStyle(const QString& layerId);
    virtual PwGisStyle* createNetAreaStyle(const QString& layerId);

	inline virtual PwGisStyle* getAisStyle() { return getStyle( MAP_STYLE_NAME_AIS ); }
	inline virtual PwGisStyle* getFriendBplaStyle() { return getStyle( MAP_STYLE_NAME_FRIEND_BPLA ); }
	inline virtual PwGisStyle* getFriendBplaTrackStyle() { return getStyle( MAP_STYLE_NAME_FRIEND_BPLA_TRACK ); }
	inline virtual PwGisStyle* getFriendBplaSliceStyle() { return getStyle( MAP_STYLE_NAME_FRIEND_BPLA_SLICE ); }
    inline virtual PwGisStyle* getEnemyBplaStyle(uint source, uint state) { return getStyle( MapStyleManager::getEnemyBplaStyleName(source, state) ); }
	inline virtual PwGisStyle* getEnemyBplaTrackStyle(uint source) { return getStyle( MapStyleManager::getEnemyBplaTrackStyleName(source) ); }
	inline virtual PwGisStyle* getAdsbStyle() { return getStyle( MAP_STYLE_NAME_ADSB ); }
	inline virtual PwGisStyle* getAdsbTrackStyle() { return getStyle( MAP_STYLE_NAME_ADSB_TRACK ); }
	inline virtual PwGisStyle* getInterceptionStyle() { return getStyle( MAP_STYLE_NAME_INTERCEPTION ); }
	inline virtual PwGisStyle* getNiippStyle() { return getStyle( MAP_STYLE_NAME_NIIPP ); }
	inline virtual PwGisStyle* getNiippPointStyle() { return getStyle( MAP_STYLE_NAME_NIIPP_POINT ); }
	inline virtual PwGisStyle* getPelengatorStyle() { return getStyle( MAP_STYLE_NAME_PELENGATOR ); }
	inline virtual PwGisStyle* getPelengatorPointStyle() { return getStyle( MAP_STYLE_NAME_PELENGATOR_POINT ); }
	inline virtual PwGisStyle* getStationStyle() { return getStyle( MAP_STYLE_NAME_STATION ); }
	inline virtual PwGisStyle* getGridStyle() { return getStyle( MAP_STYLE_NAME_GRID ); }
	inline virtual PwGisStyle* getCheckpointsStyle() { return getStyle( MAP_STYLE_NAME_CHECK_POINTS ); }
	inline virtual PwGisStyle* getHistoryStyle() { return getStyle( MAP_STYLE_NAME_HISTORY ); }
	inline virtual PwGisStyle* getHyperboleStyle() { return getStyle( MAP_STYLE_NAME_HYPERBOLE ); }

	static inline QString getEnemyBplaStyleName(uint source, uint state) { return QString(MAP_STYLE_NAME_ENEMY_BPLA).arg(source).arg(state); }
	static inline QString getEnemyBplaTrackStyleName(uint source) { return QString(MAP_STYLE_NAME_ENEMY_BPLA_TRACK).arg(source); }
	static inline QString getEnemyBplaErrorStyleName(uint source) { return QString(MAP_STYLE_NAME_ENEMY_BPLA_ERROR).arg(source); }

	static inline QString getADSBStyleName() { return QString(MAP_STYLE_NAME_ADSB); }
	static inline QString getADSBTrackStyleName() { return QString(MAP_STYLE_NAME_ADSB); }

    virtual PwGisStyle *createStationRebStyle(const QString &layerId, int active);
    virtual PwGisStyle *createStationPvoStyle(const QString &layerId, int active);
protected:
	/// Creates style with \a name
	/// \return false, is style was created before
	PwGisStyle* createStyle(const QString& name, bool* exists);

	/// Returns style from style map, associated with \a name,
	/// otherwise NULL
	PwGisStyle* getStyle(const QString& name, bool log = true);

	/// Adds \a style to map, associated with \a name
	void setStyle(const QString& name, PwGisStyle* style);
};

#endif // MAPSTYLEFACTORY_H
