
int main()
{
	return 0;
}

/// prm controls and data flow

unsigned char Modify5A(unsigned char byte)
{
    switch(byte)
    {
        case 0x00: return 0x5A;
        case 0x01: return 0x55;
        case 0x02: return 0xA5;
        case 0x03: return 0xAA;
    }
}

/// recv data from ADC PRM
void GraphicWidget::slotReadyReadADC()
{
   ZaviruhaPayloadPacketHeader payloadHeader_;

   QVector<QPointF> spectr;
   qreal x,y;
   QString msg;

   for (;;)
   {
       if (newData)
       {
           //Если пришли новые данные
           //Проверяем каждый из 4х байт преамбулы
           if (qSocketADC->bytesAvailable()<PAYLOAD_HEADER_LENGTH) break;
           newData=false;
           Time1=QTime::currentTime();

           //Прочитать заголовок
           QDataStream streamRead(qSocketADC->read(PAYLOAD_HEADER_LENGTH));
           streamRead.setVersion(QDataStream::Qt_4_8);

           //Достать поля
           //Проверка преамбулы
           quint8 aTemp1, aTemp2, aTemp3, aTemp4;
           streamRead>>aTemp1; if (aTemp1!=255) break;
           streamRead>>aTemp2; if (aTemp2!=238) break;
           streamRead>>aTemp3; if (aTemp3!=204) break;
           streamRead>>aTemp4; if (aTemp4!=255) break;
           streamRead>>payloadHeader_.number;
           streamRead>>payloadHeader_.id;
           streamRead>>payloadHeader_.flags;
           streamRead>>payloadHeader_.timestamp;
           streamRead>>payloadHeader_.type;
           streamRead>>payloadHeader_.length;
           streamRead>>payloadHeader_.reserver;
           streamRead>>payloadHeader_.messageCRC;
           streamRead>>payloadHeader_.headerCRC;

//            emit signalSendText("\n");
//            emit signalSendText(QString("id %1\n").arg(payloadHeader_.id));
//            emit signalSendText(QString("Тип сообщения %1\n").arg(payloadHeader_.type));
           blockSize=payloadHeader_.length;
           headerType=payloadHeader_.type;
       }

       if (qSocketADC->bytesAvailable() < blockSize)
       {
//            emit signalSendText(QString("Вывалился т.к. в сокете осталось %1 байт\n").arg(tcpSocket->bytesAvailable()));
//            emit signalSendText(QString("Размер blockSize = %1 байт\n").arg(blockSize));
           break;
       }

       newData=true;
//        emit signalSendText("В сокете накопилось достаточно данных для считывания\n");
       QDataStream streamRead(qSocketADC->read(blockSize));
       streamRead.setVersion(QDataStream::Qt_4_8);
//        if (headerType==0) // 0 - ping,
//        { QString pingText; streamRead>>pingText; blockSize=0;  }

       if (headerType==1) // 1 - FFT (QVector<QPointF>),
       {
           for (int r=0; r<(blockSize/sizeof(QPointF));r++)
           {
               streamRead>>x>>y;
               rSpectr->SetPoint(x,y);
//                if (r==0) emit signalSendText(QString("Первая точка №%1 (%2,%3)\n").arg(r).arg(x).arg(y));
//                if (r==(blockSize/sizeof(QPointF)-1)) emit signalSendText(QString("Последняя точка №%1 (%2,%3)\n").arg(r).arg(x).arg(y));
////	QRectF widgetRect = qSpectrView->childrenRect();
////	scene->setSceneRect(widgetRect);
               scene->update();
           }
           blockSize=0;
//            emit signalSendText(QString("Время приема=%1 мсек\n").arg(mSec));
       }

//        if (headerType==3) //  3 - текст (QString),
//        {  blockSize=0; int mSec=Time1.msecsTo(QTime::currentTime()); }

//        if (headerType==4) // 4 - корреляция (unsigned int номер пункта_A, unsigned int номер пункта_B, QVector<QPointF>),
//        {
//            unsigned int aNumPointA, aNumPointB;
//            streamRead>>aNumPointA>>aNumPointB;
//            for (int r=0; r<(blockSize/sizeof(QPointF));r++)
//            {
//                streamRead>>x>>y;
//                if (r==0) emit signalSendText(QString("Первая точка №%1 (%2,%3)\n").arg(r).arg(x).arg(y));
//                if (r==(blockSize/sizeof(QPointF)-1)) emit signalSendText(QString("Последняя точка №%1 (%2,%3)\n").arg(r).arg(x).arg(y));
//            }
//            blockSize=0;
//            int mSec=Time1.msecsTo(QTime::currentTime());
//        }

//        if (headerType==5)     //   5 - координаты (QPointF),
//        { blockSize=0; int mSec=Time1.msecsTo(QTime::currentTime()); }
//		if ((headerType<0) || (headerType>5)) {}//emit signalSendText("ХЗ что за данные\n");
   }
}


