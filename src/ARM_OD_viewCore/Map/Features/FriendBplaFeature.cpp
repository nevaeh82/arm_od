#include "Map/IMapStyleManager.h"
#include "Map/Features/FriendBplaFeature.h"

namespace MapFeature {

FriendBpla::FriendBpla(IObjectsFactory* factory , const QString& id, int bplaId,
					   QPointF position)
	: Marker( factory, id, QString(), position )
{
	m_marker->addStyleByName( MAP_STYLE_NAME_FRIEND_BPLA );

	m_tail = factory->createPath();
	m_tail->addStyleByName( MAP_STYLE_NAME_FRIEND_BPLA );

	setName( QString::number( bplaId ) );
	setPosition( position );
}

FriendBpla::~FriendBpla()
{
	m_tail->removeFromMap();
	delete m_tail;
}

void FriendBpla::setName(const QString& name)
{
	int id = name.toInt();
	if( id > 0 ) {
		QString newName = id == 1044 ? "БЛА-Ц" : (QString( "БЛА (№%1)" ).arg( id ));
		Marker::setName( newName );
	}
}

void FriendBpla::setPosition(const QPointF& position)
{
	Marker::setPosition( position );

	PwGisPointList* points = m_tail->points();

	points->append( new PwGisLonLat( m_position ) );

	while( points->length() > 100 ) {
		points->removeFirst();
	}

	if ( points->first() == points->last() ) {
		removeFromMap();
		return;
	}
}

void FriendBpla::updateMap()
{
	m_tail->updateMap();
	Marker::updateMap();
}

void FriendBpla::removeFromMap()
{
	m_tail->removeFromMap();
	Marker::removeFromMap();
}

} // namespace MapFeature
