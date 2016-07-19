#pragma once

#include <QTimer>

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/MarkerFeature.h"
#include "Map/Features/BplaAbstractFeature.h"

#include "Map/Features/PopupFeature.h"


#define LIFETIME_ADSB 60000

namespace MapFeature {

class FeaturesFactory;

class ADSBPlaneFeature : public Marker
{
	Q_OBJECT
	friend class FeaturesFactory;

protected:
	QString m_nameTranslated;
	QString m_originName;

	QString m_styleName;
	QString m_trackStyleName;

	inline void updateName() { setName( m_originName ); }

	ADSBPlaneFeature(MapWidget* view, IObjectsFactory* factory, const QString& id, const QPointF& pos);

private:
	QTimer* timer;
	int lifetime;

	Pw::Gis::Path* m_tail;

	PopupFeature* m_popup;

public:
	virtual ~ADSBPlaneFeature();

	virtual void setName(const QString &name);

	virtual void update(const QPointF &uav, const double& yaw = 0);

	virtual inline QString getStyleName() const { return m_styleName; }
	virtual inline QString getTrackStyleName() const { return m_trackStyleName; }
	virtual inline QString getSliceStyleName() const { return m_trackStyleName; }

	void onClicked();

	void updateMap();

	QString getMapId() const;
private slots:
	void removeFeature();

signals:
	void onFeatureRemove(QString);

};

} // namespace MapFeature
