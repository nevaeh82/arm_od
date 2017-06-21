#include <QMessageBox>

#include "MainWindowController.h"

#include "Interfaces/IUavHistoryListener.h"

MainWindowController::MainWindowController(QObject *parent) :
	QObject(parent),
	m_solverSetupWidgetController(NULL)
{
	m_view = NULL;
	m_tabManager = NULL;
	m_rpcConfigClient = NULL;
	m_uavLifeTime = 0;
	m_serverHandler = 0;
	m_ping = true;

	qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
	qRegisterMetaType<quint32>("quint32");

	qRegisterMetaType<Uav>( "Uav" );
	qRegisterMetaType<UavInfo>( "UavInfo" );
	qRegisterMetaType<Niipp>( "Niipp" );

	qRegisterMetaType<IUavHistoryListener::Status>( "Status" );

	resetServer();

	QString serverName = "./" + QString(SERVER_NAME);
	m_serverHandler = new SkyHobbit::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);

	m_serverHandler->start(true);

	connect(m_serverHandler, SIGNAL(signalProcessDead()), this, SLOT(slotServerProcessDead()));


	m_rpcCheckTimer = new QTimer(this);
	connect(m_rpcCheckTimer, SIGNAL(timeout()), this, SLOT(slotCheckRpc()));

	m_rpcCheckTimer->start(30000);
}

MainWindowController::~MainWindowController()
{
	m_serverHandler->terminate();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

	init();
}

void MainWindowController::setUavLifeTime(int msecs)
{
	if( m_tabManager != NULL ) {
		m_tabManager->setUavLifeTime( msecs );
	} else {
		m_uavLifeTime = msecs;
	}
}

void MainWindowController::init()
{
	m_view->getStackedWidget()->setCurrentIndex(1);

	m_tabManager = new TabManager(m_view->getMainTabWidget(), this);
	m_tabManager->setViewMenu(m_view->getViewMenu());

	if( m_uavLifeTime > 0 ) m_tabManager->setUavLifeTime( m_uavLifeTime );

	connect(m_tabManager, SIGNAL(readyToStart()), this, SLOT(startTabManger()));

	connect(m_view, SIGNAL(openAtlasSignal()), m_tabManager, SIGNAL(openAtlasSignal()));
	connect(m_view, SIGNAL(openMapSignal()), m_tabManager, SIGNAL(openMapSignal()));

	connect(m_view, SIGNAL(signalClearMap()), m_tabManager, SIGNAL(signalClearMap()));
	connect(m_view, SIGNAL(signalLoadObjects()), m_tabManager, SIGNAL(signalLoadObjects()));
	connect(m_view, SIGNAL(signalSaveObjects()), m_tabManager, SIGNAL(signalSaveObjects()));

	connect(m_tabManager, SIGNAL( mapOpened() ), m_view, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( atlasOpened() ), m_view, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( mapOpened() ), this, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( atlasOpened() ), this, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( cancelMapOpen() ), m_view, SLOT( cancelMapOpen() ));
	connect(m_view, SIGNAL(setupKoordinatometriyaSignal()), this, SLOT(solverDialogSlot()));
	connect(m_view, SIGNAL(signalResetServer()), this, SLOT(resetServer()));
	connect(m_view, SIGNAL(signalEnableAdsb(bool)), this, SLOT(enableAdsbClient(bool)));
	connect(m_view, SIGNAL(signalEnableOnlineAdsb(bool)), this, SLOT(enableAdsbOnlineClient(bool)));
	connect(m_view, SIGNAL(signalOnExtraBoardInfo(int)), m_tabManager, SIGNAL(signalOnExtraBoardInfo(int)));

	m_rpcConfigClient = new RpcConfigClient(this);
	m_rpcConfigClient->registerReceiver(this);

	// create solver settings dialog controller
	solverSettingsController = new SolverSettingsDialogController(701, m_tabManager, this);

	m_solverSetupWidgetController = new SolverSetupWidgetController(m_tabManager, this);
	m_solverSetupWgt = new SolverSetupWidget(m_view);
	m_solverSetupWidgetController->appendView( m_solverSetupWgt );

	m_tabManager->addSolverSetupController( m_solverSetupWidgetController );

	connect(m_view, SIGNAL(signalSolverCommandsDialog()), m_solverSetupWgt, SLOT(show()));

	if(m_tabManager->getMapWidget()) {
		connect(m_view, SIGNAL(signalApply(int)), m_tabManager->getMapWidget(), SIGNAL(signalApply(int)));
		connect(m_view, SIGNAL(signalClear()), m_tabManager->getMapWidget(), SIGNAL(signalClear()));
	}

	serverStartedSlot();
}

void MainWindowController::serverFailedToStartSlot()
{
	QMessageBox::critical(m_view, tr("Unable to connect to server"), tr("Please try to restart application"));
}

void MainWindowController::serverStartedSlot()
{
	m_view->getStackedWidget()->setCurrentIndex(1);

	log_debug("go to sleep");
	QEventLoop loop;
	QTimer timer;
	connect(&timer, SIGNAL(timeout()),&loop, SLOT(quit()));
	timer.start(1000);
	loop.exec();
	timer.stop();
	log_debug("Sleeper is off");

	if( !m_rpcConfigClient->isConnected() ) {
		IRpcSettingsManager* rpcSettingsManager = RpcSettingsManager::instance();
		rpcSettingsManager->setIniFile("./RPC/RpcOdServer.ini");
		QString host = rpcSettingsManager->getRpcHost();
		quint16 port = rpcSettingsManager->getRpcPort().toUShort();

		m_tabManager->setRpcConfig(port, host);

		m_rpcConfigClient->start(port, QHostAddress(host));
		connect(m_rpcConfigClient, SIGNAL(connectionEstablishedSignal()), this, SLOT(rpcConnectionEstablished()));
	}
}

