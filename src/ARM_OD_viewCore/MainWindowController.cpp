#include "MainWindowController.h"

#include <QMessageBox>

#define SERVER_NAME "ARM_OD_Server"

MainWindowController::MainWindowController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_tabManager = NULL;
	m_serverHandler = 0;
	m_rpcConfigClient = NULL;

	qRegisterMetaType<UavInfo>( "UavInfo" );
}

MainWindowController::~MainWindowController()
{
	stop();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

	init();
}

void MainWindowController::startServer()
{
	start();
}

void MainWindowController::start()
{
	QString serverName = "./" + QString(SERVER_NAME);

#ifdef QT_DEBUG
	serverName += "d";
#endif

	m_serverHandler = new Pw::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);
	connect(m_serverHandler, SIGNAL(processStartedSignal()), this, SLOT(serverStartedSlot()));
	connect(m_serverHandler, SIGNAL(processStartFailedSignal()), this, SLOT(serverFailedToStartSlot()));

	m_serverHandler->start();
}

void MainWindowController::stop()
{
	//m_serverHandler->terminate();
	m_serverHandler->kill();
	m_tabManager->clearAllInformation();
}

void MainWindowController::init()
{
	m_view->getStackedWidget()->setCurrentIndex(1);

	m_tabManager = new TabManager(m_view->getMainTabWidget(), this);
	connect(m_tabManager, SIGNAL(readyToStart()), this, SLOT(startTabManger()));

	connect(m_view, SIGNAL(openAtlasSignal()), m_tabManager, SIGNAL(openAtlasSignal()));
	connect(m_view, SIGNAL(openMapSignal()), m_tabManager, SIGNAL(openMapSignal()));

	connect(m_view, SIGNAL(setupKoordinatometriyaSignal()), this, SLOT(solverDialogSlot()));

	m_rpcConfigClient = new RpcConfigClient(this);
	m_rpcConfigClient->registerReceiver(this);

	// create solver settings dialog controller
	solverSettingsController = new SolverSettingsDialogController(701, m_tabManager, this);
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
