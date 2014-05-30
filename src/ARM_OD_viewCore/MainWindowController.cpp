#include <QMessageBox>

#include "MainWindowController.h"

#include "Interfaces/IUavHistoryListener.h"

#define SERVER_NAME "ARM_OD_Server"

MainWindowController::MainWindowController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_tabManager = NULL;
	m_rpcConfigClient = NULL;
	m_uavLifeTime = 0;
	m_serverHandler = 0;

	qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
	qRegisterMetaType<quint32>("quint32");

	qRegisterMetaType<Uav>( "Uav" );
	qRegisterMetaType<UavInfo>( "UavInfo" );
	qRegisterMetaType<Niipp>( "Niipp" );

	qRegisterMetaType<IUavHistoryListener::Status>( "Status" );

	QString serverName = "./" + QString(SERVER_NAME);
	#ifdef QT_DEBUG
		serverName += "d";
	#endif
	m_serverHandler = new Pw::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);
}

MainWindowController::~MainWindowController()
{
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
	connect(m_tabManager, SIGNAL( mapOpened() ), m_view, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( atlasOpened() ), m_view, SLOT( mapOpened() ));
	connect(m_tabManager, SIGNAL( cancelMapOpen() ), m_view, SLOT( cancelMapOpen() ));
	connect(m_view, SIGNAL(setupKoordinatometriyaSignal()), this, SLOT(solverDialogSlot()));
	connect(m_view, SIGNAL(signalResetServer()), this, SLOT(resetServer()));

	m_rpcConfigClient = new RpcConfigClient(this);
	m_rpcConfigClient->registerReceiver(this);

	// create solver settings dialog controller
	solverSettingsController = new SolverSettingsDialogController(701, m_tabManager, this);

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
	searchResult = m_serverHandler->isProcessExist( m_serverHandler->getServicePath(), serverPIDList );

	if( searchResult ) {
		m_serverHandler->killProcessExist( serverPIDList );
	}
}

void MainWindowController::startTabManger()
{
	m_view->getStackedWidget()->setCurrentIndex(0);
	m_tabManager->start();
}


void MainWindowController::rpcConnectionEstablished()
{
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");

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

		} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

			QDataStream dataStream(&data, QIODevice::ReadOnly);
			DBConnectionStruct dbConfig;;
			dataStream >> dbConfig;

			m_tabManager->setDbConnectionStruct(dbConfig);
		}
}
