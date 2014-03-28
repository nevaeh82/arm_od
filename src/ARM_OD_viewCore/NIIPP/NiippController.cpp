#include "NIIPPController.h"

#include <QDebug>

NiippController::NiippController(int id, QString name, QPointF latlon, IRouter *router, MapController* map_controller, ITabManager* parent_tab)
	: m_controlView( new NiippWidget() )
	, m_controlModel( new Niipp(id, name, latlon, router, map_controller, parent_tab) )
{

	connect(this, SIGNAL(angleChanged(double)), this, SLOT(changeAngel(double)));
	map_controller->get_map_client(1)->setNiippController(this);
	m_controlModel->create();

	connect(m_controlView, SIGNAL(complexEnabled(bool)), this, SLOT(enableComplex(bool)));
	connect(m_controlView, SIGNAL(valuePowerChanged(int)), this, SLOT(changeValuePower(int)));
	connect(m_controlView, SIGNAL(stopClicked()), this, SLOT(stopClicked()));
	connect(m_controlView, SIGNAL(antennaTypeChanged(int)), this, SLOT(set_antenna_type(int)));
	connect(m_controlView, SIGNAL(modeChanged(int)), this, SLOT(changeMode(int)));
	connect(m_controlView, SIGNAL(cleared()), this, SLOT(clear()));
}

NiippController::~NiippController()
{
}

bool NiippController::getState()
{
	return m_controlView->getEnableComplexState();
}

void NiippController::setData(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int mode;

	ds >> mode;

	m_controlView->setStatusText(mode);

}

void NiippController::stopClicked()
{
	m_controlModel->stopCommad();
}

void NiippController::enableComplex(bool state)
{
	m_controlModel->enableComplex(state);
}

void NiippController::clear()
{
	m_controlModel->clear();
}

void NiippController::changeValuePower(int value)
{
	m_controlModel->changeValuePower(value);
	m_controlView->changeValuePower( value, m_controlModel->getRadiusCircle()
									, m_controlModel->getRadiusSector(), m_controlModel->getAntenaType() );
}

void NiippController::changeAngel(double value)
{
	m_controlModel->setAngel(value);
}

void NiippController::set_power(double value)
{
}

void NiippController::set_switch_on(bool state)
{
}

void NiippController::set_antenna_type(int value)
{
	if(value == 1) {
		m_controlModel->setMode(m_controlView->getModeIndex());
	}
	m_controlModel->setAntennaType(value);

	m_controlView->setAntennaType(value, getModeCurrentIndex());
}

int NiippController::get_id()
{
	return m_controlModel->getId();
}

void NiippController::set_point(QPointF coord)
{
	m_controlModel->setPoint(coord);

	QString lat_s = QString::number(coord.x(), 'f', 4);
	m_controlView->setLatText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	m_controlView->setLonText(lon_s);
}

void NiippController::send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing)
{
	if(!m_controlView->getStartState())
	{
		return;
	}

	m_controlModel->setAntenaIndex(m_controlView->getAntenaIndex());
	m_controlModel->setSBpowerValue(m_controlView->getSbPowerValue());

	m_controlModel->sendEvil(point, point_uvoda, alt, bearing);

}

void NiippController::set_angle(double angle)
{
	emit angleChanged(angle);
}

QByteArray NiippController::encode(QStringList list)
{
	return m_controlModel->encode(list);
}

void NiippController::changeMode(int value)
{
	m_controlModel->changeMode(value);
}

int NiippController::getAntenaType()
{
	return m_controlModel->getAntenaType();
}

double NiippController::getRadiusCircle()
{
	return m_controlModel->getRadiusCircle();
}

double NiippController::getRadiusSector()
{
	return m_controlModel->getRadiusSector();
}

Niipp::WorkMode NiippController::getModeCurrentIndex()
{
	return m_controlModel->getModeCurrentIndex();
}

QWidget* NiippController::getControlWidget()
{
	return m_controlView;
}
