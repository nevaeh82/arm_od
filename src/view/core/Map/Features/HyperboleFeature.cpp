#include "Map/Features/HyperboleFeature.h"

#include "Logger/Logger.h"


namespace MapFeature {

Hyperbole::Hyperbole(
    IStyleFactory* styleFactory,
    IObjectsFactory* factory,
    const QString& id,
    const QVector<QPointF>& polyline,
    const QTime timeMeasure,
    const QColor color )
	: PolylineAbstract( factory, id, "", polyline )
	, m_customStyle( NULL )
    , m_timeMeasure( timeMeasure )
    , m_zone(NULL)
{
	m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );

	// adds custom color
	if ( color.isValid() ) {
		m_customStyle = styleFactory->createStyle();
		m_customStyle->setProperty( PwGisStyle::strokeColor, color.name() );
		m_customStyle->apply();

		m_path->addStyle( m_customStyle );
	}

//    QVector<QPointF> poly2;
//    for(int i = 0; i<polyline.size(); i++) {
//        if( !(i%4) ) {
//            poly2.append( polyline.at(i) );
//        }
//    }
    updatePath( polyline, timeMeasure );
}

Hyperbole::Hyperbole(IStyleFactory *styleFactory,
                     IObjectsFactory *factory,
                     const QString &id,
                     const QVector<QPointF> &polyline,
                     const QVector<QPointF> &polyZone,
                     const QTime timeMeasure,
                     const QColor color)
    : PolylineAbstract( factory, id, "", polyline )
    , m_customStyle( NULL )
    , m_timeMeasure( timeMeasure )
    , m_zone(NULL)
    , m_currentFactory(factory)
{
    m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );
    m_zone = m_factory->createPolygon();
    m_zone->addStyleByName( QString(MAP_STYLE_NAME_HYPERBOLE_ZONE).arg(10) );

    // adds custom color
    if ( color.isValid() ) {
        m_customStyle = styleFactory->createStyle();
        m_customStyle->setProperty( PwGisStyle::strokeColor, color.name() );
        m_customStyle->apply();

        m_path->addStyle( m_customStyle );
    }

    updatePath( polyline, polyZone, timeMeasure );
}

Hyperbole::~Hyperbole()
{
	if( m_customStyle != NULL ) {
		delete m_customStyle;
	}
    if(m_zone) {
        m_zone->removeFromMap();
        delete m_zone;
    }
}

void Hyperbole::updatePath(const QVector<QPointF>& polyline, const QTime timeMeasure )
{
    setPolyline( polyline );
    //log_debug(QString("HYPERBOLE size:: %1").arg(polyline.size()));

	m_path->setToolTip( timeMeasure.toString( Qt::SystemLocaleShortDate ) );

    updateMap();
}

void Hyperbole::updatePath(const QVector<QPointF> &polyline,
                           const QVector<QPointF> &polyZone, const QTime timeMeasure)
{
    setPolyline( polyline );
    //log_debug(QString("HYPERBOLE size:: %1").arg(polyline.size()));

    //-----
    if(m_zone) {
        PwGisPointList* points = m_zone->corners();
        points->clear();

        foreach( QPointF point, polyZone ) {
            if( point.y() < -180 || point.y() > 180 || point.x() > 180 || point.y() < -180 ) {
                continue;
            }
            points->append( new PwGisLonLat( point.y(), point.x() ) );
        }

        QPointF accur = polyZone.last();
        int wholeacc = (1000 / accur.x())*10;
        m_zone->addStyleByName( QString(MAP_STYLE_NAME_HYPERBOLE_ZONE).arg(3) );

        m_zone->updateMap();
    }
    //-----

    m_path->setToolTip( timeMeasure.toString( Qt::SystemLocaleShortDate ) );

    updateMap();
}

void Hyperbole::removeFromMap()
{
    if(m_zone) {
        m_zone->removeFromMap();
        delete m_zone;
        m_zone = 0;
    }

    PolylineAbstract::removeFromMap();
}

} // namespace MapFeature


