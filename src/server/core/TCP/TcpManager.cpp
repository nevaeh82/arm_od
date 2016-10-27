#include "TcpManager.h"

#include "GPS.pb.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_rpcServer = NULL;
	m_tcpServer = NULL;

	connect(this, SIGNAL(onMethodCalledInternalSignal(QString,QVariant)), this, SLOT(onMethodCalledInternalSlot(QString,QVariant)));

	m_ktrManager = new TcpKTRManager(this, this);
}

TcpManager::~TcpManager()
{
	emit threadTerminateSignal();
}

void TcpManager::setRpcClient(RpcClientRWrapper *rpcClient)
{
	m_rpcClient = rpcClient;
}

void TcpManager::addTcpDevice(const QString& deviceName, const quint32& deviceType)
{
	log_debug(QString("Creating %1 with type %2").arg(QString(deviceName)).arg(QString::number(deviceType)));

	if(deviceName == "ADSBClient") {
		createAdsbDevice();
		return;
	}

	if(deviceName == "wwwADSBClient") {
		createwwwAdsbDevice();
		return;
	}

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

            connect( (TcpKTRController*)controller, SIGNAL(onGpsData(QByteArray)),
                     this, SLOT(slotGpsData(QByteArray data)) );

			log_debug(QString("Created TcpKTRController"));
            //return;
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
			else if(messageType == TCP_NIIPP_ANSWER_CONNECTION_STATUS) {
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS, messageData);
			}
			break;
		case DeviceTypes::KTR_TCP_DEVICE:
			if (messageType == TCP_KTR_ANSWER_CONNECTION_STATUS) {

				if (deviceType == DeviceTypes::KTR_TCP_DEVICE) {

					BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);

					int connectionState;
					QDataStream dataStream(&messageData, QIODevice::ReadOnly);
					dataStream >> connectionState;

                    if (controller == NULL) {

                        log_debug(QString("%1 %2 State: %3").arg(deviceName).arg(deviceType).arg(connectionState));

//                        log_debug(QString("Something wrong with controller %1 %2").arg(deviceName).arg(deviceType));
                        if(m_ktrManager) {
                        m_ktrManager->needToUpdateAfterDisconnect(true, "");
                        }
                        return;
                    }

					if (connectionState == TCP::Disconnected) {
						/// WTF?
						//m_ktrManager->needToUpdateAfterDisconnect(true);
						log_debug("disconnect happen. need to update when connect will happen...");
					} else if (connectionState == TCP::Connected) {
						m_ktrManager->needToUpdateAfterDisconnect(true, controller->getHost());
						log_debug("requesting board list...");
						controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_GET_BOARD_LIST, QByteArray())));
					}

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

				m_ktrManager->needToUpdateAfterDisconnect(false, controller->getHost());
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
			} else if (messageType == QString(ARM_R_SERVER_BPLA_COORDS_1)){
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_POINTS_1, messageData);
			}
			else if (messageType == QString(ARM_R_SERVER_BPLA_COORDS_AUTO)){
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, messageData);
			} else if (messageType == QString(ARM_R_SERVER_HYPERBOLA)) {
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_HYPERBOLA, messageData);
			}
			break;
		case DeviceTypes::ADSB_TCP_CLIENT:
			if (messageType == TCP_ADSB_ANSWER_DATA) {
				m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_ADSB_DATA, messageData);
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


