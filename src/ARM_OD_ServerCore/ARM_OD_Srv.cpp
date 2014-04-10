#include "ARM_OD_Srv.h"

ARM_OD_Srv::ARM_OD_Srv()
{

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForLocale(codec);

	qRegisterMetaType<rpc_msg> ("rpc_msg");
	qRegisterMetaType<rpc_QPointF> ("rpc_QPointF");
	qRegisterMetaType<QSharedPointer<IMessageOld> > ("QSharedPointer<IMessage>");

	qRegisterMetaType<QByteArray>("rpc_send_ba_data");

	qRegisterMetaTypeStreamOperators<QByteArray>("QByteArray");

	_router = new Router();

	_subscriber_up = new Subscriber();

	_router->set_subscriber(_subscriber_up);
	IRpcSettingsManager* rpcSettingsManager = RpcSettingsManager::instance();
	rpcSettingsManager->setIniFile("./RPC/RpcOdServer.ini");
	QString host = rpcSettingsManager->getRpcHost();
	quint16 port = rpcSettingsManager->getRpcPort().toUShort();
	_rpc_server = new RPCServer(this);
	_rpc_server->setRouter(_router);
	_rpc_server->start(port, QHostAddress(host));

	m_rpcConfigReader = new RpcConfigReader(this);
	m_rpcConfigReader->setRpcServer(_rpc_server);

	_rpc_client1 = NULL;

	qRegisterMetaType<MessageSP>("MessageSP");

	m_tcpManager = new TcpManager;
	QThread* tcpManagerThread = new QThread;
	connect(tcpManagerThread, SIGNAL(finished()), m_tcpManager, SLOT(deleteLater()));
	connect(tcpManagerThread, SIGNAL(finished()), tcpManagerThread, SLOT(deleteLater()));
	m_tcpManager->moveToThread(tcpManagerThread);
	tcpManagerThread->start();

	_rpc_server->registerReceiver(m_tcpManager);
	m_tcpManager->setRpcServer(_rpc_server);

	m_tcpSettingsManager = TcpSettingsManager::instance();
	m_tcpSettingsManager->setIniFile("./TCP/devices.ini");
	QMap<QString, quint32> mapInfo = m_tcpSettingsManager->getAllInfo();

	foreach(QString key, mapInfo.keys()) {
		log_debug(QString(key));
		m_tcpManager->addTcpDevice(key, mapInfo.value(key));
	}

	IEnviromentSettingsManager* envSettingsManager = EnviromentSettingsManager::instance();
	envSettingsManager->setIniFile("./Enviroment/armr_connection.ini");
	quint32 armrConnectionType = envSettingsManager->getArmrConnectionMode().toUInt();

	switch (armrConnectionType) {
		case 0:
			/// Tcp connection mode
			addTcpArmrConnection();
			break;
		case 1:
			/// Rpc connection mode
			addRpcArmrConnection();
			break;
		default:
			break;
	}

	emit signalStartRPC();
}

ARM_OD_Srv::~ARM_OD_Srv()
{
	signalFinishRPC();
	emit closeALL();
}

void ARM_OD_Srv::addTcpArmrConnection()
{
	log_debug("Adding tcp connection to armr...");
	m_tcpSettingsManager->setIniFile("./TCP/tcpArmrDevice.ini");
	QMap<QString, quint32> mapInfo = m_tcpSettingsManager->getAllInfo();
	foreach(QString key, mapInfo.keys()) {
		log_debug(QString(key));
		m_tcpManager->addTcpDevice(key, mapInfo.value(key));
	}
}

void ARM_OD_Srv::addRpcArmrConnection()
{
	log_debug("Adding rpc connection to armr...");

	_rpc_client1 = new RPCClient_R(_router);

	QThread* thread_rpc_client = new QThread;
	log_debug("create thread for rpc client");

	connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
	connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
	connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

	connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
	connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
	connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));

	_rpc_client1->setParent(0);
	_rpc_client1->moveToThread(thread_rpc_client);

	_subscriber_up->add_subscription(SOLVER_SET, _rpc_client1);
	_subscriber_up->add_subscription(SOLVER_CLEAR, _rpc_client1);

	thread_rpc_client->start();

}
