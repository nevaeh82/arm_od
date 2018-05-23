#ifndef PLANESCLENT_H
#define PLANESCLENT_H

#include "TCPClient.h"
#include <QElapsedTimer>
#include <QTimer>
class QSpinBox;
class QStandardItemModel;
class QProcess;
class RawDataClient;

//////////////////////////   N M E A   C L I E N T   //////////////////////////

struct PlaneInfo {
    QString id_;    //!Id самолета
    double lat_;    //!текущая широта
    double lon_;    //!текущая долгота
    double alt_;    //!текущая высота
    double speed_;  //!текущая скорость
    QElapsedTimer elapsed_timer_;
};

/**
 * @brief класс клиента, принимающего данные от самолетов через "свисток"
 */
class PlanesClient : public TCPClient
{
    Q_OBJECT
public:
	explicit PlanesClient(QObject* parent = 0);
    ~PlanesClient();
//    void SaveSettings();//!загрузить настройки
//    void LoadSettings();//!загрузить настройки

	//START or STOP THIS MAGIC
	void on_connectToDongleCheckBox_toggled(bool run,
											 QString processPath = "dump1090",
											 QString processName = "dump1090.exe",
											 QString adsbHost = "192.168.24.29",
											 uint adsbPort = 30005,
											 uint riPort = 30001,
											 uint sbsPort = 30003 );
signals:
    //!сигнал, что данные поменялись
	void signal_PlaneDataChanged( QByteArray );
    //!сигнал о том, что самолет улетел
    void signal_PlaneFlyAway( const QString deleted_id );

protected slots:
    //!пришли данные от свистка
	virtual void slot_ReadyRead();

    //!обновить полетные данные по прошествии времени
	//void slot_Update();


private slots:
    //!обработака сигнала, что данные поменялись
//   / void slot_PlaneDataChanged( const PlaneInfo& plane_info );

    //!слот, принимающий сигнал о том, что самбик улетел к ... собачьим
   // void slot_PlaneFlyAway( const QString deleted_id );

    //!отобразить данные, приходящие с ADSB
    void slot_ShowRawData( const char* data, const int len );

protected:

    //!найти колонку с самолетом по id, если нету то возвращается -1
	//int FindColumnOfPlane( const QString& id );

    void AppendColumn();//!Добавить колонку в таблицу

    //!распарсить сообщение от свистка и выбросить сигнал
    //! signal_PlaneDataChanged если данные поменялись
    bool ParseMessage(const QString& msg);
    QMap<QString,PlaneInfo> planesInfo_;//!множество самолетиков
    int traj_time_max_length_;
   // QStandardItemModel* model_;
    QTimer* timer_;
    friend class ViewerLoaderSaver;
    QProcess* dump_process_;
    RawDataClient* raw_data_getter_from_ADSB_;
    RawDataClient* raw_data_sender_to_dump_;
};

#endif // PLANESCLENT_H
