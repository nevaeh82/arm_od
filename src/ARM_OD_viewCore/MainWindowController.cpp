#include "MainWindowController.h"

#define SERVER_NAME "ARM_OD_Server"

MainWindowController::MainWindowController(QObject *parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(MainWindowController));

	m_view = NULL;
	m_tabManager = NULL;
	m_solver = NULL;
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
}

void MainWindowController::init()
{
	m_tabManager = new TabManager(m_view->getMainTabWidget(), this);

	QString tabs_setting_file = QCoreApplication::applicationDirPath();
	tabs_setting_file.append("/Tabs/Tabs.ini");

	m_tabManager->createSubModules(tabs_setting_file);
	m_tabManager->start();

	m_solver = new SolverController(701, m_tabManager);

	connect(m_view, SIGNAL(openAtlasSignal()), m_tabManager->get_map_controller(), SLOT(openMapFromAtlas()));
	connect(m_view, SIGNAL(openMapSignal()), m_tabManager->get_map_controller(), SLOT(openMapFromLocalFile()));

	connect(m_view, SIGNAL(setupKoordinatometriyaSignal()), m_solver, SLOT(show()));

}

void MainWindowController::serverFailedToStartSlot()
{
	QMessageBox::critical(m_view, tr("Unable to connect to server"), tr("Please try to restart application"));
}

void MainWindowController::serverStartedSlot()
{
	//startRpc();
}

