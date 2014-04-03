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

namespace MapFeature {

/// Station feature draws in PwGis map
class Station
{

public:
	Station( PwGisWidget* pwwidget, QString layerId );
	~Station();

	int readFromFile( QString fileName );

private:
	PwGisWidget* m_pwwidget;
};

} // namespace MapFeature

#endif // STATIONFEATURE_H
