#ifndef BPLAFEATURE_H
#define BPLAFEATURE_H

#include <QTimer>

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/BplaAbstractFeature.h"


#define LIFETIME_1 5000
#define LIFETIME_2 3000

namespace MapFeature {

class FeaturesFactory;

/// Enemy BPLA feature representation on map
class EnemyBpla : public BplaAbstract
{
	friend class FeaturesFactory;

protected:
	QString m_nameTranslated;
	QString m_originName;
	QString m_targNameTranslated;

	QString m_styleName;
	QString m_trackStyleName;
	QString m_errorStyleName;

	inline void updateName() { setName( m_originName ); }

	EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

	int m_isExtraInfo;

private:
	QTimer* timer;
	int lifetime;
    int m_state;
	UavInfo m_info;

public:
	virtual ~EnemyBpla() {}

	virtual void setName(const QString &name);

	virtual void update(const UavInfo &uav);

	virtual inline QString getStyleName() const { return m_styleName; }
	virtual inline QString getTrackStyleName() const { return m_trackStyleName; }
	virtual inline QString getSliceStyleName() const { return m_trackStyleName; }
	virtual inline QString getErrorStyleName() const { return m_errorStyleName; }

	void setExtraInfo(int val);

	UavInfo getInfo() {return m_info;}
};

} // namespace MapFeature

#endif // BPLAFEATURE_H
