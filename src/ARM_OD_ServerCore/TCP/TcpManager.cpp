#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_rpcServer = NULL;

	connect(this, SIGNAL(onMethodCalledInternalSignal(QString,QVariant)), this, SLOT(onMethodCalledInternalSlot(QString,QVariant)));

	m_ktrManager = new TcpKTRManager(this, this);
}

TcpManager::~TcpManager()
{
	emit threadTerminateSignal();
}

void TcpManager::addTcpDevice(const QString& deviceName, const quint32& deviceType)
{
	log_debug(QString("Creating %1 with type %2").arg(QString(deviceName)).arg(QString::number(deviceType)));

	BaseTcpDeviceController* controller = NULL;

	/// TODO: recheck following
//	/// Recheck map on existing device
//	controller = m_controllersMap.value(uavTcpDeviceName, NULL);
//	if (controller != NULL) {
//		debug(QString("Controller %1 %2 already exists").arg(deviceName).arg(deviceType));
//		return;
//	}

	switch(deviceType)
	{
		case DeviceTypes::NIIPP_TCP_DEVICE:
			controller = new TcpNIIPPController(deviceName);
			log_debug(QString("Created TcpNIIPPController"));
			break;
		case DeviceTypes::KTR_TCP_DEVICE:
			controller = new TcpKTRController(deviceName);
			log_debug(QString("Created TcpKTRController"));
			break;
		case DeviceTypes::AIS_TCP_DEVICE:
			controller = new TcpAISController(deviceName);
			log_debug(QString("Created TcpAISController"));
			break;
		case DeviceTypes::ARMR_TCP_CLIENT:
			controller = new TcpArmrClientController(deviceName);
			log_debug(QString("Created TcpArmrClientController"));
			break;
		default:
			break;
	}


	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		log_debug(QString("Unable to create %1 with type %2").arg(QString(deviceName)).arg(QString::number(deviceType)));
		return;
	}

	controller->createTcpClient();
	controller->createTcpDeviceCoder();

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	m_controllersMap.insert(deviceName, controller);

	controller->registerReceiver(this);

	controller->connectToHost();

//	if (deviceType == DeviceTypes::KTR_TCP_DEVICE) {
//		controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_GET_BOARD_LIST, QByteArray())));
//	}

	log_debug(QString("Added device connection for %1 with %2").arg(deviceName).arg(deviceType));
}

void TcpManager::removeTcpDevice(const QString& deviceName)
{
	if (!m_controllersMap.contains(deviceName)) {
		log_debug(QString("Map doesn't contain %1").arg(deviceName));
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

void TcpManager::setTcpServer(ITcpServer *tcpServer)
{
	m_tcpServer = tcpServer;
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
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_NIIPP_DATA, messageData);
			}
			break;
		case DeviceTypes::KTR_TCP_DEVICE:
			if (messageType == TCP_KTR_ANSWER_CONNECTION_STATUS) {

				if (deviceType == DeviceTypes::KTR_TCP_DEVICE) {

					BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);

					if (controller == NULL) {
						log_debug(QString("Something wrong with controller %1 %2").arg(deviceName).arg(deviceType));
						return;
					}

					controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_GET_BOARD_LIST, QByteArray())));
				}

			} else if (messageType == TCP_KTR_ANSWER_BOARD_LIST) {

				if (!m_controllersMap.contains(deviceName)) {
					log_debug(QString("Map doesn't contain %1 %2").arg(deviceName).arg(deviceType));
					return;
				}

				BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);
				if (controller == NULL) {
					log_debug(QString("Something wrong with controller %1 %2").arg(deviceName).arg(deviceType));
					return;
				}

				QList<quint16> boardList;
				QDataStream inputDataStream(&messageData, QIODevice::ReadOnly);
				inputDataStream >> boardList;

				foreach (quint16 boardID, boardList) {
					m_ktrManager->connectToBoard(controller->getHost(), boardID, KTR_DEVICE_AUTOPILOT);
					m_ktrManager->connectToBoard(controller->getHost(), boardID, KTR_DEVICE_KTRGA622);
				}
				/// To future:
//				m_ktrManager->connectToBoard(controller->getHost(), KTR_BOARD_BROADCAST, KTR_DEVICE_KTRGA622);
//				m_ktrManager->connectToBoard(controller->getHost(), KTR_BOARD_BROADCAST, KTR_DEVICE_KTRGA623);

			} else if (messageType == TCP_KTR_ANSWER_BPLA){

				QVector<UAVPositionData> positionDataVector;
				QDataStream inputDataStream(&messageData, QIODevice::ReadOnly);
				inputDataStream >> positionDataVector;

				for (quint64 i = 0; i < positionDataVector.size(); ++i) {
					UAVPositionData positionData = positionDataVector.at(i);
					/**
					 * deviceNameInfo:
					 *		host
					 *		port
					 *		boardID
					 *		device
					 */
					QStringList deviceNameInfo = deviceName.split(":");
					positionData.boardID = deviceNameInfo.at(2).toUShort();
					positionData.device = deviceNameInfo.at(3).toUInt();
					positionDataVector.replace(i, positionData);
				}

				QByteArray dataToSend;
				QDataStream outputDataStream(&dataToSend, QIODevice::WriteOnly);
				outputDataStream << positionDataVector;

				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BLA_POINTS, dataToSend);
			}
			break;
		case DeviceTypes::AIS_TCP_DEVICE:
			if (messageType == TCP_AIS_ANSWER_DATA) {
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_AIS_DATA, messageData);
			}
			break;
		case DeviceTypes::ARMR_TCP_CLIENT:
			if (messageType == QString(ARM_R_SERVER_ATLANT_DIRECTION)) {
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, messageData);
			} else if (messageType == QString(ARM_R_SERVER_ATLANT_POSITION)){
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_ATLANT_POSITION, messageData);
			} else if (messageType == QString(ARM_R_SERVER_BPLA_COORDS)){
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_POINTS, messageData);
			} else if (messageType == QString(ARM_R_SERVER_BPLA_COORDS_AUTO)){
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, messageData);
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

QString TcpManager::getTcpClientName()
{
	return ARMR_TCP_CLIENT_NAME;
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
	if (method == RPC_SLOT_SET_SOLVER_CLEAR) {
		BaseTcpDeviceController* controller = m_controllersMap.value(getTcpClientName(), NULL);
		if (controller == NULL) {
			return;
		}
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_ARMR_SEND_SOLVER_CLEAR, argument.toByteArray())));

	} else if (method == RPC_SLOT_SET_SOLVER_DATA) {
		BaseTcpDeviceController* controller = m_controllersMap.value(getTcpClientName(), NULL);
		if (controller == NULL) {
			return;
		}
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_ARMR_SEND_SOLVER_DATA, argument.toByteArray())));
	}
}
