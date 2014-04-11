#include "Map/IMapStyleManager.h"
#include "Map/Features/InterceptionFeature.h"

namespace MapFeature {

Interception::Interception(IObjectsFactory* factory, const QString& id,
						   int friendBplaId, int enemyBplaId, const QPointF& position,
						   float height, float radius, float course, float speed)
	: FeatureAbstract( factory, id, "", position )
	, m_friendBplaId( friendBplaId )
	, m_enemyBplaId( enemyBplaId )
	, m_height( height )
	, m_course( course )
	, m_speed( speed )
{
	m_circle = factory->createCircle();
	m_circle->setOriginPoint( &m_position );
	m_circle->addStyleByName( MAP_STYLE_NAME_INTERCEPTION );

	setRadius( radius );
}

void Interception::updatePresentation()
{
	QString caption = QObject::tr( "Высота" ) + " = " + QString::number( m_height ) + "\\n";
	caption.append( QObject::tr( "Радиус" ) + " = " + QString::number( m_radius ) + "\\n" );
	caption.append( QObject::tr( "Курс" ) + " = " + QString::number( m_course ) + "\\n" );
	caption.append( QObject::tr( "Скорость" ) + " = " + QString::number( m_speed ) );

	m_circle->setName( caption );

	m_circle->setRadius( m_radius );
}

Interception::~Interception()
{
	m_circle->removeFromMap();
	delete m_circle;
}

void Interception::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );
	m_circle->setOriginPoint( &m_position );
}

void Interception::setRadius(float value)
{
	m_radius = value;
	updatePresentation();
}

void Interception::setHeight(float value)
{
	m_height = value;
	updatePresentation();
}

void Interception::setCourse(float value)
{
	m_course = value;
	updatePresentation();
}

void Interception::setSpeed(float value)
{
	m_speed = value;
	updatePresentation();
}

void Interception::update(const QPointF& position, float height, float radius, float course, float speed)
{
	bool changed = false;

	if( position != this->position() ) {
		setPosition( position );
		changed = true;
	}

	if( height != m_height ) {
		m_height = height;
		changed = true;
	}

	if( radius != m_radius ) {
		m_radius = radius;
		changed = true;
	}

	if( course != m_course ) {
		m_course = course;
		changed = true;
	}

	if( speed != m_speed ) {
		m_speed = speed;
		changed = true;
	}

	if( changed ) {
		updatePresentation();
		updateMap();
	}
}

void Interception::updateMap()
{
	m_circle->updateMap();
}

void Interception::removeFromMap()
{
	m_circle->removeFromMap();
}

} // namespace MapFeature