/// set data freq to ADC
void GraphicWidget::SendSetFreqADC(unsigned short aFreq)
{
    ZaviruhaPayloadPacketHeader payloadHeader_;
 //   emit signalSendText("Клиент отправляет центральную частоту\n");
    //Размер поля Data
    int sizeOfData=sizeof(aFreq);
 //   emit signalSendText(QString("Размер aFreq=%1\n").arg(sizeOfData));
    //Формирование заголовка
    payloadHeader_.magic = PAYLOAD_PREAMBULE;
    payloadHeader_.number = 0;
    payloadHeader_.id = 0;
    payloadHeader_.flags = 0;
    payloadHeader_.timestamp = 0;
    payloadHeader_.type = 100; //Установить центральную частоту
    payloadHeader_.length = sizeOfData;
    payloadHeader_.reserver = 0;
    payloadHeader_.messageCRC = 0;//crc16(reinterpret_cast<unsigned char *>(&points_spectr), sizeOfData);
    payloadHeader_.headerCRC = 0;//crc8(reinterpret_cast<unsigned char *>(&payloadHeader_), PAYLOAD_HEADER_LENGTH);

    QDataStream streamWrite(qSocketADC);

    streamWrite<<payloadHeader_.magic;
    streamWrite<<payloadHeader_.number;
    streamWrite<<payloadHeader_.id;
    streamWrite<<payloadHeader_.flags;
    streamWrite<<payloadHeader_.timestamp;
    streamWrite<<payloadHeader_.type;
    streamWrite<<payloadHeader_.length;
    streamWrite<<payloadHeader_.reserver;
    streamWrite<<payloadHeader_.messageCRC;
    streamWrite<<payloadHeader_.headerCRC;
    streamWrite<<aFreq;
//    emit signalSendText(QString("Клиент бинарные данные отправил\n"));
}

/// set filter ADC
void GraphicWidget::SendSetFiltrADC(int aFiltr)
{
    ZaviruhaPayloadPacketHeader payloadHeader_;
//    emit signalSendText("Клиент отправляет фильтр\n");
    //Размер поля Data
    int sizeOfData=sizeof(aFiltr);
//    emit signalSendText(QString("Размер aFreq=%1\n").arg(sizeOfData));
    //Формирование заголовка
    payloadHeader_.magic = PAYLOAD_PREAMBULE;
    payloadHeader_.number = 0;
    payloadHeader_.id = 0;
    payloadHeader_.flags = 0;
    payloadHeader_.timestamp = 0;
    payloadHeader_.type = 101; //Фильтр
    payloadHeader_.length = sizeOfData;
    payloadHeader_.reserver = 0;
    payloadHeader_.messageCRC = 0;//crc16(reinterpret_cast<unsigned char *>(&points_spectr), sizeOfData);
    payloadHeader_.headerCRC = 0;//crc8(reinterpret_cast<unsigned char *>(&payloadHeader_), PAYLOAD_HEADER_LENGTH);


    QDataStream streamWrite(qSocketADC);

    streamWrite<<payloadHeader_.magic;
    streamWrite<<payloadHeader_.number;
    streamWrite<<payloadHeader_.id;
    streamWrite<<payloadHeader_.flags;
    streamWrite<<payloadHeader_.timestamp;
    streamWrite<<payloadHeader_.type;
    streamWrite<<payloadHeader_.length;
    streamWrite<<payloadHeader_.reserver;
    streamWrite<<payloadHeader_.messageCRC;
    streamWrite<<payloadHeader_.headerCRC;
    streamWrite<<aFiltr;
//  emit signalSendText(QString("Клиент бинарные данные отправил\n"));
}


