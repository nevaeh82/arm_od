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


/** Bla feature draw in PwGis map
  */
class BlaFeature
{

public:
	BlaFeature( PwGisWidget* pwwidget, QMap< int, PwGisPointList* > *lastCoord,
				QString layerId );
	~BlaFeature();

	void add( int id, QPointF point );

private:
	PwGisWidget* m_pwwidget;
	QMap< int, PwGisPointList* > *m_lastCoord;
};


#endif // BLAFEATURE_H
