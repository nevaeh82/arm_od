#include "TcpKTRController.h"

TcpKTRController::TcpKTRController(QObject *parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpKTRController)), parent)
{
	m_tcpDeviceName = NIIPP_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
}

TcpKTRController::TcpKTRController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpKTRController)), parent)
{
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
}

TcpKTRController::~TcpKTRController()
{
}

void TcpKTRController::createTcpDeviceCoder()
{

}

QObject* TcpKTRController::asQObject()
{
	return this;
}

void TcpKTRController::createTcpKTRCoderInternalSlot()
{

}