/// set shift
void GraphicWidget::SendSetShift(float aShift)
{
    ZaviruhaPayloadPacketHeader payloadHeader_;
//    emit signalSendText("Клиент отправляет сдвиг\n");
    //Размер поля Data
    int sizeOfData=sizeof(aShift)*2;
//    emit signalSendText(QString("Размер aShift=%1\n").arg(sizeOfData));
    //Формирование заголовка
    payloadHeader_.magic = PAYLOAD_PREAMBULE;
    payloadHeader_.number = 0;
    payloadHeader_.id = 0;
    payloadHeader_.flags = 0;
    payloadHeader_.timestamp = 0;
    payloadHeader_.type = 102; //Установить сдвиг
    payloadHeader_.length = sizeOfData;
    payloadHeader_.reserver = 0;
    payloadHeader_.messageCRC = 0;//crc16(reinterpret_cast<unsigned char *>(&points_spectr), sizeOfData);
    payloadHeader_.headerCRC = 0;//crc8(reinterpret_cast<unsigned char *>(&payloadHeader_), PAYLOAD_HEADER_LENGTH);

    QDataStream streamWrite(qSocketADC);

    streamWrite<<payloadHeader_.magic;
    streamWrite<<payloadHeader_.number;
    streamWrite<<payloadHeader_.id;
    streamWrite<<payloadHeader_.flags;
    streamWrite<<payloadHeader_.timestamp;
    streamWrite<<payloadHeader_.type;
    streamWrite<<payloadHeader_.length;
    streamWrite<<payloadHeader_.reserver;
    streamWrite<<payloadHeader_.messageCRC;
    streamWrite<<payloadHeader_.headerCRC;
    streamWrite<<aShift;
//   emit signalSendText(QString("Клиент бинарные данные отправил\n"));
}


/// send strat command for ADC
void GraphicWidget::SendStartADC(bool aIsStart)
{
    ZaviruhaPayloadPacketHeader payloadHeader_;
//    if (aIsStart) emit signalSendText("Клиент отправляет 'Начать координатометрию'\n");
//    else emit signalSendText("Клиент отправляет 'Остановить координатометрию'\n");

    //Размер поля Data
    int sizeOfData=sizeof(aIsStart);
//    emit signalSendText(QString("Размер bool=%1\n").arg(sizeOfData));
    //Формирование заголовка
    payloadHeader_.magic = PAYLOAD_PREAMBULE;
    payloadHeader_.number = 0;
    payloadHeader_.id = 0;
    payloadHeader_.flags = 0;
    payloadHeader_.timestamp = 0;
    payloadHeader_.type = 103; //Начать(остановить координтометрию)
    payloadHeader_.length = sizeOfData;
    payloadHeader_.reserver = 0;
    payloadHeader_.messageCRC = 0;//crc16(reinterpret_cast<unsigned char *>(&points_spectr), sizeOfData);
    payloadHeader_.headerCRC = 0;//crc8(reinterpret_cast<unsigned char *>(&payloadHeader_), PAYLOAD_HEADER_LENGTH);


    QDataStream streamWrite(qSocketADC);

    streamWrite<<payloadHeader_.magic;
    streamWrite<<payloadHeader_.number;
    streamWrite<<payloadHeader_.id;
    streamWrite<<payloadHeader_.flags;
    streamWrite<<payloadHeader_.timestamp;
    streamWrite<<payloadHeader_.type;
    streamWrite<<payloadHeader_.length;
    streamWrite<<payloadHeader_.reserver;
    streamWrite<<payloadHeader_.messageCRC;
    streamWrite<<payloadHeader_.headerCRC;
    streamWrite<<aIsStart;
//    emit signalSendText(QString("Клиент бинарные данные отправил\n"));
}


/// send request freq attinuator
void GraphicWidget::SendGetFreqAtt()
{
  NovomarMsg msg;
    msg.SetAddr(0xFF);//addr);
    msg.SetSize(0x01);
    msg.SetType(0x04);
    msg.CalcCRC();
    SendMsgBuff.Add(msg);
  statSend++;
}

/// set attinuator
void GraphicWidget::SendSetAtt(unsigned char att)
{
  NovomarMsg msg;
    msg.SetAddr(0xFF);//addr);
    msg.SetSize(0x02);
    msg.SetType(0x03);
    msg.SetBody(0,att);
    msg.CalcCRC();
    SendMsgBuff.Add(msg);
  statSend++;
}

/// set second attinuator
void GraphicWidget::SendSetAtt2(unsigned char att)
{
  NovomarMsg msg;
    msg.SetAddr(0xFF);//addr);
    msg.SetSize(0x02);
    msg.SetType(0x07);
    msg.SetBody(0,att);
    msg.CalcCRC();
    SendMsgBuff.Add(msg);
  statSend++;
}


/// set freq
void GraphicWidget::SendSetFreq(unsigned short freq)
{
  NovomarMsg msg;
    msg.SetAddr(0xFF);//addr);
    msg.SetSize(0x03);
    msg.SetType(0x02);
    msg.SetBody(0,(freq>>8)&0xFF);
    msg.SetBody(1,freq&0xFF);
    msg.CalcCRC();
    SendMsgBuff.Add(msg);
  statSend++;
}

