#include "TabManager.h"

#include <QDebug>

TabManager::TabManager(QTabWidget* tabWidget, QObject *parent):
	QObject(parent)
{
	m_tabWidget = tabWidget;

    _map_controller = new MapController();
    _router = new Router();


    _map_niipp = new QMap<int, QDockWidget *>;

    _db_manager_bla = new DBManager(this);
    _db_manager_evil = new DBManager(this);



//    this->setGeometry(QRect(10, 10, 300, 250));
//    _layout = new QVBoxLayout();
//    _layout->alignment();
//    this->setLayout(_layout);

//    _model_controller = new Controller();

}

TabManager::~TabManager()
{
    disconnect(this, SIGNAL(currentChanged(int)), this, SLOT(_slot_change_tab(int)));
    if(_map_settings.count() > 0)
    {
        QMap<int, TabsProperty *>::iterator it;
        for(it = _map_settings.begin(); it != _map_settings.end(); ++it)
        {
            delete it.value();
        }
    }
    if(_map_tabs.count() > 0)
    {
        QMap<int, TabMap *>::iterator its;
        for(its = _map_tabs.begin(); its != _map_tabs.end(); ++its)
        {
            delete its.value();
        }
    }
    if(_map_controller != NULL)
    {
        delete _map_controller;
    }

    if(_router != NULL)
    {
        delete _router;
    }

    delete _map_niipp;

//    delete _db_manager_bla;
//    delete _db_manager_evil;
}

MapController *TabManager::get_map_controller()
{
    return _map_controller;
}

void TabManager::set_dbs(IDBManager *db_bla, IDBManager *db_evil)
{
}

void TabManager::send_data_niipp_control(int id, QByteArray ba)
{
    emit signalSendToNIIPPControl(id, ba);

}

int TabManager::start()
{
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(_slot_change_tab(int)));

	 _current_tab_widget  = static_cast<TabMap* >(m_tabWidget->currentWidget());
     _current_tab_widget->start();
//    _map_settings.value(this->currentIndex());
    return 0;
}

int TabManager::stop()
{
    return 0;
}

void TabManager::show()
{

}

void TabManager::hide()
{

}

int TabManager::createSubModules(QString path_to_ini_file)
{

    int count = _read_settings(path_to_ini_file);
    /// create map controller
    _map_controller->init(_map_settings, _db_manager_bla, _db_manager_evil);
//    /// create common database manager for spectrum tabs
//    _db_manager_spectrum = new DBManager(this);
//    /// create common model for spectrum tabs
//    QStringList headers;
//    headers << tr("Название") << tr("Свойство");
//    _model_spectrum = new TreeModel(headers);

//    _db_manager_spectrum->set_model(_model_spectrum);
//    _model_spectrum->set_db(_db_manager_spectrum);


    QPointF latlon1;
    latlon1.setX(42.511183);
    latlon1.setY(41.6905);
    NIIPPControl* _niipp1 = new NIIPPControl(100, tr("СПИП ДД-1"), latlon1, _router, _map_controller, this);

    QPointF latlon2;
    latlon2.setX(42.634183);
    latlon2.setY(41.912167);
    NIIPPControl* _niipp2 = new NIIPPControl(101, tr("СПИП ДД-2"), latlon2, _router, _map_controller, this);

    QDockWidget* dock_niipp1 = new QDockWidget(tr("СПИП ДД-1"));
    dock_niipp1->setWidget(_niipp1);

    QDockWidget* dock_niipp2 = new QDockWidget(tr("СПИП ДД-2"));
    dock_niipp2->setWidget(_niipp2);


    _map_niipp->insert(1, dock_niipp1);
    _map_niipp->insert(2, dock_niipp2);

    connect(this, SIGNAL(signalSendToNIIPPControl(int,QByteArray)), this, SLOT(_slot_send_data_to_niipp_control(int,QByteArray)));

    connect(dock_niipp1, SIGNAL(visibilityChanged(bool)), this, SLOT(_slot_show_niipp1(bool)));

    QMap<int, TabsProperty* >::iterator it;
    for(it = _map_settings.begin(); it != _map_settings.end(); ++it)
    {
        TabMap* tab_sp = new TabMap(it.value(), _router, this, _map_niipp, _db_manager_bla, _db_manager_evil);
		m_tabWidget->addTab(tab_sp, it.value()->get_name());
        _map_tabs.insert(it.key(), tab_sp);
    }



    return count;
}

QString TabManager::getStationName(int id)
{
    _mux.lock();
    TabsProperty *t = _map_settings.value(id);
    _mux.unlock();
    return t->get_name();
}

/// call this method when data in tree has changed
void TabManager::send_data(int pid, IMessage *msg)
{
	TabMap* tab_sp = static_cast<TabMap* >(m_tabWidget->widget(pid));
    tab_sp->set_command(msg);
}

/// slot tab change
void TabManager::_slot_change_tab(int index)
{
    _current_tab_widget->stop();

	_current_tab_widget = static_cast<TabMap* >(m_tabWidget->widget(index));
    _current_tab_widget->start();
    TabsProperty *prop = _current_tab_widget->get_tab_property();
//    qDebug() << "cur id = " << prop->get_id();

    //    _model_spectrum->fill_model(prop->get_id());
}

void TabManager::_slot_show_niipp1(bool state)
{
//    QDockWidget* dock = _map_niipp->value(1);
//    NIIPPControl *n_control = static_cast<NIIPPControl* >(dock->widget());
//    if(state == false && n_control->getState())
//    {
//        dock->show();

//        dock->setWidget(n_control);
//        dock->eventFilter()

////        n_control->show();
    //    }
}

void TabManager::_slot_send_data_to_niipp_control(int id, QByteArray data)
{
    NIIPPControl* niip_contr;
    switch(id)
    {
    case 100:
        niip_contr = dynamic_cast<NIIPPControl *>(_map_niipp->value(1)->widget());
        niip_contr->set_data(data);
        break;
    case 101:
        niip_contr = dynamic_cast<NIIPPControl *>(_map_niipp->value(2)->widget());
        niip_contr->set_data(data);        break;
    default:
        break;
    }
}

/// read settings for generated submodules (tabs)
int TabManager::_read_settings(QString path_to_ini_file)
{
    int count = 0;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QSettings m_settings(path_to_ini_file, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        qDebug() << "m_settings = " << childKey.toLatin1();
        m_settings.beginGroup(childKey);

        TabsProperty *prop = new TabsProperty();

        prop->set_id(m_settings.value("Id", 0).toInt());
        prop->set_name(m_settings.value("Name", 0).toString());
        prop->set_latitude(m_settings.value("Latitude", "0").toDouble());
        prop->set_longitude(m_settings.value("Longitude", "0").toDouble());

        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop);
        m_settings.endGroup();
        count++;
    }

    return count;
}
