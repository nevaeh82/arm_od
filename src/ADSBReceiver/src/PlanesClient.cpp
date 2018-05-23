#include "PlanesClient.h"
#include <QDateTime>
#include <QStandardItemModel>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "BullionClient.h"
#include "RawDataClient.h"
#include "Isnan.h"
#include <QStandardItemModel>

#include <limits>


//////////////////////////   К О Н С Т Р У К Т О Р   //////////////////////////

PlanesClient::PlanesClient( QObject *parent ) :
    TCPClient(parent)
  , traj_time_max_length_(60)
  , timer_( new QTimer() )
  , dump_process_(0) {

	bool ans;
//    bool ans =   connect( timer_, SIGNAL( timeout()),
//                          this, SLOT( slot_Update()) );

//    ans = ans && connect( this, SIGNAL( signal_PlaneDataChanged(PlaneInfo) ),
//                          this, SLOT( slot_PlaneDataChanged(PlaneInfo)),
//                          Qt::DirectConnection );
//    ans = ans && connect( this, SIGNAL( signal_PlaneFlyAway(QString)),
//                          this, SLOT( slot_PlaneFlyAway(QString)) );
	//Q_ASSERT_X( ans,__FUNCTION__,"Не удалось создать внутрений коннкет" );

//	model_ = new QStandardItemModel(6,0, 0); //6 Rows and 0 Columns
//    model_->setVerticalHeaderItem(0, new QStandardItem("Id"));
//    model_->setVerticalHeaderItem(1, new QStandardItem("Широта (гр.)"));
//    model_->setVerticalHeaderItem(2, new QStandardItem("Долгота (гр.)"));
//    model_->setVerticalHeaderItem(3, new QStandardItem("Высота (м.)"));
//    model_->setVerticalHeaderItem(4, new QStandardItem("Скор. (км/ч)"));
//    model_->setVerticalHeaderItem(5, new QStandardItem("тик, с."));
	//this->AppendColumn();

	QStringList param;
	param.append("/F");
	param.append("/IM");
	param.append("dump1090.exe");
	QProcess::execute("taskkill", param);

    timer_->start(1000);

    raw_data_getter_from_ADSB_ = new BullionClient(this);
    raw_data_sender_to_dump_ = new RawDataClient(this);
    ans = connect( raw_data_getter_from_ADSB_, SIGNAL(signal_DataHasCame(const char*, int)),
                   raw_data_sender_to_dump_ , SLOT(slot_SendRawData(const char*, int)) );
    ans = ans && connect( raw_data_getter_from_ADSB_, SIGNAL(signal_DataHasCame(const char*, int)),
                          this,                       SLOT(slot_ShowRawData(const char*, int)) );
    ans = ans && connect( raw_data_getter_from_ADSB_, SIGNAL(signal_SendMessage(QString)),
                          this,                       SIGNAL(signal_SendMessage(QString)) );
    ans = ans && connect( raw_data_sender_to_dump_, SIGNAL(signal_SendMessage(QString)),
                          this,                     SIGNAL(signal_SendMessage(QString)) );
    Q_ASSERT(ans);
	//this->LoadSettings();
}


//////////////////////   S L O T   R E A D Y   R E A D   //////////////////////

void PlanesClient::slot_ReadyRead() {
    QDataStream in(m_pTcpSocket);
    const int max_len = 10000, min_len = 10;
    int bytes_available;
    char buf [max_len] ;
    int ans, len;
    for (;;) {
        bytes_available = m_pTcpSocket->bytesAvailable();
        if (bytes_available < min_len * sizeof(char)) {
            break;
        }
        if(bytes_available > max_len * sizeof(char)) {
            bytes_available = max_len * sizeof(char);
        }
        ans = in.readRawData(buf,bytes_available);
        len = strlen(buf);
        if( ans==-1 || len < min_len ) {
            break;
        }
        QString str = QString::fromLatin1(buf,len);
		this->ParseMessage(str.mid( 0,str.lastIndexOf(",")+2 ));
    }
}


////////////////////////   P A R S E   M E S S A G E   ////////////////////////

