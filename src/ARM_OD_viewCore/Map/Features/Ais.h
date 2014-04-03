#ifndef AISFEATURE_H
#define AISFEATURE_H

#include <QString>
#include <QMap>
#include <PwGis/pwgiswidget.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>


/** Ais feature draw in PwGis map
  */
class AisFeature
{

public:
	AisFeature( PwGisWidget* pwwidget, QString layerId );
	~AisFeature();

	void add( QMap<int, QVector<QString> > aisMap );
	void remove();

private:
	PwGisWidget* m_pwwidget;
	QMap<int, QVector<QString> > m_mapAis;
};


#endif // AISFEATURE_H
