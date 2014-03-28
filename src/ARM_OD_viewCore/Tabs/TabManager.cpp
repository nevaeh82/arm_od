#include "TabManager.h"

#include <QDebug>

TabManager::TabManager(QTabWidget* tabWidget, QObject *parent):
	QObject(parent)
{
	m_currentWidget = NULL;

	m_tabWidget = tabWidget;

	_db_manager_bla = new DBManager(this);
	_db_manager_evil = new DBManager(this);

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
}

TabManager::~TabManager()
{
}


void TabManager::send_data_niipp_control(int id, QByteArray ba)
{
	emit signalSendToNIIPPControl(id, ba);

}

void TabManager::start()
{

	changeTabSlot(m_tabWidget->currentIndex());



	//	_current_tab_widget  = static_cast<MapTabWidget* >(m_tabWidget->currentWidget());
	//	_current_tab_widget->start();
}


int TabManager::createSubModules(const QString& settingsFile)
{
	int count = readSettings(settingsFile);

	foreach (Station* station, m_stationsMap) {

		MapTabWidgetController* tabWidgetController = new MapTabWidgetController(station, m_stationsMap, this, _db_manager_bla, _db_manager_evil);
		MapTabWidget* tabWidget = new MapTabWidget(m_tabWidget);

		tabWidgetController->appendView(tabWidget);

		connect(this, SIGNAL(signalSendToNIIPPControl(int,QByteArray)), tabWidgetController, SLOT(_slot_send_data_to_niipp_control(int,QByteArray)));
		connect(this, SIGNAL(openAtlasSignal()), tabWidgetController, SLOT(openAtlasSlot()));
		connect(this, SIGNAL(openMapSignal()), tabWidgetController, SLOT(openMapSlot()));

		m_tabWidget->addTab(tabWidget, station->name);

		m_tabWidgetsMap.insert(station->name, tabWidgetController);
	}

	return count;
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


/// read settings for generated submodules (tabs)
int TabManager::readSettings(const QString& settingsFile)
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
}
