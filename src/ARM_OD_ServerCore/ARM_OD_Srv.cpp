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
//    _tcp_controller = new TCPController(_router);
//    _router->set_tcp_controller(_tcp_controller);


//    _read_settings();


// connect up the signals
//    QObject::connect(_rpc_server, SIGNAL(finished()), &a, SLOT(quit()));
//    QObject::connect(&a, SIGNAL(aboutToQuit()), _rpc_server, SLOT(aboutToQuitApp()));

	_rpc_server = new RPCServer(this);
	_rpc_server->setRouter(_router);
	_rpc_server->start(24550, QHostAddress::Any);


//	_read_settingsKTR();

    _rpc_client1 = new RPCClient_R(_router);


    QThread *thread_rpc_client = new QThread;
    qDebug() << "create thread for rpc client ";

//    connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

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

	qRegisterMetaType<MessageSP>("MessageSP");

	m_tcpManager = new TcpManager;
	QThread* tcpManagerThread = new QThread;
	connect(tcpManagerThread, SIGNAL(finished()), m_tcpManager, SLOT(deleteLater()));
	connect(tcpManagerThread, SIGNAL(finished()), tcpManagerThread, SLOT(deleteLater()));
	m_tcpManager->moveToThread(tcpManagerThread);
	tcpManagerThread->start();

	ITcpSettingsManager* settingsManager = new TcpSettingsManager(this);
	settingsManager->setIniFile("./TCP/devices.ini");
	QMap<QString, quint32> mapInfo = settingsManager->getAllInfo();

	foreach(QString key, mapInfo.keys()) {
		log_debug(QString(key));
		m_tcpManager->addTcpDevice(key, mapInfo.value(key));
	}

	_rpc_server->registerReceiver(m_tcpManager);

	 emit signalStartRPC();

//	m_tcpManager->addTcpDevice(NIIPP_TCP_DEVICE, "192.168.10.50", 6340);
//	m_tcpManager->addTcpDevice(NIIPP_TCP_DEVICE, "192.168.245.51", 6340);
//	m_tcpManager->addTcpDevice(KTR_TCP_DEVICE, "192.168.137.98", 64300);

	/// 60.7209, 28.0289, 58.8187, 34.0933
//	m_tcpManager->addTcpDevice(AIS_TCP_DEVICE, "60.7209:28.0289:58.8187:34.0933", 0);

}

ARM_OD_Srv::~ARM_OD_Srv()
{
    signalFinishRPC();
   // delete _rpc_server;
    delete _router;
	delete _subscriber_up;

    emit closeALL();
}
