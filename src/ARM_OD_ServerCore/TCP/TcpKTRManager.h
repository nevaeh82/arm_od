#ifndef TCPKTRMANAGER_H
#define TCPKTRMANAGER_H

#include "Interfaces/ITcpKTRManager.h"

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QSignalMapper>
#include <QMutex>
#include <QMutexLocker>

#include "TcpKTRController.h"

/// 5 minutes = 300000 msec
#define MAX_LIFE_TIME			300000
#define KTR_DEVICE_AUTOPILOT	1
#define KTR_DEVICE_KTRGA622		622
#define KTR_DEVICE_KTRGA623		623
#define KTR_BOARD_BROADCAST		65535

class TcpKTRManager : public QObject, public ITcpKTRManager
{
	Q_OBJECT

private:
	QMap<QString, BaseTcpDeviceController*> m_connectionMap;
	QMap<QString, QTimer*> m_lifeTimerMap;

	QSignalMapper* m_timeoutSignalMapper;

	QMutex m_deleteMutex;
public:
	explicit TcpKTRManager(QObject* parent = NULL);
	virtual ~TcpKTRManager();

	void connectToBoard(const quint16& board, const quint32& device);
//	void disconnectFromBoard(const quint16& board);

private slots:
	void timeoutSlot(const QString& key);

};

#endif // TCPKTRMANAGER_H
