#include "BplaSimulator.h"
#include "Logger/Logger.h"

BplaSimulator::BplaSimulator(QObject* parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    m_lat = 46;
    m_lon = 34;

    r = 1;
    df = 0;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotNewPoint()));

    uav.name = QString("Sim");
    uav.role = ENEMY_UAV_ROLE;
    uav.id = 1;
    uav.uavId = 1;
    uav.device = 1;
    uav.source = 1;
    uav.lat = m_lat;
    uav.lon = m_lon;
    uav.latStddev = 10;
    uav.lonStddev = 10;
    uav.alt = 1000;
    uav.speed = 100;
    uav.yaw = 0;
    uav.restTime = QTime(0,0,0,0);
    uav.statusId = 1;
    uav.dateTime = QDateTime::currentDateTime();
    uav.historical = false;
}

BplaSimulator::~BplaSimulator()
{
    m_timer->stop();
    delete m_timer;
}

void BplaSimulator::startSim()
{
    m_timer->start();
}

void BplaSimulator::stopSim()
{
    m_timer->stop();
}

void BplaSimulator::slotNewPoint()
{

   // df=0; pi=2*asin(1); r=10;
    if(df<360)
    {
        df += 1;
    }
    else
    {
        df = 0;
    }

    log_debug(df);
    log_debug(qCos(df*M_PI/180));
    log_debug(qSin(df*M_PI/180));
    uav.lat = m_lat + r * qCos(df*M_PI/180);
    uav.lon = m_lon + r * qSin(df*M_PI/180);
    uav.dateTime = QDateTime::currentDateTime();
    QVector<QPointF> tail;
    QVector<QPointF> tailStdDev;
    int y = 9;
    emit signalGenNewPoint(uav.lat, uav.lon);
    //const UavInfo& uav, const QVector<QPointF>& tail, const QVector<QPointF>& tailStdDev
}
