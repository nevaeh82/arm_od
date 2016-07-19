#include "TabManager.h"

#include <QDebug>


TabManager::TabManager(QTabWidget* tabWidget, QObject *parent):
	QObject(parent)
{
	m_currentWidget = NULL;

	m_viewMenu = NULL;

	m_tabWidget = tabWidget;

	//Creating db uav manager
	m_dbUavManager = new DbUavManager();

	//Creating db uav controller
	m_dbUavController = new DbUavController(m_dbUavManager);

	//Set to db uav manager its controller
	m_dbUavManager->setDbController(m_dbUavController);

	//Moving db uav manager to another thread
	QThread* dbBlaManagerThread = new QThread;
	//connect(m_dbUavManager, SIGNAL(destroyed()), dbBlaManagerThread, SLOT(terminate()));

	m_dbUavManager->moveToThread(dbBlaManagerThread);
	dbBlaManagerThread->start();

	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));

	IDbBlaSettingsManager* dbUavSettingsManager = DbBlaSettingsManager::instance();
	dbUavSettingsManager->setIniFile("./DB/db_uav.ini");

	DBConnectionStruct connectionStruct;

	connectionStruct.host		= dbUavSettingsManager->getBlaDbHost();
	connectionStruct.port		= dbUavSettingsManager->getBlaDbPort();
	connectionStruct.login		= dbUavSettingsManager->getBlaDbLogin();
	connectionStruct.password	= dbUavSettingsManager->getBlaDbPassword();
	connectionStruct.dbName		= dbUavSettingsManager->getBlaDbName();

	m_dbUavController->connectToDB(connectionStruct);

	m_dbUavManager->setLifeTime( dbUavSettingsManager->getBlaLifeTime() );

	connect(this, SIGNAL(finished()), dbBlaManagerThread, SLOT(quit()));
	connect(this, SIGNAL(finished()), dbBlaManagerThread, SLOT(deleteLater()));
	connect(this, SIGNAL(finished()), m_dbUavManager, SLOT(deleteLater()));
	connect(this, SIGNAL(finished()), m_dbUavController, SLOT(deleteLater()));
}

TabManager::~TabManager()
{
	emit finished();

	clearAllInformation();
}

void TabManager::send_data_niipp_control(int id, QByteArray ba)
{
	emit signalSendToNIIPPControl(id, ba);

}

void TabManager::setRpcConfig(const quint16& port, const QString& host)
{
	m_rpcHost = host;
	m_rpcPort = port;
}

void TabManager::setDbConnectionStruct(const DBConnectionStruct& connectionStruct)
{
	/// BUG HERE!!!
	m_dbUavController->disconnectFromDb();
	m_dbUavController->connectToDB(connectionStruct);
}

void TabManager::setStationsConfiguration(const QList<StationConfiguration>& stationList)
{
	m_stationsMap.clear();

	foreach (StationConfiguration stationConf, stationList) {
		Station *station = new Station();

		station->id = stationConf.id;
		station->name = stationConf.name;
		station->latitude = stationConf.latitude;
		station->longitude = stationConf.longitude;

		m_stationsMap.insert(station->id, station);
	}
}

