#include "Tabs/DbBla/Defines.h"
#include "NIIPPController.h"

#include <Logger/Logger.h>

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

void NiippController::slotStopClicked()
{
	m_model->stopCommad();
}

void NiippController::slotStartClicked()
{
	m_model->startCommand();
}

void NiippController::slotOpenRDP()
{
	int ret = openRDP(m_model->getId());
	log_debug(QString("Open RDP for NIIPP = %1").arg(QString::number(ret)));

}

int NiippController::openRDP(int id)
{
	QProcess process;
//    process.setReadChannelMode(ForwardedChannels);
	switch(id)
	{
	case 100:
		process.start("mstsc RDP/spip_dd1.rdp");
		break;
	case 101:
		process.start("mstsc RDP/spip_dd2.rdp");
		break;
	default:
		return -1;
		break;
	}

	if (!process.waitForFinished(-1))
		return -2;
	return process.exitStatus() == QProcess::NormalExit ? process.exitCode() : -1;
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
	m_model->setSBpowerValue(value);
	m_model->changeValuePower(0, value );
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

	m_model->setAntenaIndex(value);

	m_model->setAntennaType(0, value );
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

void NiippController::onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole,
									   const QVector<QPointF>& tail,
									   const QVector<QPointF>& tailStdDev)
{
	Q_UNUSED( tail );
	Q_UNUSED( tailStdDev );

	if ( uavRole != ENEMY_UAV_ROLE ) return;
	if ( uavInfo.source != UAV_SOLVER_MANUAL_SOURCE &&
		 uavInfo.source != UAV_SOLVER_AUTO_SOURCE) return;

	/// \todo: I don't know why numbers are 100 and 101. It's from MapClient1
	if( getId() == 100 || getId() == 101 ) {
		QPointF point( uavInfo.lon, uavInfo.lat );
		sendEnemyBpla(point, m_model->getPoint(), uavInfo.alt, uavInfo.yaw );
	}
}

void NiippController::onMethodCalled(const QString &method, const QVariant &argument)
{
	QByteArray data = argument.toByteArray();

	if( method == RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS ) {
		int status;
		int id;
		QDataStream dataStream(&data, QIODevice::ReadOnly);
		dataStream >> id;
		dataStream >> status;
		if(m_model->getId() != id)
			return;
		m_view->setStatusConnection(status);
	}

	if( method == RPC_SLOT_SERVER_SEND_NIIPP_DATA ) {
		QDataStream ds(&data, QIODevice::ReadOnly);

		log_info("IN NIIPP CONTROLLER!");

		int id;
		QDateTime dt;
		QTime time;
		int mode;
		QPointF point;
		QString NS;
		QString EW;
		int alt;
		int zone;
		int course;
		int angle;

		ds >> id;
		ds >> dt;
		ds >> time;
		ds >> mode;
		ds >> point;
		ds >> NS;
		ds >> EW;
		ds >> alt;
		ds >> zone;
		ds >> course;
		ds >> angle;

		if(m_model->getId() != id)
			return;

		QPointF latlon;
		switch( id ) {
			case 100:
				latlon.setX(42.511183);
				latlon.setY(41.6905);
				break;

			case 101:
				latlon.setX(42.634183);
				latlon.setY(41.912167);
				break;
		}

//        Niipp niipp( id, QString::number( id ), latlon, NULL );

		m_model->setAntennaType(1, mode == 1 ? 1 : 0 );
		m_model->changeValuePower(1, zone );
		log_info(QString("IN NIIPP CONTROLLER SET ANGLE = %1!").arg(course));
		m_model->setAngel(course);
		m_view->setStatusText(mode);


		m_mapController->updateNiippPowerZone( *m_model );

		/// TODO: recheck following. WTF?!
		//		QByteArray ba1;
		//		QDataStream ds2(&ba1, QIODevice::WriteOnly);
		//		ds2 << mode;
		//		m_tabManager->send_data_niipp_control(id, ba1);
		}

}

void NiippController::appendView(NiippWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(complexEnabled(bool)), this, SLOT(enableComplex(bool)));
	connect(m_view, SIGNAL(valuePowerChanged(int)), this, SLOT(changeValuePower(int)));
	connect(m_view, SIGNAL(stopClicked()), this, SLOT(slotStopClicked()));
	connect(m_view, SIGNAL(startClicked()), this, SLOT(slotStartClicked()));
	connect(m_view, SIGNAL(antennaTypeChanged(int)), this, SLOT(setAntennaType(int)));
	connect(m_view, SIGNAL(modeChanged(int)), this, SLOT(changeMode(int)));
	connect(m_view, SIGNAL(cleared()), this, SLOT(clear()));
	connect(m_view, SIGNAL(signalOpenRDP()), this, SLOT(slotOpenRDP()));
}

void NiippController::onGetConnectionStatus()
{
	m_model->getStatusConnection();
}
