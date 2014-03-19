#include "TcpKTRController.h"

TcpKTRController::TcpKTRController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = KTP_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
}

TcpKTRController::TcpKTRController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
}

TcpKTRController::~TcpKTRController()
{
}

void TcpKTRController::createTcpDeviceCoder()
{
	emit createTcpKTRCoderInternalSignal();
}

void TcpKTRController::createTcpKTRCoderInternalSlot()
{
	debug("Creating TcpKTRCoder...");
	m_tcpDeviceCoder = new TcpKTRCoder(this);
}
