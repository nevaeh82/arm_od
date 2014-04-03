#ifndef BPLAFEATURE_H
#define BPLAFEATURE_H

#include <QString>
#include <QMap>
#include <qmath.h>
#include <PwGis/pwgiswidget.h>
#include <PwGis/objects/PwGisStyle.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// Enemy BPLA feature draws in PwGis map
class EnemyBpla
{

public:
	EnemyBpla( PwGisWidget* pwwidget, QString layerId );
	~EnemyBpla();

	void add( int id, QPointF sko, QVector<QPointF> *track,
		double alt );

private:
	PwGisWidget* m_pwwidget;
	QMap<int, QVector<QString> > m_mapAis;
};

} // namespace MapFeature

#endif // BPLAFEATURE_H
