#include "RawDataClient.h"
#include <QHostAddress>
#include <QDataStream>


RawDataClient::RawDataClient( QObject* parent ): IADSBTcpClient( parent ) { }


///////////////////   S L O T   S E N D   R A W   D A T A   ///////////////////

void RawDataClient::slot_SendRawData(const char *data, int len) {
//    QByteArray arr( data, len);
//    QString str(arr);
//    emit signal_SendMessage( QString("Посылаются данные на адрес %1:%2 \"%3\"")
//                             .arg( host_address_ ).arg(port_number_).arg(str) );
    QDataStream out(m_pTcpSocket);
    out.writeRawData( data, len );
}

//210137
//////////////////////   S L O T   R E A D Y   R E A D   //////////////////////

void RawDataClient::slot_ReadyRead() {
    QDataStream in(m_pTcpSocket);
    const int max_len = 10000;
    const int min_len = 10;
    char buf[max_len] ;
    int len=-1, bytes_available;
    for (;;) {
        if ( m_pTcpSocket->bytesAvailable() < 1 ) {
            break;
        }
        bytes_available = m_pTcpSocket->bytesAvailable();
        if (bytes_available < min_len * sizeof(char)) {
            break;
        }
        if(bytes_available > max_len * sizeof(char)) {
            bytes_available = max_len * sizeof(char);
        }
        len = in.readRawData(buf,bytes_available);
        if( len < 0 ) {
            break;
        }
        emit signal_DataHasCame( buf, len );
//        QTcpSocket* sender_sock = static_cast<QTcpSocket*>(sender());
//        emit signal_SendMessage( QString("пришли данные с адреса %1:%2 \"%3\"")
//                                 .arg( sender_sock->peerAddress().toString() )
//                                 .arg( sender_sock->peerPort() )
//                                 .arg( QString( QByteArray(buf, len) ) ) );
    }
}
