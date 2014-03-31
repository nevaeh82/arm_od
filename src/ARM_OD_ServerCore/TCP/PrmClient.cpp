#include "PrmClient.h"

PrmClient::PrmClient(const QString& strHost, int nPort, QObject* parent) :
    QObject(parent), newData(true)
{
    mTcpSocket = new QTcpSocket(this);

    mTcpSocket->connectToHost(strHost, nPort);
    mTcpSocket->setReadBufferSize(1544);
    connect(mTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void PrmClient::getFreq()
{
    QDataStream streamWrite(mTcpSocket);
    quint8 aStart, aAddr, aSize, aBodyType, aCrc;

    //Запрос о текущей частоте
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1;            //0x01
    aBodyType=4;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aCrc=calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType<<aCrc;

    qDebug()<<"Отправил железу запрос о частоте";
    mTimeStart=QTime::currentTime();
}


void PrmClient::setFreq(quint16 aFreq)
{
    QDataStream streamWrite(mTcpSocket);
    quint8 aStart, aAddr, aSize, aBodyType, aCrc, aFreqFirst, aFreqLast, aTemp;
    quint8 aSpecialB;
    aTemp=0;

    //Установка частоты
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1+2;
    aBodyType=2;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);

    aFreqFirst=((aFreq>>8)&0xFF);
    aFreqLast=(aFreq&0xFF);

    aForCrcCount.append(aFreqFirst); aForCrcCount.append(aFreqLast);
    aCrc=calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType;

    //Проверка специальных символов
    if (specialSymb(aFreqFirst, aSpecialB)) streamWrite<<aFreqFirst<<aSpecialB;
    else streamWrite<<aFreqFirst;

    if (specialSymb(aFreqLast, aSpecialB)) streamWrite<<aFreqLast<<aSpecialB;
    else streamWrite<<aFreqLast;

    if (specialSymb(aCrc, aSpecialB)) streamWrite<<aCrc<<aSpecialB;
    else streamWrite<<aCrc;

    qDebug()<<"Отправил железу частоту для установки";
    mTimeStart=QTime::currentTime();
}

void PrmClient::setAttenuator1(quint8 aValue)
{
    QDataStream streamWrite(mTcpSocket);
    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка аттенюатора 1
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1+1;
    aBodyType=3;
    aBodyData=aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    aCrc=calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType<<aBodyData;

    //Проверка специальных символов
    if (specialSymb(aCrc, aSpecialB)) streamWrite<<aCrc<<aSpecialB;
    else streamWrite<<aCrc;

    qDebug()<<"Отправил железу данные для установки аттенюатора1";
    mTimeStart=QTime::currentTime();
}

void PrmClient::setAttenuator2(quint8 aValue)
{
    QDataStream streamWrite(mTcpSocket);
    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка аттенюатора 2
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1+1;
    aBodyType=7;
    aBodyData=aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    aCrc=calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType<<aBodyData;

    //Проверка специальных символов
    if (specialSymb(aCrc, aSpecialB)) streamWrite<<aCrc<<aSpecialB;
    else streamWrite<<aCrc;

    qDebug()<<"Отправил железу данные для установки аттенюатора2";
    mTimeStart=QTime::currentTime();
}

void PrmClient::setFiltr(quint8 aValue)
{
    QDataStream streamWrite(mTcpSocket);
    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка фильтра
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1+1;
    aBodyType=6;
    aBodyData=aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    aCrc=calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType<<aBodyData;

    //Проверка специальных символов
    if (specialSymb(aCrc, aSpecialB)) streamWrite<<aCrc<<aSpecialB;
    else streamWrite<<aCrc;

    qDebug()<<"Отправил железу данные для установки фильтра";
    mTimeStart=QTime::currentTime();
}

void PrmClient::slotReadyRead()
{
    qDebug()<<"От приемника пришли данные";

    QVector<quint8> aForCrcCount; //Для проверки CRC8
    QDataStream in(mTcpSocket); in.setVersion(QDataStream::Qt_4_8);
    quint8 aFirstByte, aAddr, aSize, aPacketType, aLastByte, aCrc;
    quint8 aSizeAll;

    for (;;)
    {
        //Проверка есть ли непрочитанные данные, если нет, то break
        if (newData)
        {
            if (mTcpSocket->bytesAvailable() < 1) break;

            qDebug()<<mTcpSocket->bytesAvailable()<<" байт пришло";
            aSizeAll=mTcpSocket->bytesAvailable();

            //Считывание первого байта
            in>>aFirstByte;

            qDebug()<<aFirstByte<<" первый байт";

            if (aFirstByte==85)
            {
                qDebug()<<"Нет данных";
                break;
            }

            if (mTcpSocket->bytesAvailable() < 3) break;
            {
                if (aFirstByte==170)

                {
                    qDebug()<<"стартовый байт прочитан";

                    in>>aAddr;
                    qDebug()<<"адрес передачи "<<aAddr;

                    in>>aSize;
                    qDebug()<<"размер тела "<<aSize;

                    in>>aPacketType;
                    qDebug()<<"тип пакета "<<aPacketType;

                    newData=false;
                    mTimeStop=QTime::currentTime();
                    qDebug()<<"Время ответа "<<mTimeStart.msecsTo(mTimeStop)<<" мсек";
                }
                else break;
            }
        }

        //Проверка пришли ли остальные данные aSize
        if (mTcpSocket->bytesAvailable() < (aSize)) break;
        //Все данные пришли, разбираем
        newData=true;

        if (aPacketType==2)
        {
            qDebug()<<"Ответ об изменении частоты";
            quint16 aFlag;
            in>>aFlag;

            qDebug()<<aFlag;
            emit freqChanged();
        }

        if (aPacketType==3)
        {
            qDebug()<<"Ответ об изменении аттенюатора1";
            quint16 aFlag;
            in>>aFlag;

            qDebug()<<aFlag;

            emit attenuator1Changed();
        }

        if (aPacketType==4)
        {
            qDebug()<<"Ответ о частоте";
            quint16 aFreq;
            quint8 aAt1, aAt2, aAt3, aFltr, aFreqA, aFreqB, aFreqC, aFreqFirst, aFreqLast;
            bool aChangeCRCFlag=false; //Флаг обратной замены в CRC

            if (aSizeAll!=12) in>>aFreq; //Если aSizeAll=12, то где-то нужно делать обратную замену спецсимволов
            else
            {
                //Обратная замена спецсимволов в частоте
                qDebug()<<"Обратная замена специальных символов";
                in>>aFreqA>>aFreqB>>aFreqC;
                if (aFreqA==(0x5A))
                {
                    if (aFreqB==0x00)
                    {
                        aFreqFirst=0x5A;
                        aFreqLast=aFreqC;
                    }
                    else if (aFreqB==0x01)
                    {
                        aFreqFirst=0x55;
                        aFreqLast=aFreqC;
                    }
                    else if (aFreqB==0x02)
                    {
                        aFreqFirst=0xA5;
                        aFreqLast=aFreqC;
                    }
                    else if (aFreqB==0x03)
                    {
                        aFreqFirst=0xAA;
                        aFreqLast=aFreqC;
                    }

                } else if (aFreqB==(0x5A))
                {
                    if (aFreqC==0x00)
                    {
                        aFreqFirst=aFreqA;
                        aFreqLast=0x5A;
                    }
                    else if (aFreqC==0x01)
                    {
                        aFreqFirst=aFreqA;
                        aFreqLast=0x55;
                    }
                    else if (aFreqC==0x02)
                    {
                        aFreqFirst=aFreqA;
                        aFreqLast=0xA5;
                    }
                    else if (aFreqC==0x03)
                    {
                        aFreqFirst=aFreqA;
                        aFreqLast=0xAA;
                    }
                } else aChangeCRCFlag=true;

                //перевод 2х quint8 в quint16
                if (!aChangeCRCFlag) aFreq=256*aFreqFirst+aFreqLast;
                else
                {
                    aFreq=256*aFreqA+aFreqB;
                    aAt1=aFreqC;
                }

            }

            if (!aChangeCRCFlag) in>>aAt1>>aFltr>>aAt2>>aAt3>>aLastByte;
            else
            {
                //Обратная замена спецсимволов в CRC
                in>>aFltr>>aAt2>>aAt3>>aFreqA>>aFreqB;
                if (aFreqA==(0x5A))
                {
                    if (aFreqB==0x00)   aLastByte=0x5A;
                    else if (aFreqB==0x01)  aLastByte=0x55;
                    else if (aFreqB==0x02)  aLastByte=0xA5;
                    else if (aFreqB==0x03)  aLastByte=0xAA;
                }
            }

            qDebug()<<"Частота "<<aFreq;
            qDebug()<<"Фильтр "<<aFltr;
            qDebug()<<"Аттенюатор1 "<<aAt1;
            qDebug()<<"Аттенюатор2 "<<aAt2;
            qDebug()<<"Аттенюатор3 "<<aAt3;
            //Проверка CRC
            aForCrcCount.append(aAddr);
            aForCrcCount.append(aSize);
            aForCrcCount.append(aPacketType);
            aForCrcCount.append((aFreq>>8)&0xFF); aForCrcCount.append(aFreq&0xFF); aForCrcCount.append(aAt1); aForCrcCount.append(aFltr); aForCrcCount.append(aAt2); aForCrcCount.append(aAt3);

            aCrc=calcCRC(aForCrcCount);
            qDebug()<<"Последний байт "<<aLastByte<<" CRC8 "<<aCrc;

            emit sendPrmFreq(aFreq, aAt1, aAt2, aFltr);
        }

        if (aPacketType==6)
        {
            qDebug()<<"Ответ об изменении фильтра";
            quint16 aFlag;
            in>>aFlag;

            qDebug()<<aFlag;

            emit filterChanged();
        }

        if (aPacketType==7)
        {
            qDebug()<<"Ответ об изменении аттенюатора2";
            quint16 aFlag;
            in>>aFlag;

            qDebug()<<aFlag;

            emit attenuator2Changed();
        }
    }
}

void PrmClient::slotConnected()
{
    emit sendText(QString("Подключен к приемнику " + mTcpSocket->peerAddress().toString()));
    emit deviceConnected(true);
    getFreq();
}

void PrmClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                              "приемник не найден " :
                              err == QAbstractSocket::RemoteHostClosedError ?
                                  "приемник закрыл соединение " :
                                  err == QAbstractSocket::ConnectionRefusedError ?
                                      "не удалось подключиться к приемнику " :
                                      err == QAbstractSocket::SocketTimeoutError ?
                                          "не удалось подключиться к приемнику " :
                                          QString(mTcpSocket->errorString()));
    emit sendText(strError + mTcpSocket->peerAddress().toString());
    emit deviceConnected(false);
}

quint8 PrmClient::calcCRC(QVector<quint8> aForCrcCount)
{
    int aSize;
    quint8 c=0;
    aSize=aForCrcCount.size();

    if(aSize<=12)
    {
        for(int i=0; i<aSize; i++) c += aForCrcCount.at(i);
        return c+1;
    } else return 0;
}

bool PrmClient::specialSymb(quint8 &aByteA, quint8 &aByteB)
{
    bool aTemp=false;
    if (aByteA==0x5A)
    {
        aByteA=0x5A;
        aByteB=0x00;
        aTemp=true;
    }
    else if (aByteA==0x55)
    {
        aByteA=0x5A;
        aByteB=0x01;
        aTemp=true;
    }
    else if (aByteA==0xA5)
    {
        aByteA=0x5A;
        aByteB=0x02;
        aTemp=true;
    }
    else if (aByteA==0xAA)
    {
        aByteA=0x5A;
        aByteB=0x03;
        aTemp=true;
    }
    return aTemp;
}
