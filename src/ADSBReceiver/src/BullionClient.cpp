#include "BullionClient.h"
#include <QHostAddress>
#include <QTime>
#include <QApplication>
#include <QDataStream>

BullionClient::BullionClient( QObject* parent ): RawDataClient( parent ) { }

bool BullionClient::HasWaitingPacket() const {
    int prefixIndex = m_dataFromTcpSocket.lastIndexOf( '*' );
    int suffixIndex = m_dataFromTcpSocket.lastIndexOf( ';' );
    int suffixIndex1 = m_dataFromTcpSocket.lastIndexOf( '\n' );
    int suffixIndex2 = m_dataFromTcpSocket.lastIndexOf( '\r' );
    return prefixIndex >= 0 && suffixIndex > 0
            && suffixIndex > prefixIndex
            && + m_dataFromTcpSocket.size() > suffixIndex + 3
            && suffixIndex1 == suffixIndex + 1
            && suffixIndex2 == suffixIndex1 + 1;
}


//210137
//////////////////////   S L O T   R E A D Y   R E A D   //////////////////////

void BullionClient::slot_ReadyRead() {
    int max_packet_lenght = 1000000;
    m_dataFromTcpSocket.append( m_pTcpSocket->readAll() );
    const int pack_len = 1000;
    char min_buf[pack_len];
    int j;
    bool start_write;
    while( this->HasWaitingPacket() ) {
        if( m_dataFromTcpSocket.size() > int(max_packet_lenght)) {
            emit signal_SendMessage( "Размер буфера превысил свой предел,"
                                     " удаляем данные с начала..." );
            m_dataFromTcpSocket = m_dataFromTcpSocket.
                    right( m_dataFromTcpSocket.length()
                           - int(max_packet_lenght) );
        }
        int len = m_dataFromTcpSocket.length();
        start_write = false;
        for ( int i = 0; i < len; i++ ) {
            if( !start_write && m_dataFromTcpSocket[i] == '*' ) {
                start_write = true;
                j = 0;
            }
            if ( start_write ) {
                min_buf[j] = m_dataFromTcpSocket[i];
                j++;
            }

            if( start_write && i > 1 && i < len && m_dataFromTcpSocket[i-2] == ';'
                && m_dataFromTcpSocket[i-1] == '\n' && m_dataFromTcpSocket[i] == '\r' ) {
                start_write = false;
                emit signal_DataHasCame( min_buf, j );
                if ( i + 1< len ) {
                    m_dataFromTcpSocket = m_dataFromTcpSocket.mid(i+1);
                } else {
                    m_dataFromTcpSocket.clear();
                }
                break;
            }
        }
    }
//    QDataStream in(m_pTcpSocket);
//    const int min_len = 10;
//    const int read_pack_len = 100;
//    char buf[read_pack_len] ;
//    char min_buf[read_pack_len];

//    int len=-1, bytes_available;
//    for (;;) {
//        if ( m_pTcpSocket->bytesAvailable() < 1 ) {
//            break;
//        }
//        bytes_available = m_pTcpSocket->bytesAvailable();
//        if (bytes_available < min_len * sizeof(char)) {
//            break;
//        }
//        len = in.readRawData( buf, read_pack_len );
//        if( len < 0 ) {
//            break;
//        }
//        bool start_write = false;
//        int j;
//        for ( int i = 0; i < len; i++ ) {
//            if( !start_write && buf[i] == '*' ) {
//                start_write = true;
//                j = 0;
//            }
//            if ( start_write ) {
//                min_buf[j] = buf[i];
//                j++;
//            }

//            if( start_write && i > 1 && i < len && buf[i-2] == ';'
//                && buf[i-1] == '\n' && buf[i] == '\r' ) {
//                start_write = false;
//                emit signal_DataHasCame( min_buf, j );
//            }
//        }
//        QTcpSocket* sender_sock = static_cast<QTcpSocket*>(sender());
//        emit signal_SendMessage( QString("пришли данные с адреса %1:%2 \"%3\"")
//                                 .arg( sender_sock->peerAddress().toString() )
//                                 .arg( sender_sock->peerPort() )
//                                 .arg( QString( QByteArray(buf, len) ) ) );
//    }
}
