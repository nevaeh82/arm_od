#ifndef TCPCLIENT_H
#define TCPCLIENT_H


///////////////////////////   T C P   C L I E N T   ///////////////////////////

#include <QWidget>
#include <QTcpSocket>

/**
 * @brief The TCPClient class
 */
class TCPClient : public QObject
{
    Q_OBJECT
public:
	explicit TCPClient( QObject* pwgt = 0 );
    void SetAddress(const QString& strHost, int nPort);
    bool IsConnected()const;
    void Disconnect();
    virtual ~TCPClient();
    void SetMsecReconnect( int msec );
signals:
    void signal_SendMessage(const QString& str);
    void signal_Connected( bool );
protected slots:
	virtual void slot_ReadyRead() = 0;
    virtual void slot_Error(QAbstractSocket::SocketError);
    virtual void slot_Connected();
    virtual void slot_Disconnected();
    virtual void slot_DefferedReconnect();
protected:
    QTcpSocket* m_pTcpSocket;
    bool try_to_connect_;
    QString host_for_try_;
    int port_for_try_;
    int msec_reconnect_;
};

#endif // TCPCLIENT_H
