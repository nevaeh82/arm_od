#include "RPCClient.h"

#include <QDebug>

const double m_zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
                       5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
                       18, 20, 22, 24, 28, 30};
const double m_zoneDir[28] = {2.5, 3, 4, 5,
                       6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
                       22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

RPCClient::RPCClient(Station *station, IDbBlaManager *db_manager, IDBManager* db_manager_targer,
					 IMapController* map_controller, ITabMap* parent_tab, ITabManager* tab_manager, QObject *parent)
	: RpcClientBase(parent)
{
	m_tabManager = tab_manager;
	m_solverAuto = false;
	m_mapController = map_controller;
	m_parentTab = parent_tab;
	m_station = station;
	m_dbBlaManager = db_manager;
	m_dbManagerTarget = db_manager_targer;
	m_pelengEvilIds = 0;
	m_rdsEvilIds = 50;
}

RPCClient::~RPCClient()
{
}

bool RPCClient::start(quint16 port, QHostAddress ipAddress)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(slotSetCommand(IMessageOld*)));

	m_clientPeer->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetNIIPPBPLA(QByteArray)), RPC_SLOT_SET_NIIPP_BPLA);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolverData(QByteArray)), RPC_SLOT_SET_SOLVER_DATA);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolverDataClear(QByteArray)), RPC_SLOT_SET_SOLVER_CLEAR);

	connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
	connect(m_clientPeer, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));
	qDebug() << this->thread();

	///server
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BLA_POINTS, this, SLOT(rpcSendBlaPoints(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_AIS_DATA, this, SLOT(rpcSlotServerSendAisData(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS, this, SLOT(rpcSendBplaPoints(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, this, SLOT(rpcSendBplaPointsAuto(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_NIIPP_DATA, this, SLOT(rpcSendNiippData(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, this, SLOT(rpcSlotServerSendAtlantDirection(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_POSITION, this, SLOT(rpcSlotServerSendAtlantPosition(QByteArray)));

	log_debug("Start RPCClient");
	return RpcClientBase::start(port, ipAddress);
}

void RPCClient::setCommand(IMessageOld *msg)
{
    emit signalSetCommand(msg);
}

void RPCClient::slotSetCommand(IMessageOld *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RPCClient::formCommand(IMessageOld *msg)
{
    qDebug() << "form command";
    QByteArray data;
    int type = msg->get(data);
    switch(type)
    {
    case COMMAND_SET_NIIPP_BPLA:
		sendNiippBpla(data);
        break;

    case COMMAND_SET_SOLVER:
		sendDataToSovler(data);
        break;
    case COMMAND_SET_SOLVER_AUTO:
		setSolverAuto(data);
        break;

    case COMMAND_SET_SOLVER_CLEAR:
		setSolverClear(data);
        break;

    default:
        break;
    }
    msg->clenup();
}

void RPCClient::setSolverAuto(QByteArray ba)
{
    QDataStream ds(&ba, QIODevice::ReadOnly);
	ds >> m_solverAuto;
}

/// slot when connection complete
void RPCClient::slotRCPConnetion()
{
    qDebug() << "Connection complete!";
	emit signalSetClientId(m_station->id);
}

void RPCClient::rpcSendBlaPoints(QByteArray data)
{
	int blaId = m_dbBlaManager->getBlaByBlaId(id);
	if (blaId < 0){
		int blaUnknownTypeId = m_dbBlaManager->getBlaTypeByName("UnknownBlaType");

		if (blaUnknownTypeId < 0){
			BlaType blaType;
			blaType.name = "UnknownBlaType";
			blaUnknownTypeId = m_dbBlaManager->addBlaType(blaType);
		}

		Bla bla;
		bla.blaId = id;
		bla.type = blaUnknownTypeId;

		blaId = m_dbBlaManager->addBla(bla);
	}

	int statusUnknownId = m_dbBlaManager->getStatusByName("UnknownStatus");
	int deviceUnknownId = -1;

	QList<Devices> devices;
	int unknownDeviceTypeId = m_dbBlaManager->getDeviceTypeByName("UnknownDeviceType");

	if (unknownDeviceTypeId < 0){
		DeviceType deviceType;
		deviceType.name = "UnknownDeviceType";
		unknownDeviceTypeId = m_dbBlaManager->addDeviceType(deviceType);
	}

	if (!m_dbBlaManager->getDevicesByType(unknownDeviceTypeId, devices)){
		Devices device;
		device.blaId = id;
		device.deviceId = unknownDeviceTypeId;
		deviceUnknownId = m_dbBlaManager->addDevice(device);
	} else {
		deviceUnknownId = devices.at(0).id;
	}

	BlaInfo blaInfo;
	blaInfo.blaId = blaId; // FK
	blaInfo.device = deviceUnknownId; // FK
	blaInfo.lat = point.x();
	blaInfo.lon = point.y();
	blaInfo.alt = alt;
	blaInfo.speed = speed;
	blaInfo.yaw = course;
	blaInfo.restTime = QTime(1, 0);
	blaInfo.statusId = statusUnknownId; // FK
	blaInfo.dateTime = QDateTime::currentDateTime();

//	debug("Get data from server");
	QDataStream inputDataStream(&data, QIODevice::ReadOnly);
	QVector<UAVPositionData> positionDataVector;

	inputDataStream >> positionDataVector;

	if (positionDataVector.size() < 1) {
		debug("Size uavpositiondata vector < 1");
		return;
	}

	/// Now we take first point, but we need to take more than 1 point
	UAVPositionData positionData = positionDataVector.at(0);

	int id = positionData.boardID; /// need quint16
	QPointF point;
	point.setX(positionData.latitude);
	point.setY(positionData.longitude);
	double alt = positionData.altitude;
	double speed = positionData.speed;
	double course = positionData.course;
	quint32 state = positionData.state;    QByteArray ddd;
    QDataStream ds(&ddd, QIODevice::WriteOnly);
    ds << point;
    ds << alt;
    ds << speed;
    ds << course;
    ds << state;

	m_mapController->get_map_client(1)->slot_add_BLA(id, ddd);
	/*QMap<QString, QVariant>* rec = new QMap<QString, QVariant>;

    rec->insert("id", QVariant::fromValue(id));

    rec->insert("pid", QVariant::fromValue(0));
    rec->insert("name", QVariant::fromValue(id));
    rec->insert("state", QVariant::fromValue(1));
	m_dbBlaManager->set(0, rec);*/

   /* QMap<QString, QVariant>* rec_p = new QMap<QString, QVariant>;

    QString s_prop;
    s_prop = tr("Latitude");
    rec_p->insert("pid", QVariant::fromValue(id));
    rec_p->insert("name", QVariant::fromValue(s_prop));
    rec_p->insert("value", QVariant::fromValue(point.x()));
    rec_p->insert("state", QVariant::fromValue(1));

	QVector<QMap<QString, QVariant> >* map_p = m_dbBlaManager->get(id, 0);
    for(int i = 0; i < map_p->count(); ++i)
    {
        QString nam = map_p->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p->insert("id", QVariant::fromValue(map_p->at(i).value("id").toInt()));
            break;
        }
	}

	m_dbBlaManager->set_property(0, rec_p);*/

	/*QMap<QString, QVariant>* rec_p1 = new QMap<QString, QVariant>;

    s_prop = tr("Longitude");
    rec_p1->insert("pid", QVariant::fromValue(id));
    rec_p1->insert("name", QVariant::fromValue(s_prop));
    rec_p1->insert("value", QVariant::fromValue(point.y()));
    rec_p1->insert("state", QVariant::fromValue(1));

	QVector<QMap<QString, QVariant> >* map_p1 = m_dbBlaManager->get(id, 0);

    for(int i = 0; i < map_p1->count(); ++i)
    {
        QString nam = map_p1->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p1->insert("id", QVariant::fromValue(map_p1->at(i).value("id").toInt()));
            break;
        }
    }

	m_dbBlaManager->set_property(0, rec_p1);*/


   /* QMap<QString, QVariant>* rec_p2 = new QMap<QString, QVariant>;

	s_prop = tr("Altitude");
    rec_p2->insert("pid", QVariant::fromValue(id));
    rec_p2->insert("name", QVariant::fromValue(s_prop));
    rec_p2->insert("value", QVariant::fromValue(alt));
	rec_p2->insert("state", QVariant::fromValue(1));

	QVector<QMap<QString, QVariant> >* map_p2 = m_dbBlaManager->get(id, 0);

    for(int i = 0; i < map_p2->count(); ++i)
    {
        QString nam = map_p2->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p2->insert("id", QVariant::fromValue(map_p2->at(i).value("id").toInt()));
            break;
        }
    }

	m_dbBlaManager->set_property(0, rec_p2);*/
}

void RPCClient::rpcSlotServerSendAisData(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    int id;
    ds >> id;
	QMap<int, QVector<QString> > map1;
    ds >> map1;

	m_mapController->get_map_client(1)->slot_add_ais(map1);

}

void RPCClient::rpcSendBplaPoints(QByteArray data)
{
	if(m_solverAuto == false)
    {
		sendBplaPoints(data);
    }

}

void RPCClient::rpcSendBplaPointsAuto(QByteArray data)
{
	if(m_solverAuto == true)
    {
		sendBplaPoints(data);
    }

}

void RPCClient::rpcSendNiippData(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);

    QDateTime dt;
    QTime time;
    int mode;
    QPointF point;
    QString NS;
    QString EW;
    int alt;
    int zone;
    int course;
    int angle;

    int id;

    ds >> id;

    ds >> dt;
    ds >> time;
    ds >> mode;
    ds >> point;
    ds >> NS;
    ds >> EW;
    ds >> alt;
    ds >> zone;
    ds >> course;
    ds >> angle;

    QByteArray ba;
    QDataStream ds1(&ba, QIODevice::WriteOnly);

    QPointF latlon;
    if(id == 100)
    {
        latlon.setX(42.511183);
        latlon.setY(41.6905);
    }
    if(id ==101)
    {
        latlon.setX(42.634183);
        latlon.setY(41.912167);
    }

    QString name = QString::number(id) + " - niipp";
    double wid = 25;
    ds1 << name;
    ds1 << latlon;
    ds1 << wid;

    if(mode == 01)
    {
		m_mapController->get_map_client(1)->slot_niipp_power_cicle(id, m_zone[zone], ba);
    }

    if(mode == 10)
    {
		m_mapController->get_map_client(1)->slot_update_sector(id, m_zoneDir[zone], course, NULL);
    }

	qDebug() << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMM = " << mode << course << zone;

    QByteArray ba1;
    QDataStream ds2(&ba1, QIODevice::WriteOnly);
    ds2 << mode;
	m_tabManager->send_data_niipp_control(id, ba1);

}

void RPCClient::rpcSlotServerSendAtlantDirection(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadWrite);
    A_Dir_Ans_msg msg;
    ds >> msg.requestId;
    ds >> msg.sourceId;
    ds >> msg.dateTime;
    ds >> msg.post;
    ds >> msg.postLatitude;
    ds >> msg.postLongitude;
    ds >> msg.postHeight;
    ds >> msg.frequency;
    ds >> msg.widht;
    ds >> msg.direction;
    ds >> msg.angle;
    ds >> msg.level;
    ds >> msg.quality;
    ds >> msg.motionType;
    ds >> msg.motionConfidence;

    int id_post = msg.post.right(1).toInt();

	m_mapController->get_map_client(1)->slot_peleng(msg.sourceId, id_post, msg.postLatitude, msg.postLongitude, msg.direction);
}

void RPCClient::rpcSlotServerSendAtlantPosition(QByteArray data)
{
//    qDebug() << "GOT DATA FROM ATLANT!";
    QDataStream ds(&data, QIODevice::ReadWrite);
    A_Pos_Ans_msg msg;
    ds >> msg.requestId;
    ds >> msg.sourceId;
    ds >> msg.dateTime;
    ds >> msg.latitude;
    ds >> msg.longitude;
    ds >> msg.quality;

	if(!m_mapPelengEvilIds.contains(msg.sourceId))
    {
		m_mapPelengEvilIds.insert(msg.sourceId, ++m_pelengEvilIds);
		if(m_pelengEvilIds > 49)
        {
			m_pelengEvilIds = 0;
			m_mapPelengEvilIds.clear();
        }
    }
    QPointF point(msg.longitude, msg.latitude);

    QByteArray ba1;
    QDataStream ds1(&ba1, QIODevice::WriteOnly);
    ds1 << point;

	qDebug() << "ID = " << m_mapPelengEvilIds.size() << m_pelengEvilIds << m_mapPelengEvilIds.value(msg.sourceId);

	m_mapController->get_map_client(1)->slot_add_evil(m_mapPelengEvilIds.value(msg.sourceId), ba1);

}

void RPCClient::sendNiippBpla(QByteArray data)
{
    emit signalSetNIIPPBPLA(data);
}



void RPCClient::setSolverClear(QByteArray data)
{
    emit signalSetSolverDataClear(data);
}

void RPCClient::sendDataToSovler(QByteArray data)
{
    emit signalSetSolverData(data);
}

void RPCClient::sendBplaPoints(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    QTime t;
    int state;
    QPointF pointStdDev;
    double speed;
    double alt;
    double course;
    QVector<QPointF> track;
    ds >> t;
    ds >> state;
    ds >> pointStdDev;
    ds >> track;
    ds >> speed;
    ds >> alt;
    ds >> course;

	if(m_rdsEvilIds > 99)
    {
		m_rdsEvilIds = 50;
    }
	m_mapController->get_map_client(1)->slot_add_evil(m_rdsEvilIds, data);

    QMap<QString, QVariant>* rec = new QMap<QString, QVariant>;

	rec = m_dbManagerTarget->get_bpla_fields(m_rdsEvilIds);
    if(rec->count() == 0)
    {
		rec->insert("id", QVariant::fromValue(m_rdsEvilIds));
        rec->insert("pid", QVariant::fromValue(0));
		rec->insert("name", QVariant::fromValue(m_rdsEvilIds));
        rec->insert("state", QVariant::fromValue(0));
		m_dbManagerTarget->set(1, rec);
    }

    QMap<QString, QVariant>* rec_p = new QMap<QString, QVariant>;

    QString s_prop;
    s_prop = tr("Latitude");
	rec_p->insert("pid", QVariant::fromValue(m_rdsEvilIds));
    rec_p->insert("name", QVariant::fromValue(s_prop));
    rec_p->insert("value", QVariant::fromValue(track.at(track.size()-1).x()));
    rec_p->insert("state", QVariant::fromValue(1));

	QVector<QMap<QString, QVariant> >* map_p = m_dbManagerTarget->get(m_rdsEvilIds, 1);
    for(int i = 0; i < map_p->count(); ++i)
    {
        QString nam = map_p->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p->insert("id", QVariant::fromValue(map_p->at(i).value("id").toInt()));
            break;
        }
	}

	m_dbManagerTarget->set_property(1, rec_p);

    QMap<QString, QVariant>* rec_p1 = new QMap<QString, QVariant>;

    s_prop = tr("Longitude");
	rec_p1->insert("pid", QVariant::fromValue(m_rdsEvilIds));
    rec_p1->insert("name", QVariant::fromValue(s_prop));
    rec_p1->insert("value", QVariant::fromValue(track.at(track.size()-1).y()));
    rec_p1->insert("state", QVariant::fromValue(1));

	//TODO: shouldnt m_dbManager be m_dbManagerTarget?
	//QVector<QMap<QString, QVariant> >* map_p1 = m_dbManager->get(m_rdsEvilIds, 1);
	QVector<QMap<QString, QVariant> >* map_p1 = m_dbManagerTarget->get(m_rdsEvilIds, 1);

    for(int i = 0; i < map_p1->count(); ++i)
    {
        QString nam = map_p1->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p1->insert("id", QVariant::fromValue(map_p1->at(i).value("id").toInt()));
            break;
        }
    }

	m_dbManagerTarget->set_property(1, rec_p1);


    QMap<QString, QVariant>* rec_p2 = new QMap<QString, QVariant>;

	s_prop = tr("Altitude");
	rec_p2->insert("pid", QVariant::fromValue(m_rdsEvilIds));
    rec_p2->insert("name", QVariant::fromValue(s_prop));
    rec_p2->insert("value", QVariant::fromValue(alt));
    rec_p2->insert("state", QVariant::fromValue(1));

	QVector<QMap<QString, QVariant> >* map_p2 = m_dbManagerTarget->get(m_rdsEvilIds, 1);

    for(int i = 0; i < map_p2->count(); ++i)
    {
        QString nam = map_p2->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p2->insert("id", QVariant::fromValue(map_p2->at(i).value("id").toInt()));
            break;
        }
    }

	m_dbManagerTarget->set_property(1, rec_p2);
}
