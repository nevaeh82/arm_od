#ifndef STATIONFEATURE_H
#define STATIONFEATURE_H

#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QSettings>
#include <PwGis/pwgiswidget.h>

#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>


/** Station feature draw in PwGis map
  */
class StationFeature
{

public:
	StationFeature( PwGisWidget* pwwidget, QString layerId );
	~StationFeature();

	int readFromFile( QString fileName );

private:
	PwGisWidget* m_pwwidget;
};


#endif // STATIONFEATURE_H
