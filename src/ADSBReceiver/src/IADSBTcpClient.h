#pragma once


///////////////////////////   T C P   C L I E N T   ///////////////////////////

#include <QObject>
#include <QTcpSocket>

class IADSBTcpClient : public QObject
{
    Q_OBJECT
public:
	explicit IADSBTcpClient( QObject* pwgt = 0 );
    bool IsConnected()const;
    QString GetHostAddress()const;
    int GetPortNumber()const;
    bool IsTryingToConnect()const;
	virtual ~IADSBTcpClient();
    void SetMsecReconnect( int msec );
public slots:
    void slot_ConnectToHost( bool connect );//!законнектиться
    void slot_ConnectToHost(const QString& strHost, int nPort);
    void slot_Disconnect();                //!расконнектиться
    void slot_SetPortNumber( int val ); //!задать номер порта
    void slot_SetHostAddress( const QString& host );//!задать хост
signals:
    void signal_PortNumberChanged( int val ); //!задать номер порта
    void signal_HostAddressChanged( const QString& host );//!задать хост
    void signal_SendMessage(const QString& str);
    void signal_Connected( bool );
    void signal_TryToConnectToServer( bool conn );//!пытаемся коннектиться
protected slots:
    virtual void slot_ReadyRead()=0;
    virtual void slot_Error(QAbstractSocket::SocketError);
    virtual void slot_Connected();
    virtual void slot_Disconnected();
    virtual void slot_DefferedReconnect();
protected:
    QTcpSocket* m_pTcpSocket;
    bool try_to_connect_;
    QString host_address_;
    int port_number_;
    int msec_reconnect_;
};