void MainWindowController::resetServer()
{
	QStringList serverPIDList;
	bool searchResult;

	//! TODO Make other function to kill process
	//	searchResult = m_serverHandler->isProcessExist( m_serverHandler->getServicePath(), serverPIDList );

	//	if( searchResult ) {
	//		m_serverHandler->killProcessExist( serverPIDList );
	//	}

	QStringList param;
	param.append("/F");
	param.append("/IM");
	param.append("ZaviruhaODServer.exe");
	QProcess::execute("taskkill", param);

	QStringList paramAtlas;
	paramAtlas.append("/F");
	paramAtlas.append("/IM");
	paramAtlas.append("PwGisAtlas.exe");
	QProcess::execute("taskkill", paramAtlas);

	QStringList paramServer;
	paramServer.append("/F");
	paramServer.append("/IM");
	paramServer.append("PwGisServer.exe");
	QProcess::execute("taskkill", paramServer);
}

void MainWindowController::startTabManger()
{
	m_view->getStackedWidget()->setCurrentIndex(0);
	m_tabManager->start();
}


void MainWindowController::rpcConnectionEstablished()
{
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");

	m_solverSetupWidgetController->slotGetAll();

	/// BUG HERE!!!
	//	m_rpcConfigClient->requestGetDbConfiguration("./DB/db_uav.ini");

	//	m_rpcConfigClient->requestGetMapObjects("./Map/map_objects.ini");
	//	m_rpcConfigClient->requestGetMapObjects("./Map/map_points.ini");
}

void MainWindowController::solverDialogSlot()
{
	SolverSettingsDialog solverSettingsDialog(m_view);

	solverSettingsController->appendView(&solverSettingsDialog);

	int res = solverSettingsDialog.exec();

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);
	ds << 701;

	CommandMessage* msg = NULL;

	switch (res){
	case SOLVERSETTINGDIALOGACCEPT:
	{
		ds << solverSettingsDialog.getTrackLength();
		ds << solverSettingsDialog.getHeight();

		msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
		break;
	}
	case SOLVERSETTINGDIALOGCLEAR:
	{
		bool t = true;
		ds << t;

		msg = new CommandMessage(COMMAND_SET_SOLVER_CLEAR, ba);
		break;
	}
	default:
		return;
		break;
	}

	if (NULL == msg){
		return;
	}

	m_tabManager->send_data(0, msg);
}

void MainWindowController::enableAdsbClient(bool status)
{
	CommandMessage* msg = NULL;
	quint8 type = (quint8)hwAdsb;

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);
	ds << type;
	ds << status;

	msg = new CommandMessage(COMMAND_SET_ADSB, ba);

	m_tabManager->send_data(0, msg);
}

void MainWindowController::enableAdsbOnlineClient(bool status)
{
	CommandMessage* msg = NULL;
	quint8 type = (quint8)onlineAdsb;

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);
	ds << type;
	ds << status;

	msg = new CommandMessage(COMMAND_SET_ADSB, ba);

	m_tabManager->send_data(0, msg);
}

void MainWindowController::mapOpened()
{
	m_solverSetupWidgetController->setMapFlag();
}

void MainWindowController::slotServerProcessDead()
{
	QProcess p;
	QString guiName = "./" + QString(GUI_NAME);
	p.startDetached(guiName);

	qApp->exit(0);
}

void MainWindowController::slotCheckRpc()
{
	if(!m_armrConnection) {
		m_ping = true;
		return;
	}

	if(!m_ping) {
		log_debug("Ololo, no ping from ARM_R, goto restart");
		slotServerProcessDead();
	} else {
		m_ping = false;
	}
}

void MainWindowController::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();
	if (method == RPC_METHOD_CONFIG_ANSWER_STATION_LIST) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		QList<StationConfiguration> stationList;
		dataStream >> stationList;

		m_tabManager->clearAllInformation();
		m_tabManager->setStationsConfiguration(stationList);
		m_tabManager->addStationTabs();

		if(m_tabManager->getMapWidget()) {
			connect(m_view, SIGNAL(signalApply(int)), m_tabManager->getMapWidget(), SIGNAL(signalApply(int)));
			connect(m_view, SIGNAL(signalApply(int)), m_tabManager->getMapWidget(), SLOT(slotApply(int)));
			connect(m_view, SIGNAL(signalClear()), m_tabManager->getMapWidget(), SIGNAL(signalClear()));
			connect(m_view, SIGNAL(signalLoadBaseStations()), m_tabManager->getMapWidget(), SLOT(slotLoadBaseStations()));
		}

		m_tabManager->getRpcClient()->registerReceiver(this);

	} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		DBConnectionStruct dbConfig;
		dataStream >> dbConfig;

		m_tabManager->setDbConnectionStruct(dbConfig);
	} else if(method == RPC_SLOT_SERVER_SEND_MAP_PING) {
		m_ping = true;
		log_debug("Im Pinging!!!!!!!!! =))))))))))))))))))");
	} else if(method == RPC_SLOT_SERVER_SEND_ARMR_CONNECTION) {
		bool b = argument.toBool();

		m_armrConnection = b;
	}
}