//!распарсить сообщение от свистка и выбросить сигнал
//! signal_PlaneDataChanged если данные поменялись
bool PlanesClient::ParseMessage(const QString& msg) {
    emit signal_SendMessage(QString("\n")+msg);
    QStringList list = msg.split(",");
    int siz_list = list.size();
    if( siz_list<16 ) {
        return false;
    }
    QString msg_type = list[0];
    if( msg_type!="MSG" ) {
        return true;
    }
    PlaneInfo current_info;
    QString id = list[4];
    bool this_is_new_plane = planesInfo_.find(id) == planesInfo_.end();
    if( !this_is_new_plane ) {
        current_info = planesInfo_[id];
    }
    else {
        current_info.id_ = id;
        current_info.elapsed_timer_.start();
    }
    QDateTime date_time( QDate::fromString(list[8],"yyyy/MM/dd"),
                         QTime::fromString(list[9]) );
    if( !date_time.isValid() ) {
        return false;
    }
    bool ok1,ok2,ok3,ok4;
    double count_of_feets_in_one_meter = 3.2808399;
    double count_kilometers_in_nautical_mile = 1.852;
    double alt = list[11].toDouble(&ok1)/count_of_feets_in_one_meter;
    double lat = list[14].toDouble(&ok2);
    double lon = list[15].toDouble(&ok3);
    double speed = list[12].toDouble(&ok4)*count_kilometers_in_nautical_mile;
    double nan = std::numeric_limits<double>::quiet_NaN();
    if(!ok1) {
        alt=nan;
    }
    if(!ok2) {
        lat=nan;
    }
    if(!ok3) {
        lon=nan;
    }
    if(!ok4) {
        speed=nan;
    }
    if(ok1||ok2||ok3||ok4) {
        current_info.lat_ = lat;
        current_info.lon_ = lon;
        current_info.alt_ = alt;
        current_info.speed_ = speed;
        planesInfo_.insert( current_info.id_, current_info );

		QByteArray dataToSend;
		QDataStream ds(&dataToSend, QIODevice::WriteOnly);

		ds << current_info.id_;
		ds << current_info.lon_;
		ds << current_info.lat_;
		ds << current_info.alt_;
		ds << current_info.speed_;

		emit signal_PlaneDataChanged(dataToSend);
		qDebug() << "Incoming Plane " << current_info.id_;
        return true;
    }
    return false;
}


///////////////////   P L A N E   D A T A   C H A N G E D   ///////////////////

//!обработака сигнала, что данные поменялись
//void PlanesClient::slot_PlaneDataChanged( const PlaneInfo& plane_info ) {
//    int col_number = this->FindColumnOfPlane(plane_info.id_);
//    if( col_number < 0 || col_number > model_->columnCount()-1 ) {
//        col_number = model_->columnCount();
//        this->AppendColumn();
//    }
//    model_->setData( model_->index(0, col_number ), plane_info.id_ );
//    if(!std::isnan( plane_info.lat_ )) {
//        model_->setData( model_->index( 1, col_number ),
//                         QString::number( plane_info.lat_, 'g', 10 ) );
//    }
//    if(!std::isnan( plane_info.lon_ )) {
//        model_->setData( model_->index( 2, col_number ),
//                         QString::number( plane_info.lon_, 'g', 10 ) );
//    }
//    if(!std::isnan( plane_info.alt_ )) {
//        model_->setData( model_->index( 3, col_number ),
//                         QString::number( plane_info.alt_ ) );
//    }
//    if(!std::isnan( plane_info.speed_ )) {
//        model_->setData( model_->index( 4, col_number ),
//                         QString::number( plane_info.speed_ ) );
//    }
//}

/////////////////   F I N D   C O L U M N   O F   P L A N E   /////////////////

//int PlanesClient::FindColumnOfPlane( const QString& id ) {
//    int colCount = model_->columnCount();
//    QString txt;
//    for ( int i =0; i<colCount; i++ ) {
//        txt = model_->item(0,i)->text();
//        if( txt == "" || txt == id    ) {
//            return i;
//        }
//    }
//    return -1;
//}

////////////////////////   A P P E N D   C O L U M N   ////////////////////////

//void PlanesClient::AppendColumn( ) {
//    int new_col_ind = model_->columnCount();
//    model_->setHorizontalHeaderItem( new_col_ind, new QStandardItem
//                                     (QString("Борт %1").arg(new_col_ind+1)) );
//    Q_ASSERT(new_col_ind+1==model_->columnCount());
//    for ( int i = 0; i < model_->rowCount(); ++i ) {
//        model_->setData( model_->index( i, new_col_ind ),
//                         Qt::AlignCenter, Qt::TextAlignmentRole );
//        Q_ASSERT( model_->item(i,new_col_ind)->text()=="" );
//    }
//}


///////////////////////   К Н О П К А   К О Н Н Е К Т   ///////////////////////

