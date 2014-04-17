#include "Tabs/DbBla/Defines.h"
#include "NIIPPController.h"

NiippController::NiippController(int id, QString name, QPointF latlon, IMapController* mapController,
								 ITabManager* parentTab, QObject* parent)
	: QObject(parent)
	, m_model( new Niipp( id, name, latlon, parentTab ) )
{
	m_view = NULL;

	m_mapController = mapController;
	m_mapController->setNiippController( this );
	m_mapController->updateNiippPowerZone( *m_model );

	connect( this, SIGNAL(angleChanged(double)), this, SLOT(changeAngel(double)) );
}

NiippController::~NiippController()
{
}

bool NiippController::getState()
{
	return m_view->getEnableComplexState();
}

void NiippController::setData(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int mode;

	ds >> mode;

	m_view->setStatusText(mode);

}

void NiippController::stopClicked()
{
	m_model->stopCommad();
}

void NiippController::enableComplex(bool state)
{
	m_model->enableComplex(state);
}

void NiippController::clear()
{
	m_model->clear();
	m_mapController->removeNiippPoint();
}

void NiippController::changeValuePower(int value)
{
	m_model->changeValuePower( value );
	m_view->changeValuePower( value, m_model->getRadiusCircle(),
							  m_model->getRadiusSector(), m_model->getAntenaType() );

	m_mapController->updateNiippPowerZone( *m_model );
}

void NiippController::changeAngel(double value)
{
	m_model->setAngel(value);
}

void NiippController::setPower(double value)
{
	Q_UNUSED(value);
}

void NiippController::setSwitchOn(bool state)
{
	Q_UNUSED(state);
}

void NiippController::setAntennaType(int value)
{
	if( value == 1 ) {
		m_model->setMode( m_view->getModeIndex() );
	}

	m_model->setAntennaType( value );
	m_view->setAntennaType( m_model->getAntenaType(), getModeCurrentIndex() );
	m_mapController->updateNiippPowerZone( *m_model );
}

int NiippController::getId()
{
	return m_model->getId();
}

void NiippController::setPoint(QPointF coord)
{
	m_model->setPoint(coord);

	QString lat_s = QString::number(coord.y(), 'f', 4);
	m_view->setLatText(lat_s);
	QString lon_s = QString::number(coord.x(), 'f', 4);
	m_view->setLonText(lon_s);
}

void NiippController::sendEnemyBpla(QPointF point, QPointF point_uvoda, double alt, double bearing)
{
	if(!m_view->getStartState())
	{
		return;
	}

	m_model->setAntenaIndex(m_view->getAntenaIndex());
	m_model->setSBpowerValue(m_view->getSbPowerValue());

	m_model->sendEvil(point, point_uvoda, alt, bearing);

}

void NiippController::setAngle(double angle)
{
	emit angleChanged(angle);
}

QByteArray NiippController::encode(QStringList list)
{
	return m_model->encode(list);
}

void NiippController::changeMode(int value)
{
	m_model->changeMode(value);
}

int NiippController::getAntenaType()
{
	return m_model->getAntenaType();
}

double NiippController::getRadiusCircle()
{
	return m_model->getRadiusCircle();
}

double NiippController::getRadiusSector()
{
	return m_model->getRadiusSector();
}

Niipp::WorkMode NiippController::getModeCurrentIndex()
{
	return m_model->getModeCurrentIndex();
}

void NiippController::onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole)
{
	if( uavRole != ENEMY_UAV_ROLE ) return;

	/// \todo: I don't know why numbers are 100 and 101. It's became from MapClient1
	if( getId() == 100 || getId() == 101 ) {
		QPointF point( uavInfo.lon, uavInfo.lat );
		sendEnemyBpla( point, m_model->getPoint(), uavInfo.alt, uavInfo.yaw );
	}
}

void NiippController::appendView(NiippWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(complexEnabled(bool)), this, SLOT(enableComplex(bool)));
	connect(m_view, SIGNAL(valuePowerChanged(int)), this, SLOT(changeValuePower(int)));
	connect(m_view, SIGNAL(stopClicked()), this, SLOT(stopClicked()));
	connect(m_view, SIGNAL(antennaTypeChanged(int)), this, SLOT(setAntennaType(int)));
	connect(m_view, SIGNAL(modeChanged(int)), this, SLOT(changeMode(int)));
	connect(m_view, SIGNAL(cleared()), this, SLOT(clear()));
}
