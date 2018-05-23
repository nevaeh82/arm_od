#include "TCPClient.h"
#include <QTimer>

//////////////////////////   К О Н С Т Р У К Т О Р   //////////////////////////

TCPClient::TCPClient(QObject* pwgt )
	: QObject(pwgt)
    , try_to_connect_(false)
    , m_pTcpSocket(new QTcpSocket(this))
    , msec_reconnect_(5000) {
    bool ans = connect( m_pTcpSocket, SIGNAL( connected() ),
                        this, SLOT( slot_Connected() ) );
    ans = ans && connect( m_pTcpSocket, SIGNAL( disconnected() ),
                          this, SLOT( slot_Disconnected()) );
    ans = ans && connect( m_pTcpSocket, SIGNAL( readyRead()),
                          this, SLOT( slot_ReadyRead()) );
    ans = ans && connect( m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                          this, SLOT(slot_Error(QAbstractSocket::SocketError)) );
    Q_ASSERT_X(ans,__FUNCTION__,"Не удалось создать подключение к слоту ошибок");
}


//////////////////////////   S E T   A D D R E S S   //////////////////////////

void TCPClient::SetAddress(const QString& strHost, int nPort) {
    m_pTcpSocket->abort();
    emit signal_SendMessage("Попытка установления соединения с адресом: "
                            + strHost + "; порт = " + QString::number(nPort));
    try_to_connect_ = true;
    m_pTcpSocket->connectToHost(strHost, nPort);
    host_for_try_ = strHost;
    port_for_try_ = nPort;
}


/////////////////////////   I S   C O N N E C T E D   /////////////////////////

bool TCPClient::IsConnected()const {
    return m_pTcpSocket->isValid();
}


///////////////////////////   D I S C O N N E C T   ///////////////////////////

void TCPClient::Disconnect( ) {
    emit signal_SendMessage("Разрыв соединения...");
    try_to_connect_ = false;
    m_pTcpSocket->disconnectFromHost();
}


///////////////////////////   S L O T   E R R O R   ///////////////////////////

void TCPClient::slot_Error(QAbstractSocket::SocketError err)
{
    QString strError =
        "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                     "Узел не найден." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "Удаленный узел закрыт." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "Соединение было разорвано." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_pTcpSocket->abort();
    emit signal_SendMessage(strError);
    if( try_to_connect_ ) {
        emit signal_SendMessage( "Попытка повторного установления соединения"
                                 " будет предпринята через "
                                 + QString::number( double(msec_reconnect_)
                                                    / 1000.0 ) + " секунд." );
        QTimer::singleShot( msec_reconnect_, this,
                            SLOT( slot_DefferedReconnect() ) );
    }
    else {
        emit signal_Connected(false);
    }
}


///////////////////////   S L O T   C O N N E C T E D   ///////////////////////

void TCPClient::slot_Connected()
{
    emit signal_SendMessage("Соединение установлено");
    emit signal_Connected(true);
}


////////////////////   S L O T   D I S C O N N E C T E D   ////////////////////

void TCPClient::slot_Disconnected()
{
    emit signal_SendMessage("Соединение разорвано");
    emit signal_Connected(false);
}


////////////   S L O T   D E F F E R E D   R E C O N N E C T E D   ////////////

void TCPClient::slot_DefferedReconnect() {
    if( try_to_connect_ ) {
        m_pTcpSocket->abort();
        emit signal_SendMessage( "Повторная попытка установления соединения"
                                 " с адресом: " + host_for_try_ + "; порт = "
                                 + QString::number(port_for_try_) );
        m_pTcpSocket->connectToHost( host_for_try_, port_for_try_ );
    }
}


///////////////////////////   Д Е С Т Р У К Т О Р   ///////////////////////////

TCPClient::~TCPClient(){
    this->Disconnect();
}

void TCPClient::SetMsecReconnect(int msec) {
    if ( msec > 0 ) {
        msec_reconnect_ = msec;
    }
}

