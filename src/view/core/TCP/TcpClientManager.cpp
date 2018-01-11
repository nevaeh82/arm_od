#include <QString>
#include "TcpClientManager.h"

TcpClientManager::TcpClientManager(QObject* parent) :
    QObject(parent)
{
    m_tcpClient = new TcpPvoClient();
    m_tcpServer = new TcpPvoServer();

    addTcpDevice(m_tcpClient);
    addTcpDevice(m_tcpServer);

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

void TcpClientManager::restartServerInternal(int port)
{
    m_tcpServer->stop();
    m_tcpServer->start(QHostAddress::Any, port);
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
