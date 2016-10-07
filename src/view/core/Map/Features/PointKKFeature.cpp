#include "Map/IMapStyleManager.h"
#include "PointKKFeature.h"

#define TIMEOUTKK 5000

namespace MapFeature {

PointKK::PointKK(PwGisWidget* wgt, const QPointF &position, const QDateTime &createTime, QObject *parent) : QObject(parent), m_bright(1)
{
    m_widget = wgt;
    m_kkId = QString("KKpoint_%1").arg(createTime.toMSecsSinceEpoch());

    m_pos = position;
    m_widget->addPoint(m_kkId, m_pos.x(), m_pos.y(), "", "", QString(MAP_STYLE_NAME_POINTKK).arg(m_bright));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onKKpointAction()));

    m_timer.start(TIMEOUTKK);
}

PointKK::~PointKK()
{
    m_widget->removeObject( m_kkId );
}

void PointKK::onKKpointAction()
{
    m_bright = m_bright - 0.1;

    if(m_bright <= 0) {
        m_timer.stop();
        this->deleteLater();
        return;
    }

    QString opacitySt = QString::number( m_bright, 'g', 1 );

    m_widget->removeObject( m_kkId );
    m_kkId = QString("KKpoint_%1").arg(QDateTime::currentMSecsSinceEpoch());

    m_widget->addPoint( m_kkId, m_pos.x(), m_pos.y(), "", "", QString(MAP_STYLE_NAME_POINTKK).arg(opacitySt) );
}



} // namespace MapFeature
