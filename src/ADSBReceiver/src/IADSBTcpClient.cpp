#include "IADSBTcpClient.h"
#include <QTimer>
#include <QHostAddress>


//////////////////////////   К О Н С Т Р У К Т О Р   //////////////////////////

IADSBTcpClient::IADSBTcpClient( QObject* pwgt )
    : QObject(pwgt)
    , try_to_connect_(false)
    , m_pTcpSocket(new QTcpSocket(this))
    , host_address_("no_host")
    , port_number_(-1)
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

void IADSBTcpClient::slot_ConnectToHost(const QString& strHost, int nPort) {
    m_pTcpSocket->abort();
    if ( try_to_connect_== false ) {
        try_to_connect_ = true;
        emit signal_SendMessage("Попытка установления соединения с адресом: "
                                + strHost + "; порт = " + QString::number(nPort));
        emit signal_TryToConnectToServer( try_to_connect_ );
    }
    slot_SetHostAddress(strHost);
    slot_SetPortNumber(nPort);
    m_pTcpSocket->connectToHost(host_address_, port_number_);
}


/////////////////////////   I S   C O N N E C T E D   /////////////////////////

bool IADSBTcpClient::IsConnected()const {
    return m_pTcpSocket->state() == QTcpSocket::ConnectedState;
}

QString IADSBTcpClient::GetHostAddress() const {
    return host_address_;
}

int IADSBTcpClient::GetPortNumber() const {
    return port_number_;
}

////////////////   S L O T   S E T   P O R T   F O R   T R Y   ////////////////

void IADSBTcpClient::slot_SetPortNumber( int val ) {
    if ( val < 0 ||val > 65536 ) {
        emit signal_SendMessage( QString( "Недопустимый номер порта"
                                          " = %1" ).arg( val ));
        emit signal_PortNumberChanged( port_number_ );
    } else {
        if ( port_number_!= val ) {
            port_number_ = val;
            emit signal_PortNumberChanged( val );
        }
    }
}


////////////////   S L O T   S E T   H O S T   F O R   T R Y   ////////////////

void IADSBTcpClient::slot_SetHostAddress( const QString& host ) {

    if ( QHostAddress(host).toIPv4Address() < 0 ) {
        emit signal_SendMessage( QString( "Недопустимый ip-адрес "
                                          " = %1" ).arg( host ));
        emit signal_HostAddressChanged( host_address_ );
    } else {
        if ( host_address_!= host ) {
            host_address_ = host;
            emit signal_HostAddressChanged( host );
        }
    }
}

/////////////////   S L O T   C O N N E C T   T O   H O S T   /////////////////

void IADSBTcpClient::slot_ConnectToHost( bool connect ) {
    if ( connect ) {
        this->slot_ConnectToHost( host_address_, port_number_ );
    } else if ( this->IsConnected() || this->IsTryingToConnect() ) {
        this->slot_Disconnect();
    }
}

/////////////////   I S   T R Y I N G   T O   C O N N E C T   /////////////////

bool IADSBTcpClient::IsTryingToConnect() const {
    return try_to_connect_;
}


///////////////////////////   D I S C O N N E C T   ///////////////////////////

void IADSBTcpClient::slot_Disconnect( ) {
    if ( try_to_connect_ == true ) {
        try_to_connect_ = false;
        emit signal_SendMessage("Разрыв соединения...");
        emit signal_TryToConnectToServer( false );
    }
    m_pTcpSocket->disconnectFromHost();
}


///////////////////////////   S L O T   E R R O R   ///////////////////////////

void IADSBTcpClient::slot_Error(QAbstractSocket::SocketError err)
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

void IADSBTcpClient::slot_Connected()
{
    emit signal_SendMessage("Соединение установлено");
    emit signal_Connected(true);
}


////////////////////   S L O T   D I S C O N N E C T E D   ////////////////////

void IADSBTcpClient::slot_Disconnected()
{
    emit signal_SendMessage("Соединение разорвано");
    emit signal_Connected(false);
}


////////////   S L O T   D E F F E R E D   R E C O N N E C T E D   ////////////

void IADSBTcpClient::slot_DefferedReconnect() {
    if( try_to_connect_ && !this->IsConnected() ) {
        m_pTcpSocket->abort();
        emit signal_SendMessage( "Повторная попытка установления соединения"
                                 " с адресом: " + host_address_ + "; порт = "
                                 + QString::number(port_number_) );
        m_pTcpSocket->connectToHost( host_address_, port_number_ );
    }
}


///////////////////////////   Д Е С Т Р У К Т О Р   ///////////////////////////

IADSBTcpClient::~IADSBTcpClient(){
    this->slot_Disconnect();
}

void IADSBTcpClient::SetMsecReconnect(int msec) {
    if ( msec > 0 ) {
        msec_reconnect_ = msec;
    }
}

