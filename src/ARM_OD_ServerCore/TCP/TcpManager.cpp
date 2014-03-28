#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_rpcServer = NULL;

	connect(this, SIGNAL(onMethodCalledInternalSignal(QString,QVariant)), this, SLOT(onMethodCalledInternalSlot(QString,QVariant)));
}

TcpManager::~TcpManager()
{
	emit threadTerminateSignal();
}

void TcpManager::addTcpDevice(const QString& deviceName, const quint32& deviceType)
{
	debug(QString("Creating %1 with type %2").arg(QString(deviceName)).arg(QString::number(deviceType)));

	BaseTcpDeviceController* controller = NULL;

	switch(deviceType)
	{
		case DeviceTypes::NIIPP_TCP_DEVICE:
			controller = new TcpNIIPPController(deviceName);
			debug(QString("Created TcpNIIPPController"));
			break;
		case DeviceTypes::KTR_TCP_DEVICE:
			controller = new TcpKTRController(deviceName);
			debug(QString("Created TcpKTRController"));
			break;
		case DeviceTypes::AIS_TCP_DEVICE:
			controller = new TcpAISController(deviceName);
			debug(QString("Created TcpAISController"));
			break;
		default:
			break;
	}

	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		debug(QString("Unable to create %1 with type %2").arg(QString(deviceName)).arg(QString::number(deviceType)));
		return;
	}

	controller->registerReceiver(this);

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	m_controllersMap.insert(deviceName, controller);

	controller->createTcpDeviceCoder();
	controller->createTcpClient();
	controller->connectToHost();

	if (deviceType == DeviceTypes::KTR_TCP_DEVICE) {
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_GET_BOARD_LIST, QByteArray())));
	}

	debug(QString("Added device connection for %1 with %2").arg(deviceName).arg(deviceType));
}

void TcpManager::removeTcpDevice(const QString& deviceName)
{
	if (!m_controllersMap.contains(deviceName)) {
		debug(QString("Map doesn't contain %1").arg(deviceName));
		return;
	}

	BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);
	if (controller == NULL) {
		return;
	}

	controller->deregisterReceiver(this);
	controller->disconnectFromHost();
	m_controllersMap.remove(deviceName);
}

void TcpManager::setRpcServer(IRPC* rpcServer)
{
	m_rpcServer = rpcServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}

void TcpManager::onMessageReceived(const quint32 deviceType, const QString& deviceName, const MessageSP argument)
{
	/// TODO : refactor it. It's bad.

	QString messageType = argument->type();
	QByteArray messageData = argument->data();

	switch (deviceType) {
		case DeviceTypes::NIIPP_TCP_DEVICE:
			if (messageType == TCP_NIIPP_ANSWER) {
//				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_NIIPP_DATA, messageData);
			}
			break;
		case DeviceTypes::KTR_TCP_DEVICE:
			if (messageType == TCP_KTR_ANSWER_BOARD_LIST) {

				if (!m_controllersMap.contains(deviceName)) {
					debug(QString("Map doesn't contain %1 %2").arg(deviceName).arg(deviceType));
					return;
				}

				BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);
				if (controller == NULL) {
					debug(QString("Something wrong with controller %1 %2").arg(deviceName).arg(deviceType));
					return;
				}

				QList<quint16> boardList;
				QDataStream inputDataStream(&messageData, QIODevice::ReadOnly);
				inputDataStream >> boardList;

				foreach (quint16 boardID, boardList) {
					{
						quint32 dev = 1;
						QByteArray dataToSend;
						QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
						dataStream << boardID << dev;
						controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));
					}
					{
						quint32 dev = 622;
						QByteArray dataToSend;
						QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
						dataStream << boardID << dev;
						controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));
					}
				}
			} else if (messageType == TCP_KTR_ANSWER_BPLA){
//				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BLA_POINTS, messageData);
			}
			break;
		case DeviceTypes::AIS_TCP_DEVICE:
			if (messageType == TCP_AIS_ANSWER_DATA) {
//				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_AIS_DATA, messageData);
			}
			break;
		default:
			break;
	}
}

void TcpManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	emit onMethodCalledInternalSignal(method,argument);
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
}

void TcpManager::setTcpServer(ITcpServer* tcpServer)
{
}
