#include "TcpKTRManager.h"

TcpKTRManager::TcpKTRManager(QObject *parent) :
	QObject(parent)
{
	m_timeoutSignalMapper = new QSignalMapper(this);

	connect(m_timeoutSignalMapper, SIGNAL(mapped(const QString &)),
			this, SLOT(timeoutSlot(const QString &)));
}

TcpKTRManager::~TcpKTRManager()
{
}

void TcpKTRManager::connectToBoard(const quint16& board, const quint32& device)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	QString key = QString::number(board) +":" + QString::number(device);

	BaseTcpDeviceController* controller = NULL;
	QTimer* lifeTimer = NULL;

	controller = m_connectionMap.value(key, NULL);
	lifeTimer = m_lifeTimerMap.value(key, NULL);

	if (controller == NULL && lifeTimer == NULL) {
		controller = new TcpKTRController();
		m_connectionMap.insert(key, controller);

		lifeTimer = new QTimer();
		connect(lifeTimer, SIGNAL(timeout()), m_timeoutSignalMapper, SLOT(map()));
		m_timeoutSignalMapper->setMapping(lifeTimer, key);
		//connect(lifeTimer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));

		m_lifeTimerMap.insert(key, lifeTimer);

		QByteArray dataToSend;
		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << board << device;
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_KTR_REQUEST_COMMAND_TO_BPLA, dataToSend)));
	}

	//Need to update lifetimer
	lifeTimer->start(MAX_LIFE_TIME);
}

void TcpKTRManager::timeoutSlot(const QString& key)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	BaseTcpDeviceController* controller = m_connectionMap.take(key);
	QTimer* lifeTimer = m_lifeTimerMap.take(key);

	if (controller != NULL){
		delete controller;
	}

	if (lifeTimer != NULL){
		delete lifeTimer;
	}
}