/// recv data from prm
void GraphicWidget::Protocol()
{
    QByteArray buffer;
    QByteArray send;
    NovomarMsg msgRecv;
    if(1)//stateSerial)
    {
		unsigned char byte;
		unsigned counter=0;
		unsigned numOfSend=0;
		int i;
		
        long NumOfBytesSerial = qSocketPRM->bytesAvailable();
        if(!NumOfBytesSerial)
            isFree=1;

        if(NumOfBytesSerial)
        {
/*			ttt++;
            if(ttt%2)
                SendSetFreq(0xFF, freqPRM+=10); /// Test Scan
            else
                SendGetFreqAtt(0xFF);*/
            if(NumOfBytesSerial>512) NumOfBytesSerial=512;
            buffer = qSocketPRM->read( 1544);
            if(isFree)
            {
            for(i=0; i<NumOfBytesSerial; i++)
            {
                counterIn++;
                byte=buffer[i];
                if(byte==0x55) isCtrlPresent=1;
                switch(byte)
                {
                case 0x55:
                    novomarMsg.Init();
                    isRcv=0;
                    break;
                case 0xAA: isRcv=1; novomarMsg.Init(); break; // Пакет
                case 0xA5: break; // Маркеров здесь нет!
                case 0x5A: mode5A=1; break;
                default:
                    if(isRcv==1)
                    {
                        if(mode5A)
                        {
                            novomarMsg.Add(Modify5A(byte));
                            mode5A=0;
                        }
                        else novomarMsg.Add(byte);
                    }
                }
                if(novomarMsg.IsEnd())
                {
                    RecvMsgBuff.Add(novomarMsg);
                }
            }
            }
        }
        //	else
        if(isFree)
        {
            counter=0;
            send[counter++] = 0x00;
            if(SendMsgBuff.Used())
            {
                while(SendMsgBuff.Used())
                {
                    NovomarMsg msgSend = SendMsgBuff.Get();
                    send[counter++] = 0xAA;
                    for(i=0; i<msgSend.FullSize(); i++)
                    {
                        switch(msgSend.body[i])
                        {
                        case 0x5A: send[counter++] = 0x5A; send[counter++] = 0x00; break;
                        case 0x55: send[counter++] = 0x5A; send[counter++] = 0x01; break;
                        case 0xA5: send[counter++] = 0x5A; send[counter++] = 0x02; break;
                        case 0xAA: send[counter++] = 0x5A; send[counter++] = 0x03; break;
                        default: send[counter++] = msgSend.body[i];
                        }
                    }
                    numOfSend++;
                    if(numOfSend>=2) break;
                }
                send[counter++] = 0x55;
                qSocketPRM->write( send );
                counterOut+=counter;
            }
        }
    }

//	SendGetState(0xFF);
//	SendSetFreq(0xFF,0x1234);
//	SendSetAtt(0xFF,0x57);
//	SendGetFreqAtt(0xFF);
	SendMsgBuff.Used();

    while(RecvMsgBuff.Used())
    {
        msgRecv = RecvMsgBuff.Get();
        statAll++;
        if(msgRecv.ChkCRC())
            statGood++;
        else
            statBad++;

        switch(msgRecv.Type())
        {
            case 0x01:
                break;
            case 0x02:
                statGood--;
                statGood++;
                break;
            case 0x03:
//				rcv_freq = (msgRecv.Body(0)<<8) + msgRecv.Body(1);
                break;
            case 0x04:
                m_frequ = (msgRecv.Body(0)<<8) + msgRecv.Body(1);
                m_att = msgRecv.Body(2);
                m_att2 = msgRecv.Body(4);
                (msgRecv.Body(0)<<8) + msgRecv.Body(1);
                qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, m_att*10, m_att2);
                scene->update();
                isModify=true;
                (msgRecv.Body(0)<<8) + msgRecv.Body(1);
                break;
            case 0x20:
                break;
        }
    }
}

/// scan by timer
void GraphicWidget::timerEvent(QTimerEvent *)
{
    if(isScan)
    {
        if(Time.msecsTo(QTime::currentTime())>=scanPause)
        {
            Time = QTime::currentTime();
            m_frequ += SCAN_STEP;
            if(m_frequ >= FREQ_MAX)
            {
                m_frequ=FREQ_MIN;
            }
            SendSetFreq(m_frequ);
            SendGetFreqAtt();
        }
    }
    Deal();
}

void GraphicWidget::Deal()
{
    Protocol();
}
