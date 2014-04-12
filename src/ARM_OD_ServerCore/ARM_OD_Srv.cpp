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

	_rpc_server = new RPCServer(this);
	_rpc_server->setRouter(_router);
	_rpc_server->start(24550, QHostAddress::Any);

	m_rpcClientR = NULL;

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

	QString tabsSettingFile = QCoreApplication::applicationDirPath();
	tabsSettingFile.append("/RPC/RPC_R_Server.ini");
	if(readSettings(tabsSettingFile) != 0){
		return;
	}

	m_rpcClientR = new RpcClientRWrapper();

	QThread* rpcClientThread = new QThread;
	connect(m_rpcClientR, SIGNAL(destroyed()), rpcClientThread, SLOT(terminate()));
	m_rpcClientR->moveToThread(rpcClientThread);
	rpcClientThread->start();

	m_rpcClientR->init(m_portRpc, QHostAddress(m_ipRpc), _router);

	_subscriber_up->add_subscription(SOLVER_SET, m_rpcClientR);
	_subscriber_up->add_subscription(SOLVER_CLEAR, m_rpcClientR);
}

int ARM_OD_Srv::readSettings(QString path_to_ini_file_RPC)
{
	int error = -1;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(path_to_ini_file_RPC, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		if(childKey.toLatin1() != "RPC_UI")
		{
			continue;
		}
		m_settings.beginGroup(childKey);

		m_ipRpc = m_settings.value("IP", "127.0.0.1").toString();
		m_portRpc = m_settings.value("Port", 24550).toInt();

		error = 0;
		m_settings.endGroup();
	}

	return error;
}
