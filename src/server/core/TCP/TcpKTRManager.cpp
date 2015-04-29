#include "TcpKTRManager.h"

TcpKTRManager::TcpKTRManager(ITcpListener* tcpManager, QObject *parent) :
	QObject(parent)
{
	m_tcpManagerAsListener = tcpManager;
	m_timeoutSignalMapper = new QSignalMapper(this);

	m_needToUpdateAfterDisconnect = true; // As first connect

	connect(m_timeoutSignalMapper, SIGNAL(mapped(const QString &)),
			this, SLOT(timeoutSlot(const QString &)));
}

TcpKTRManager::~TcpKTRManager()
{
}

void TcpKTRManager::connectToBoard(const QString& hostPort, const quint16& board, const quint32& device)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	/**
	 * deviceNameInfo:
	 *		host
	 *		port
	 *		boardID
	 *		device
	 */
	QString key = hostPort + ":" + QString::number(board) + ":" + QString::number(device);

	BaseTcpDeviceController* controller = NULL;
	QTimer* lifeTimer = NULL;

	controller = m_connectionMap.value(key, NULL);
	lifeTimer = m_lifeTimerMap.value(key, NULL);

	if (controller == NULL && lifeTimer == NULL) {

		controller = new TcpKTRController(key);

		if (controller == NULL) {
			log_debug(QString("Unable to create %1 with type %2").arg(QString(key)).arg(QString::number(DeviceTypes::KTR_TCP_DEVICE)));
			return;
		}

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		QThread* controllerThread = new QThread;
		connect(controller, SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		connect(this, SIGNAL(threadTerminateSignal()), controller, SLOT(deleteLater()));
		connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		m_connectionMap.insert(key, controller);

		controller->registerReceiver(m_tcpManagerAsListener);



		QStringList strList = hostPort.split(":");
		if (strList.isEmpty()) {
			log_debug("Everything is not ok... HostPort is empty");
		}

		controller->connectToHost(strList.at(0), strList.at(1).toUInt());

		QTime t = QTime::currentTime().addSecs(10);
		while( !controller->isConnected() && QTime::currentTime() < t) {
			qApp->processEvents();
		}


		lifeTimer = new QTimer();
		connect(lifeTimer, SIGNAL(timeout()), m_timeoutSignalMapper, SLOT(map()));
		m_timeoutSignalMapper->setMapping(lifeTimer, key);
		//connect(lifeTimer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));

		m_lifeTimerMap.insert(key, lifeTimer);


	}

	//Need to update lifetimer
	lifeTimer->start(MAX_LIFE_TIME);

	if (m_needToUpdateAfterDisconnect) {

		if (!m_hostPortForUpdate.contains(hostPort)) {
			return;
		}

		// Need to register again
		QByteArray dataToSend;
		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << board << device;
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));
	}
}

void TcpKTRManager::needToUpdateAfterDisconnect(const bool value, const QString& hostPost)
{
	QMutexLocker mutexLocker(&m_deleteMutex);

	if (value) {
		m_hostPortForUpdate.insert(hostPost);
	} else {
		m_hostPortForUpdate.remove(hostPost);
	}

	m_needToUpdateAfterDisconnect = value;
}

void TcpKTRManager::timeoutSlot(const QString& key)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	BaseTcpDeviceController* controller = m_connectionMap.take(key);
	QTimer* lifeTimer = m_lifeTimerMap.take(key);

	if (controller != NULL){
		controller->disconnectFromHost();
		controller->deleteLater();
	}

	if (lifeTimer != NULL){
		lifeTimer->deleteLater();
	}
}