void PlanesClient::on_connectToDongleCheckBox_toggled(bool run, QString processPath,
																QString processName,
																QString adsbHost,
																uint adsbPort,
																uint riPort,
																uint sbsPort) {
	if(run) {
		QDir dump_dir( QDir::currentPath() + QDir::separator() + processPath + QDir::separator() + processName );

		QString dt = dump_dir.absoluteFilePath("");

        if( dump_process_ == 0 ) {
            dump_process_ = new QProcess(this);
        }

        QStringList Arg;
        Arg = QStringList() << "--interactive" << "--net-only" << "--net-http-port"
                            << "8081" << "--net-ro-size" << "500" << "--net-ro-rate"
                            << "5" << "--net-buffer" << "5" << "--mlat"
							<< "--net-ri-port" << QString::number(riPort) << "--net-sbs-port" <<  QString::number(sbsPort);

        // dump1090.exe --interactive --net-only --net-http-port 8081 --net-ro-size 500 --net-ro-rate 5 --net-buffer 5 --mlat --net-ri-port 30001 --net-sbs-port 30003

		dump_process_->start( dt, Arg );
        dump_process_->waitForStarted();
        qDebug() << dump_process_->readAllStandardOutput();
		this->SetAddress( "localhost", sbsPort );
		raw_data_getter_from_ADSB_->slot_ConnectToHost( adsbHost, adsbPort );
		raw_data_sender_to_dump_->slot_ConnectToHost("localhost", riPort );
    }
    else {
        this->Disconnect( );
		QStringList param;
		param.append("/F");
		param.append("/IM");
		param.append(processName);
		QProcess::execute("taskkill", param);
		//dump_process_->kill();
        raw_data_getter_from_ADSB_->slot_Disconnect();
        raw_data_sender_to_dump_->slot_Disconnect();
    }
}


//////////////////////////   S L O T   U P D A T E   //////////////////////////

//!обновить полетные данные по прошествии времени
//void PlanesClient::slot_Update() {
//    QString id;
//    for ( int n =0; n<model_->columnCount(); n++ ) {
//        id = model_->item(0, n)->text();
//        if(planesInfo_.find(id)==planesInfo_.end()) {
//            if(id!="") {
//                qDebug()<<QString("\nВ ТАБЛИЦЕ НЕСУЩЕСТВУЮЩИЙ БОРТ id = %1!!!\n").arg(id);
//            }
//            continue;
//        }
//        PlaneInfo& i = planesInfo_[id];
//        int elapsed_secs =i.elapsed_timer_.elapsed()/1000;
//        if( elapsed_secs <= traj_time_max_length_ ) {
//            model_->setData( model_->index(5, n ),QString::number(elapsed_secs) );
//        } else {
//            emit signal_PlaneFlyAway(i.id_);
//        }
//    }

//}


//////////////////   S L O T   P L A N E   F L Y   A W A Y   //////////////////

//!слот, принимающий сигнал о том, что самбик улетел к ... собачьим
//void PlanesClient::slot_PlaneFlyAway( const QString deleted_id ) {
//    int deleted_col_number = this->FindColumnOfPlane(deleted_id);
//    if( deleted_col_number==-1 ) {
//        return;
//    }
//    planesInfo_.remove(deleted_id);
//    model_->removeColumn(deleted_col_number);
//    if(model_->columnCount()==0) {
//        AppendColumn();
//    }
//    else {
//        int col_count = model_->columnCount();
//        for (int i = deleted_col_number; i<col_count; i++) {
//            model_->setHorizontalHeaderItem
//                ( i, new QStandardItem(QString("Борт %1").arg(i+1)) );
//        }
//    }
//    QStandardItemModel* new_model = new QStandardItemModel( model_->rowCount(),
//                                             model_->columnCount()) ;
//    delete new_model;
//}


///////////////////////////   Д Е С Т Р У К Т О Р   ///////////////////////////

PlanesClient::~PlanesClient() {
	//this->SaveSettings();
}


////////////////////////   S A V E   S E T T I N G S   ////////////////////////

////!загрузить настройки
//void PlanesClient::SaveSettings() {
//    QFile out_file( "PlanesClientState.dat" );
//    if(!out_file.open(QFile::WriteOnly)) {
//        QMessageBox().critical( 0, "Не удается сохранить настройки",
//                                "Не открыть файл \"PlanesClientState.dat\"" );
//        return;
//    }
//    QDataStream data_stream(&out_file);
//    //загружаем ip-адрес сервера
//    data_stream << ui->ipAdressTextEdit->toPlainText();
//    data_stream << ui->portNumberSpinBox->value();
//    //загружаем номер порта сервера координат
//    data_stream << ui->connectToDongleCheckBox->isChecked();
//    out_file.close();
//}


//////////////////////////   L O A D   S E T T I N G S   ////////////////////////

////!загрузить настройки
//void PlanesClient::LoadSettings() {
//    //Открываем файл
//    QFile out_file( "PlanesClientState.dat" );
//    if(!out_file.open(QFile::ReadOnly)) {
//        QMessageBox().critical( 0, "Не удается загрузить настройки",
//                                "Не открыть файл \"PlanesClientState.dat\"" );
//        return;
//    }
//    QDataStream data_stream(&out_file);
//    bool bool_val;
//    int int_val;
//    QString str;
//    //загружаем ip-адрес сервера
//    data_stream >> str; ui->ipAdressTextEdit->setText(str);
//    //загружаем номер порта сервера координат
//    data_stream >> int_val; ui->portNumberSpinBox->setValue( int_val );
//    data_stream >> bool_val; ui->connectToDongleCheckBox->setChecked(bool_val);
//    out_file.close();
//}


void PlanesClient::slot_ShowRawData(const char *data, const int len) {
    emit signal_SendMessage(QString(QByteArray( data, len )));
}
