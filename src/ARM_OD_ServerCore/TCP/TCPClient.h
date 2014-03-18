#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qhostaddress.h>
#include <QString>
#include <QtNetwork/QtNetwork>
#include <QMutex>
#include <QMutexLocker>

#include <cstdlib>
#include <deque>
#include <iostream>

#include <QMutexLocker>

#include "ITCPClient.h"
#include "ITCPParser.h"
#include "../Common/IRouter.h"
#include "../Common/IClient.h"
#include "../Common/ISubscriber.h"

class TCPClient : public ITCPClient, public IClient
{
    Q_OBJECT
public:
    TCPClient(QString host, quint16 port, IRouter *router);
    ~TCPClient();

public:
    virtual void set_parser(ITCPParser* parser);
    virtual void set(QByteArray data);
    virtual void distruct();

    virtual bool get_state();

public:
    virtual void set_id(int id);
    virtual int  get_id();
    virtual void set_type(int type);
    virtual int  get_type();
	virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);



public:
    QTcpSocket *socket_;

private:
  bool _should_stop;

  QString _host;
  quint16 _port;

  bool _isRead;

  ITCPParser            *_parser;
  IRouter               *_router;

  QMutex                _mutex;
  bool                  _state;
  int                   _type;          /// type of tcp connection (influencee for coder/decoder)
  int                   _id;
  QString               _name;
//  int                   _id;

  ISubscriber*          _subscriber;
  bool                  _is_connected;

signals:
  void recv_msg(const char *);
  void err_connection();
  void finished();
  void error(QString err);
  void connected();

  void signalConnected(bool state);
  void signalReconnection();

  void signalSend(QByteArray data);

//  void signalPing(bool);

//  void signalPrepareToWrite(QSharedPointer<IMessage> msg_ptr);


public slots:
  void proccess();
  void stop();


  void slotDestroy();

private slots:
  void _slotWrite(QByteArray data);
//  void _slotPrepareToWrite(QSharedPointer<IMessage> msg_ptr);

private slots:
     void onSocketConnected();
     void onSocketDisconnected();
     void onSocketReadyRead();
     void onSocketDisplayError(QAbstractSocket::SocketError socketError);

     void finish_connection();
     void reconnection();
     void slotState(QAbstractSocket::SocketState state);
};

#endif // TCPCLIENT_H
