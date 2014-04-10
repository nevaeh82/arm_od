#include "TabManager.h"

#include <QDebug>

TabManager::TabManager(QTabWidget* tabWidget, QObject *parent):
	QObject(parent)
{
	m_currentWidget = NULL;

	m_tabWidget = tabWidget;

	//Creating db bla settings manager
	m_dbBlaSettingsManager = DbBlaSettingsManager::instance();
	m_dbBlaSettingsManager->setIniFile("./DB/db_uav.ini");

	//Creating db bla controller
	m_dbUavController = new DbUavController();
//	m_dbUavController->connectToDB(getDbBlaConnectionSettings());

	//Creating db bla manager and set its controller
	m_dbUavManager = new DbUavManager();
	m_dbUavManager->setDbController(m_dbUavController);

	//Moving db bla manager to another thread
	QThread* dbBlaManagerThread = new QThread;
	connect(m_dbUavManager, SIGNAL(destroyed()), dbBlaManagerThread, SLOT(terminate()));
	m_dbUavManager->moveToThread(dbBlaManagerThread);
	dbBlaManagerThread->start();

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
}

TabManager::~TabManager()
{

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

		MapTabWidgetController* tabWidgetController = new MapTabWidgetController(station, m_stationsMap, this, m_dbUavManager, _db_manager_evil, this);
		tabWidgetController->setRpcConfig(m_rpcPort, m_rpcHost);

		MapTabWidget* tabWidget = new MapTabWidget(m_tabWidget);

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
	foreach (Station* station, m_stationsMap) {
		MapTabWidgetController* tabWidgetController = m_tabWidgetsMap.take(station->name);
		if (tabWidgetController != NULL){
			tabWidgetController->deleteLater();
		}
	}

	for (qint32 index = 0; index < m_tabWidget->count(); ++index) {
		MapTabWidget* tabWidget = dynamic_cast<MapTabWidget*>(m_tabWidget->widget(index));
		if (tabWidget != NULL){
			tabWidget->deleteLater();
		}
		m_tabWidget->removeTab(index);;
	}

//	foreach (Station* station, m_stationsMap) {
//		delete station;
//	}

	m_tabWidgetsMap.clear();
	m_tabWidget->clear();
	m_stationsMap.clear();
}

void TabManager::start()
{
	changeTabSlot(m_tabWidget->currentIndex());
}

/*int TabManager::createSubModules(const QString& settingsFile)
{
//	int count = readSettings(settingsFile);

	foreach (Station* station, m_stationsMap) {

		MapTabWidgetController* tabWidgetController = new MapTabWidgetController(station, m_stationsMap, this, m_dbUavManager);
		MapTabWidget* tabWidget = new MapTabWidget(m_tabWidget);

		tabWidgetController->appendView(tabWidget);

		connect(this, SIGNAL(signalSendToNIIPPControl(int,QByteArray)), tabWidgetController, SLOT(_slot_send_data_to_niipp_control(int,QByteArray)));
		connect(this, SIGNAL(openAtlasSignal()), tabWidgetController, SLOT(openAtlasSlot()));
		connect(this, SIGNAL(openMapSignal()), tabWidgetController, SLOT(openMapSlot()));

		m_tabWidget->addTab(tabWidget, station->name);

		m_tabWidgetsMap.insert(station->name, tabWidgetController);
	}

	return count;
}*/


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


/// read settings for generated submodules (tabs)
/*int TabManager::readSettings(const QString& settingsFile)
{
	int count = 0;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(settingsFile, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		qDebug() << "m_settings = " << childKey.toLatin1();
		m_settings.beginGroup(childKey);

		Station* station = new Station();

		station->id = m_settings.value("Id", 0).toInt();
		station->name =  m_settings.value("Name", tr("Unknown")).toString();
		station->latitude = m_settings.value("Latitude", "0").toDouble();
		station->longitude = m_settings.value("Longitude", "0").toDouble();

		m_stationsMap.insert(m_settings.value("Id", 0).toInt(), station);
		m_settings.endGroup();
		count++;
	}

	return count;
}*/
/*
DBConnectionStruct TabManager::getDbBlaConnectionSettings()
{
	DBConnectionStruct connectionStruct;

	connectionStruct.dbName = m_dbBlaSettingsManager->getBlaDbName();
	connectionStruct.host = m_dbBlaSettingsManager->getBlaDbHost();
	connectionStruct.login = m_dbBlaSettingsManager->getBlaDbLogin();
	connectionStruct.password = m_dbBlaSettingsManager->getBlaDbPassword();
	connectionStruct.port = m_dbBlaSettingsManager->getBlaDbPort();

	return connectionStruct;
}*/
