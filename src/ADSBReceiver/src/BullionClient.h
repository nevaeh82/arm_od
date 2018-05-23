#ifndef BULLIONCLIENT_H
#define BULLIONCLIENT_H

#include "RawDataClient.h"


///////////////////////   B U L L I O N   C L I E N T   ///////////////////////

class BullionClient : public RawDataClient {
    Q_OBJECT
public:
    BullionClient( QObject* parent = 0 );
    bool HasWaitingPacket()const;//!есть ли ожидающий пакет данных?
    ~BullionClient( ) { }
protected slots:
    void slot_ReadyRead();
protected:
    QByteArray m_dataFromTcpSocket;
};
#endif // BULLIONCLIENT_H
