#include "NIIPPController.h"

#include <QDebug>

NiippController::NiippController(int id, QString name, QPointF latlon, MapController* map_controller, ITabManager* parent_tab,  QObject* parent)
	: QObject(parent),
	 m_controlModel( new Niipp(id, name, latlon, map_controller, parent_tab) )
{
	m_view = NULL;

	connect(this, SIGNAL(angleChanged(double)), this, SLOT(changeAngel(double)));
	map_controller->get_map_client(1)->set_niipp_controller(this);
	m_controlModel->create();


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
	m_view->changeValuePower( value, m_controlModel->getRadiusCircle()
									, m_controlModel->getRadiusSector(), m_controlModel->getAntenaType() );
}

void NiippController::changeAngel(double value)
{
	m_controlModel->setAngel(value);
}

void NiippController::set_power(double value)
{
	Q_UNUSED(value);
}

void NiippController::set_switch_on(bool state)
{
	Q_UNUSED(state);
}

void NiippController::set_antenna_type(int value)
{
	if(value == 1) {
		m_controlModel->setMode(m_view->getModeIndex());
	}
	m_controlModel->setAntennaType(value);

	m_view->setAntennaType(value, getModeCurrentIndex());
}

int NiippController::get_id()
{
	return m_controlModel->getId();
}

void NiippController::set_point(QPointF coord)
{
	m_controlModel->setPoint(coord);

	QString lat_s = QString::number(coord.x(), 'f', 4);
	m_view->setLatText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	m_view->setLonText(lon_s);
}

void NiippController::send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing)
{
	if(!m_view->getStartState())
	{
		return;
	}

	m_controlModel->setAntenaIndex(m_view->getAntenaIndex());
	m_controlModel->setSBpowerValue(m_view->getSbPowerValue());

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

void NiippController::appendView(NiippWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(complexEnabled(bool)), this, SLOT(enableComplex(bool)));
	connect(m_view, SIGNAL(valuePowerChanged(int)), this, SLOT(changeValuePower(int)));
	connect(m_view, SIGNAL(stopClicked()), this, SLOT(stopClicked()));
	connect(m_view, SIGNAL(antennaTypeChanged(int)), this, SLOT(set_antenna_type(int)));
	connect(m_view, SIGNAL(modeChanged(int)), this, SLOT(changeMode(int)));
	connect(m_view, SIGNAL(cleared()), this, SLOT(clear()));
}
