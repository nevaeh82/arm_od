#include "TcpNIIPPController.h"

TcpNIIPPController::TcpNIIPPController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = NIIPP_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpNIIPPCoderInternalSignal()), this, SLOT(createTcpNIIPPCoderInternalSlot()));
}

TcpNIIPPController::TcpNIIPPController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	connect(this, SIGNAL(createTcpNIIPPCoderInternalSignal()), this, SLOT(createTcpNIIPPCoderInternalSlot()));
}

TcpNIIPPController::~TcpNIIPPController()
{
}

void TcpNIIPPController::createTcpDeviceCoder()
{
	emit createTcpNIIPPCoderInternalSignal();
}

void TcpNIIPPController::createTcpNIIPPCoderInternalSlot()
{
	debug("Creating TcpNIIPPCoder...");
	m_tcpDeviceCoder = new TcpNIIPPCoder(this);
}