//From RPC
void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
	if (method == RPC_SLOT_SET_SOLVER_CLEAR) {
		//TODO: REMOVE RPCCLIENT USAGE WHEN TCP CLIENT PROTOBUF WILL BE TESTED

		/*BaseTcpDeviceController* controller = m_controllersMap.value(getTcpClientName(), NULL);
		if (controller == NULL) {
			return;
		}
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_ARMR_SEND_SOLVER_CLEAR, argument.toByteArray())));*/

		m_rpcClient->sendDataByRpc(TCP_ARMR_SEND_SOLVER_CLEAR, argument.toByteArray());

	} else if (method == RPC_SLOT_SET_SOLVER_DATA) {
		//TODO: REMOVE RPCCLIENT USAGE WHEN TCP CLIENT PROTOBUF WILL BE TESTED

		/*BaseTcpDeviceController* controller = m_controllersMap.value(getTcpClientName(), NULL);
		if (controller == NULL) {
			return;
		}
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_ARMR_SEND_SOLVER_DATA, argument.toByteArray())));*/

		m_rpcClient->sendDataByRpc(TCP_ARMR_SEND_SOLVER_DATA, argument.toByteArray());
	} else if (method == RPC_SLOT_SET_NIIPP_BPLA) {
			//TODO: REMOVE RPCCLIENT USAGE WHEN TCP CLIENT PROTOBUF WILL BE TESTED

//			QString name;
//			QDataStream ds(&argument.toByteArray(), QIODevice::ReadOnly);

//			int id = -1;
//			ds >> id;

//			switch (id) {
//				case 100:
//					name = "NIIPP_1";
//					break;
//				case 101:
//					name = "NIIPP_2";
//					break;

//				default:
//					log_info( QString( "Received unknown NIIPP ID from solver: %1" ).arg( id ) );
//					return;
//			}

//			BaseTcpDeviceController* controller = m_controllersMap.value(name, NULL);
//			if (controller == NULL) {
//				return;
//			}
//			controller->sendData(MessageSP(new Message<QByteArray>(TCP_NIIPP_REQUEST_PBLA, argument.toByteArray())));

        foreach (BaseTcpDeviceController* cnt, m_controllersMap.values()) {
            cnt->sendData(MessageSP(new Message<QByteArray>(TCP_NIIPP_REQUEST_PBLA, argument.toByteArray())));
        }

	}
	else if (method == RPC_SLOT_GET_NIIPP_CONNECTION_STATUS) {
		QString name;
		QDataStream ds(&argument.toByteArray(), QIODevice::ReadOnly);

		int id = -1;
		ds >> id;

		switch (id) {
			case 100:
				name = "NIIPP_1";
				break;

			case 101:
				name = "NIIPP_2";
				break;

			default:
				log_info( QString( "Received unknown NIIPP ID from solver: %1" ).arg( id ) );
				return;
		}

		BaseTcpDeviceController* controller = m_controllersMap.value(name, NULL);
		if (controller == NULL) {
			return;
		}
		controller->isConnected();
	}
	else if(method == RPC_SLOT_SET_ADSB_ENABLE) {
		QDataStream ds(&argument.toByteArray(), QIODevice::ReadOnly);

		bool enable = true;
		quint8 type;

		ds >> type;
		ds >> enable;

		if( m_adsbController.value(ADSBType(type), NULL) ) {
			m_adsbController.value(ADSBType(type))->enableAdsb(enable);
		}
	}
}

void TcpManager::createAdsbDevice()
{
	TcpADSBController* adsbController = new TcpADSBController("ADSBClient");

	QThread* controllerThread = new QThread;
	connect(adsbController->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), adsbController, SLOT(onStop()));
	connect(this, SIGNAL(threadTerminateSignal()), adsbController->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));

	connect(controllerThread, SIGNAL(started()), adsbController, SLOT(onInitPlanesClient()));

	adsbController->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	adsbController->registerReceiver(this);

	log_debug(QString("Added device connection for %1").arg("ADSBClient"));

	m_adsbController.insert(hwAdsb, adsbController);
}

void TcpManager::createwwwAdsbDevice()
{
	TcpFlyRadarController* adsbController = new TcpFlyRadarController("ADSBClient");

	QThread* controllerThread = new QThread;
	connect(adsbController->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), adsbController, SLOT(onStop()));
	connect(this, SIGNAL(threadTerminateSignal()), adsbController->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));

	connect(controllerThread, SIGNAL(started()), adsbController, SLOT(onInit()));

	adsbController->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	//adsbController->onInit();

	adsbController->registerReceiver(this);

	log_debug(QString("Added device connection for %1").arg("ADSBClient"));

    m_adsbController.insert(onlineAdsb, adsbController);
}

void TcpManager::slotGpsData(QByteArray data)
{
    KTP_3_Proto::GPSPacket pkt;

    if (!pkt.ParseFromArray(data.data(), data.size())) {
        log_info("Failed parse KTRDataPacket-GPS");
    }


    QVector<UAVPositionData> positionDataVector;

    UAVPositionData uav;
    uav.altitude = pkt.height();
    uav.latitude = pkt.latitude();
    uav.longitude = pkt.longitude();
    uav.course = pkt.course();
    uav.boardID = pkt.id();
    uav.state = 1;
    uav.device = 1;
    uav.name = QString::number(pkt.id());

    QByteArray dataToSend;
    QDataStream outputDataStream(&dataToSend, QIODevice::WriteOnly);
    outputDataStream << positionDataVector;


    m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BLA_POINTS, dataToSend);

}
