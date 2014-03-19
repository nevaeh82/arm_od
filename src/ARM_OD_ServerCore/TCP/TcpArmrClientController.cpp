#include "TcpArmrClientController.h"

TcpArmrClientController::TcpArmrClientController(QObject *parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = ARMR_TCP_CLIENT;
	debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpArmrCoderInternalSignal()), this, SLOT(createTcpArmrCoderInternalSlot()));
}

TcpArmrClientController::TcpArmrClientController(const QString &tcpDeviceName, QObject *parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	connect(this, SIGNAL(createTcpArmrCoderInternalSignal()), this, SLOT(createTcpArmrCoderInternalSlot()));
}

TcpArmrClientController::~TcpArmrClientController()
{
}

void TcpArmrClientController::createTcpDeviceCoder()
{
	emit createTcpArmrCoderInternalSignal();
}

QObject *TcpArmrClientController::asQObject()
{
	return this;
}

void TcpArmrClientController::createTcpArmrCoderInternalSlot()
{
	debug("Creating TcpArmrCoder...");
	m_tcpDeviceCoder = new TcpArmrClientCoder(this);
}
