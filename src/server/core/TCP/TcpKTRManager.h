#ifndef TCPKTRMANAGER_H
#define TCPKTRMANAGER_H


#include <QObject>
#include <QMap>
#include <QTimer>
#include <QSignalMapper>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <Interfaces/Tcp/ITcpListener.h>

#include "Interfaces/ITcpKTRManager.h"
#include "TcpKTRController.h"

#include "QApplication"

/// 5 minutes = 300000 msec
#define MAX_LIFE_TIME			100000
#define KTR_DEVICE_AUTOPILOT	1
#define KTR_DEVICE_KTRGA622		622
#define KTR_DEVICE_KTRGA623		623
#define KTR_BOARD_BROADCAST		65535

class TcpKTRManager : public QObject, public ITcpKTRManager
{
	Q_OBJECT

private:
	ITcpListener* m_tcpManagerAsListener;

	QMap<QString, BaseTcpDeviceController*> m_connectionMap;
	QMap<QString, QTimer*> m_lifeTimerMap;

	QSignalMapper* m_timeoutSignalMapper;

	QMutex m_deleteMutex;

	bool m_needToUpdateAfterDisconnect;

	QSet<QString> m_hostPortForUpdate;

public:
	explicit TcpKTRManager(ITcpListener* tcpManager, QObject* parent = NULL);
	virtual ~TcpKTRManager();

	// ITcpKTRManager interface
public:
	virtual void connectToBoard(const QString& hostPort, const quint16& board, const quint32& device);
	virtual void needToUpdateAfterDisconnect(const bool value, const QString &hostPost);
//	void disconnectFromBoard(const quint16& board);

private slots:
	void timeoutSlot(const QString& key);



};

#endif // TCPKTRMANAGER_H
