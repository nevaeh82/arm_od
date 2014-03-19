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

void TcpManager::addTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	debug(QString("Creating %1").arg(deviceType));
	QString key = host + ":" + QString::number(port);

	BaseTcpDeviceController* controller = NULL;

	if (deviceType == NIIPP_TCP_DEVICE) {
		controller = new TcpNIIPPController(NIIPP_TCP_DEVICE);
		controller->registerReceiver(this);
		debug(QString("Created TcpNIIPPController"));
	} else if (deviceType == KTR_TCP_DEVICE) {
		controller = new TcpKTRController(KTR_TCP_DEVICE);
		controller->registerReceiver(this);
		debug(QString("Created TcpKTRController"));
	}

	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		debug(QString("Unable to create %1 with %2").arg(deviceType).arg(key));
		return;
	}

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	m_controllersMap.insert(deviceType, controller);

	controller->createTcpDeviceCoder();
	controller->createTcpClient();
	controller->connectToHost(host, port);

	if (deviceType == KTR_TCP_DEVICE) {
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_GET_BOARD_LIST, QByteArray())));
	}

	debug(QString("Added device connection for %1 with %2").arg(deviceType).arg(key));
}

void TcpManager::removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	if (!m_controllersMap.contains(deviceType)) {
		debug(QString("Map doesn't contain %1").arg(deviceType));
		return;
	}

	BaseTcpDeviceController* controller = m_controllersMap.value(deviceType, NULL);
	if (controller == NULL) {
		return;
	}

	controller->deregisterReceiver(this);
	controller->disconnectFromHost();
	m_controllersMap.remove(deviceType);
}

void TcpManager::setRpcServer(IRPC* rpcServer)
{
	m_rpcServer = rpcServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}

void TcpManager::onMessageReceived(const QString& device, const MessageSP argument)
{
	/// TODO : refactor it. It's bad.

	QString messageType = argument->type();
	QByteArray messageData = argument->data();

	if (device == NIIPP_TCP_DEVICE) {
		if (messageType == TCP_NIIPP_ANSWER) {
//			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_NIIPP_DATA, messageData);
		}
	} else if (device == KTR_TCP_DEVICE) {
		if (messageType == TCP_KTR_ANSWER_BOARD_LIST) {

			if (!m_controllersMap.contains(KTR_TCP_DEVICE)) {
				debug(QString("Map doesn't contain %1").arg(KTR_TCP_DEVICE));
				return;
			}

			BaseTcpDeviceController* controller = m_controllersMap.value(KTR_TCP_DEVICE, NULL);
			if (controller == NULL) {
				return;
			}

			QList<quint16> boardList;
			QDataStream inputDataStream(&messageData, QIODevice::ReadOnly);
			inputDataStream >> boardList;

			foreach (quint16 boardID, boardList) {
				quint32 dev = 1;

				QByteArray dataToSend;
				QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
				dataStream << boardID << dev;
				controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));

				dataToSend.clear();
				dev = 622;
				dataStream << boardID << dev;
				controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));
			}

		} else if (messageType == TCP_KTR_ANSWER_BPLA){
//			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BLA_POINTS, messageData);

		}
	}
}

void TcpManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	emit onMethodCalledInternalSignal(method,argument);
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
}
