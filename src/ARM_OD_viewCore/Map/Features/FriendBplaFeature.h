#ifndef BLAFEATURE_H
#define BLAFEATURE_H

#include <QString>
#include <QMap>

#include <PwGis/pwgiswidget.h>
#include <PwGis/PwGisLonLat.h>
#include <PwGis/PwGisPointList.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// Bla feature draw in PwGis map
class FriendBpla
{

public:
	FriendBpla( PwGisWidget* pwwidget, QMap< int, PwGisPointList* > *lastCoord,
				QString layerId );
	~FriendBpla();

	void add( int id, QPointF point );

private:
	PwGisWidget* m_pwwidget;
	QMap< int, PwGisPointList* > *m_lastCoord;
};

} // namespace MapFeature

#endif // BLAFEATURE_H