void TabManager::addStationTabs()
{
	foreach (Station* station, m_stationsMap) {

		MapTabWidgetController* tabWidgetController = new MapTabWidgetController(station, m_stationsMap, this,
																				 m_dbUavManager, m_solverSetup);
		connect(m_dbUavController, SIGNAL(dbOutLog(QString)), tabWidgetController, SIGNAL(onDbOutLog(QString)));

		connect( tabWidgetController, SIGNAL( mapOpened() ), SIGNAL( mapOpened() ) );
		connect( tabWidgetController, SIGNAL( atlasOpened() ), SIGNAL( atlasOpened() ) );
		connect( tabWidgetController, SIGNAL( cancelMapOpen() ), SIGNAL( cancelMapOpen() ) );
		tabWidgetController->setRpcConfig(m_rpcPort, m_rpcHost);

		MapTabWidget* tabWidget = new MapTabWidget(m_tabWidget);

		m_viewMenu->addAction(tabWidget->getBlaDockWidget()->toggleViewAction());
		m_viewMenu->addAction(tabWidget->getBplaDockWidget()->toggleViewAction());
		//m_viewMenu->addAction(tabWidget->getNiipp1DockWidget()->toggleViewAction());
		//m_viewMenu->addAction(tabWidget->getNiipp2DockWidget()->toggleViewAction());
		m_viewMenu->addAction(tabWidget->getControlPanelWidget()->getDockWgt()->toggleViewAction());

		tabWidgetController->appendView(tabWidget);

		connect(this, SIGNAL(signalSendToNIIPPControl(int,QByteArray)), tabWidgetController, SLOT(_slot_send_data_to_niipp_control(int,QByteArray)));
		connect(this, SIGNAL(openAtlasSignal()), tabWidgetController, SLOT(openAtlasSlot()));
		connect(this, SIGNAL(openMapSignal()), tabWidgetController, SLOT(openMapSlot()));

		m_tabWidget->addTab(tabWidget, station->name);

		m_tabWidgetsMap.insert(station->name, tabWidgetController);
	}

	emit readyToStart();
}

void TabManager::clearAllInformation()
{
	m_currentWidget = NULL;

	foreach (Station* station, m_stationsMap) {

//		m_viewMenu->removeAction(tabWidget->getBlaDockWidget()->toggleViewAction());
//		m_viewMenu->removeAction(tabWidget->getBplaDockWidget()->toggleViewAction());
//		m_viewMenu->removeAction(tabWidget->getNiipp1DockWidget()->toggleViewAction());
//		m_viewMenu->removeAction(tabWidget->getNiipp2DockWidget()->toggleViewAction());

		MapTabWidgetController* tabWidgetController = m_tabWidgetsMap.take(station->name);
		if (tabWidgetController != NULL){
			disconnect( tabWidgetController, SIGNAL( mapOpened() ), this, SIGNAL( mapOpened() ) );
			disconnect( tabWidgetController, SIGNAL( atlasOpened() ), this, SIGNAL( atlasOpened() ) );
			disconnect( tabWidgetController, SIGNAL( cancelMapOpen() ), this, SIGNAL( cancelMapOpen() ) );
			delete tabWidgetController;
			tabWidgetController = NULL;
		}
	}

	for (qint32 index = 0; index < m_tabWidget->count(); ++index) {
		QWidget* tabWidget = m_tabWidget->widget(index);
		if (tabWidget != NULL){
			delete tabWidget;
			tabWidget = NULL;
		}
		m_tabWidget->removeTab(index);
	}

	foreach (Station* station, m_stationsMap) {
		if (NULL != station) {
			delete station;
			station = NULL;
		}
	}

	m_tabWidgetsMap.clear();
	m_tabWidget->clear();
	m_stationsMap.clear();

	m_dbUavManager->deleteAllUav();
}

void TabManager::addSolverSetupController(SolverSetupWidgetController *controller)
{
	m_solverSetup = controller;
}

void TabManager::setUavLifeTime(int msecs)
{
	m_dbUavManager->setLifeTime( msecs );
}

void TabManager::start()
{
	changeTabSlot(m_tabWidget->currentIndex());
}

void TabManager::setViewMenu(QMenu *menu)
{
	m_viewMenu = menu;
}

/// BUG
void TabManager::send_data(int index, IMessageOld *msg)
{
	MapTabWidgetController* controller = m_tabWidgetsMap.value(m_tabWidget->tabText(index), NULL);

	if (NULL == controller) {
		return;
	}

	controller->set_command(msg);
}

/// slot tab change
void TabManager::changeTabSlot(int index)
{
	MapTabWidgetController* controller = m_tabWidgetsMap.value(m_tabWidget->tabText(index), NULL);

	if (NULL == controller) {
		return;
	}

	if  (NULL != m_currentWidget) {
		m_currentWidget->stop();
	}

	m_currentWidget = controller;
	m_currentWidget->start();

}

