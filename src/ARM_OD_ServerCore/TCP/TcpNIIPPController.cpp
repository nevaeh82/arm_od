#include "TcpNIIPPController.h"

TcpNIIPPController::TcpNIIPPController(QObject *parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpNIIPPController)), parent)
{
	m_tcpDeviceName = NIIPP_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpNIIPPCoderInternalSignal()), this, SLOT(createTcpNIIPPCoderInternalSlot()));
}

TcpNIIPPController::TcpNIIPPController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpNIIPPController)), parent)
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

QObject* TcpNIIPPController::asQObject()
{
	return this;
}

void TcpNIIPPController::createTcpNIIPPCoderInternalSlot()
{
	m_logger->debug("Creating TcpNIIPPCoder...");
	m_tcpDeviceCoder = new TcpNIIPPCoder(this);
}
