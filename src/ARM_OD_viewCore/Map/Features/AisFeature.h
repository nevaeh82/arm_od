#ifndef AISFEATURE_H
#define AISFEATURE_H

#include <QString>
#include <QMap>
#include <PwGis/pwgiswidget.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>

namespace MapFeature {

/// AIS feature draws in PwGis map
class Ais
{

public:
	Ais( PwGisWidget* pwwidget, QString layerId );
	~Ais();

	void add( QMap<int, QVector<QString> > aisMap );
	void remove();

private:
	PwGisWidget* m_pwwidget;
	QMap<int, QVector<QString> > m_mapAis;
};

} // namespace MapFeature

#endif // AISFEATURE_H
