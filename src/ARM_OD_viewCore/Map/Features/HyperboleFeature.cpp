#include "Map/Features/HyperboleFeature.h"


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
{
	m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );

	// adds custom color
	if ( color.isValid() ) {
		m_customStyle = styleFactory->createStyle();
		m_customStyle->setProperty( PwGisStyle::strokeColor, color.name() );
		m_customStyle->apply();

		m_path->addStyle( m_customStyle );
	}

	updatePath( polyline, timeMeasure );
}

Hyperbole::~Hyperbole()
{
	if( m_customStyle != NULL ) {
		delete m_customStyle;
	}
}

void Hyperbole::updatePath(const QVector<QPointF>& polyline, const QTime timeMeasure )
{
	PolylineAbstract::setPolyline( polyline );

	m_path->setToolTip( timeMeasure.toString( Qt::SystemLocaleShortDate ) );

	updateMap();
}

} // namespace MapFeature


