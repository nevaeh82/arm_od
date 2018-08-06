#include <QString>
#include "TcpClientManager.h"

TcpClientManager::TcpClientManager(QObject* parent) :
    QObject(parent)
{
    m_tcpClient = new TcpPvoClient();
    m_tcpServer = new TcpPvoServer();
    m_tcpServer2 = new TcpPvoServer();


    addTcpDevice(m_tcpClient);
    addTcpDevice(m_tcpServer);
    addTcpDevice(m_tcpServer2);

    startClient();
    startServer();

    connect(this, SIGNAL(signalReconnectAll(QString,int,int)),
            this, SLOT(reconnectAllInternal(QString,int,int)), Qt::QueuedConnection);

    connect(this, SIGNAL(signalRestartClient(QString,int)),
            this, SLOT(restartClientInternal(QString,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(signalRestartServer(int)),
            this, SLOT(restartServerInternal(int)), Qt::QueuedConnection);
}

TcpClientManager::~TcpClientManager()
{
}

void TcpClientManager::startClient()
{
    QSettings settings("TCP/tcpPvoSettings.ini", QSettings::IniFormat);
    QString host = settings.value("tcpClientHost", "127.0.0.1").toString();
    qint32 port = settings.value("tcpClientPort", 5555).toUInt();
    m_tcpClient->connectToHostAsync(host, port);
}

void TcpClientManager::startServer()
{
    QSettings settings("TCP/tcpPvoSettings.ini", QSettings::IniFormat);
    quint32 port = settings.value("tcpServerPort", 4444).toUInt();
    m_tcpServer->start(QHostAddress::Any, port);

    quint32 port2 = settings.value("tcpServerPort2", 4443).toUInt();
    m_tcpServer2->start(QHostAddress::Any, port2);

}

void TcpClientManager::restartClient(QString host, int port)
{
   // emit signalRestartClient(host, port);
}

void TcpClientManager::restartClientInternal(QString host, int port)
{
    m_tcpClient->disconnectFromHostSync();
    m_tcpClient->connectToHostAsync(host, port);
}

void TcpClientManager::restartServer(int port)
{
   // emit signalRestartServer(port);
}

void TcpClientManager::restartServerInternal(int port, int port2)
{
    m_tcpServer->stop();
    m_tcpServer->start(QHostAddress::Any, port);
    m_tcpServer2->stop();
    m_tcpServer2->start(QHostAddress::Any, port2);
}

void TcpClientManager::sendEnemyIntersectToServer(UavInfo uav, double azimuth, double distance,
                                                  QString name)
{
    QStringList niippLst;
    niippLst.append("BPLA");
    niippLst.append(uav.name);
    niippLst.append( QString::number(uav.lat) );
    niippLst.append( QString::number(uav.lon) );
    niippLst.append( QString::number((int)uav.alt) );
    niippLst.append( "DETECT" );
    niippLst.append(name);
    niippLst.append(QString::number(azimuth));
    niippLst.append(QString::number(distance));
    niippLst.append("\r\n");

    QByteArray msg = niippLst.join(",").toAscii();
    m_tcpServer->sendData(msg);
}

void TcpClientManager::sendEnemyIntersectToClient(UavInfo uav, double azimuth, double distance,
                                                  QString name)
{
    QStringList niippLst;
    niippLst.append("BPLA");
    niippLst.append(uav.name);
    niippLst.append( QString::number(uav.lat) );
    niippLst.append( QString::number(uav.lon) );
    niippLst.append( QString::number((int)uav.alt) );
    niippLst.append( "DETECT" );
    niippLst.append(name);
    niippLst.append(QString::number(azimuth));
    niippLst.append(QString::number(distance));
    niippLst.append("\r\n");

    QByteArray msg = niippLst.join(",").toAscii();
    m_tcpClient->sendData(msg);
}

void TcpClientManager::sendEnemyToServer(UavInfo uav)
{
    QStringList niippLst;
    niippLst.append("BPLA");
    niippLst.append(uav.name);
    niippLst.append( QString::number(uav.lat) );
    niippLst.append( QString::number(uav.lon) );
    niippLst.append( QString::number((int)uav.alt) );
    niippLst.append("\r\n");

    QByteArray msg = niippLst.join(",").toAscii();
    m_tcpServer->sendData(msg);
    sendEnemyToServer2(uav);
}

void TcpClientManager::sendEnemyToClient(UavInfo uav)
{
    QStringList niippLst;
    niippLst.append("BPLA");
    niippLst.append(uav.name);
    niippLst.append( QString::number(uav.lat) );
    niippLst.append( QString::number(uav.lon) );
    niippLst.append( QString::number(uav.alt) );
    niippLst.append("\r\n");

    QByteArray msg = niippLst.join(",").toAscii();
    m_tcpClient->sendData(msg);
}

void TcpClientManager::sendEnemyToServer2(UavInfo uav)
{
    QStringList vaenga;
    vaenga.append("BPLA");
    vaenga.append( QString::number(uav.id) );
    vaenga.append( QString::number(uav.uavId) );
    vaenga.append(uav.name);
    vaenga.append( uav.dateTime.toString() );
    vaenga.append( uav.restTime.toString() );
    vaenga.append( QString::number(uav.lat) );
    vaenga.append( QString::number(uav.lon) );
    vaenga.append( QString::number(uav.latStddev) );
    vaenga.append( QString::number(uav.lonStddev) );
    vaenga.append( QString::number((int)uav.alt) );
    vaenga.append( QString::number(uav.speed) );
    vaenga.append( QString::number(uav.yaw) );
    vaenga.append("\r\n");

    QByteArray msg = vaenga.join(",").toAscii();
    m_tcpServer2->sendData(msg);
}

void TcpClientManager::addTcpDevice(QObject *controller)
{
    QThread* controllerThread = new QThread;
    connect(controller, SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
    connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
    connect(this, SIGNAL(threadTerminateSignal()), controller, SLOT(deleteLater()));
    connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
    controller->moveToThread(controllerThread);
    controllerThread->start();
}

void TcpClientManager::reconnectAll(QString clientHost, int clientPort, int serverPort)
{
    emit signalReconnectAll(clientHost, clientPort, serverPort);
}

void TcpClientManager::reconnectAllInternal(QString clientHost, int clientPort, int serverPort)
{
    if(m_clientHost != clientHost || m_clientPort != clientPort) {
        restartClient(clientHost, clientPort);
    }

    if(m_serverPort != serverPort) {
        restartServer(serverPort);
    }
}
