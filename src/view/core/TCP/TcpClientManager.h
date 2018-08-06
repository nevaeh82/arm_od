#pragma once

#include <QObject>
#include <QThread>
#include <QMap>
#include <QSettings>

#include <Logger/Logger.h>

#include "Tabs/DbBla/Defines.h"

#include "TcpPvoClient.h"
#include "TcpPvoServer.h"

#include "Interfaces/Tcp/ITcpManager.h"
#include "Interfaces/Tcp/ITcpServer.h"

class TcpClientManager : public QObject
{
	Q_OBJECT

public:
    explicit TcpClientManager(QObject* parent = NULL);
    virtual ~TcpClientManager();

public:
    TcpPvoClient* pvoClient() const;
    TcpPvoServer* pvoServer() const;

    void startClient();
    void startServer();

    void sendEnemyIntersectToServer(UavInfo uav, double azimuth, double distance, QString name);
    void sendEnemyIntersectToClient(UavInfo uav, double azimuth, double distance, QString name);

    void sendEnemyToServer(UavInfo uav);
    void sendEnemyToClient(UavInfo uav);
    void sendEnemyToServer2(UavInfo uav);

private:
    TcpPvoClient* m_tcpClient;
    TcpPvoServer* m_tcpServer;
    TcpPvoServer* m_tcpServer2;

    QString m_clientHost;
    int m_clientPort;
    int m_serverPort;

    void addTcpDevice(QObject *tcpDevice);

    void restartClient(QString host, int port);
    void restartServer(int port);

public slots:
    void reconnectAll(QString clientHost, int clientPort, int serverPort);
private slots:
    void reconnectAllInternal(QString clientHost, int clientPort, int serverPort);

    void restartClientInternal(QString host, int port);
    void restartServerInternal(int port, int port2);
signals:
    void signalReconnectAll(QString, int, int);
    void signalRestartServer(int);
    void signalRestartClient(QString, int);
};
