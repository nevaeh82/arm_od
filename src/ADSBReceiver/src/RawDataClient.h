#pragma once

#include "IADSBTcpClient.h"


//////////////////////   R A W   D A T A   C L I E N T   //////////////////////

class RawDataClient : public IADSBTcpClient {
    Q_OBJECT
public:
    RawDataClient( QObject* parent = 0 );
    ~RawDataClient( ) { }
signals:
    void signal_DataHasCame( const char* data, int len);
public slots:
    virtual void slot_SendRawData( const char* data, int len );
protected slots:
    virtual void slot_ReadyRead();
};
