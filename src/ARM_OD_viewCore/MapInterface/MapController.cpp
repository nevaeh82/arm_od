#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "../Icons/IconsGenerator.h"

#include "../UAV/ZInterception.h"

//MapController* MapController::_map_controller = 0;

//MapController* MapController::getInstance()
//{
//    if(_map_controller == 0)
//    {
//       _map_controller = new MapController();
//    }
//    return _map_controller;
//}

MapController::MapController() : _controller_widget(new MapControllerWidget(this, this))
{
    _layersCounter = 0;

	_mapManager = _controller_widget->getPwGis()->mapProvider()->mapManager();
	QObject::connect(&_mapManager->events(), SIGNAL(mapReady()), this, SLOT(onMapReady()));

	//_mapManager = _pwwidget->mapProvider()->mapManager();
	_profileManager = _controller_widget->getPwGis()->mapProvider()->profileManager();

	_profileManager->registerProfile("Zav", ":/images/");
//    _profileManager->registerProfile("Zav", ":/images/UAV/BLA.png");

//    _pwwidget->enableDebugger(true);
	_mapManager->setProfile("Zav");
	_controller_widget->getPwGis()->enableDebugger(false);


//    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	connect(_controller_widget, SIGNAL(showBLAtree()), this, SIGNAL(controllerShowBLAtree()));
	connect(_controller_widget, SIGNAL(showBPLAtree()), this, SIGNAL(controllerShowBPLAtree()));
	connect(_controller_widget, SIGNAL(showNIIPP()), this, SIGNAL(controllerShowNIIPP()));

}

MapController::~MapController()
{
	_mapManager->closeMap();
}

void MapController::init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil)
{
    _db_bla = db_bla;
    _db_evil = db_evil;

    _m_settings = map_settings;
    QMap<int, TabsProperty *>::iterator it;
    for (it = _m_settings.begin(); it != _m_settings.end(); ++it)
    {
		_map_clients[(it.value())->get_id()] = new MapClient1(_controller_widget->getPwGis()
																, it.value(), _db_bla, _db_evil);
    }
}

void MapController::open_map_from_Atlas()
{
    bool rc = _mapManager->openAtlas();
//    qDebug() << rc;
}

void MapController::open_map_from_local_file(/*const QString mapFile*/)
{

    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Открыть карту"),
        QDir::currentPath(),
        tr("Формат (*.chart *.sxf);;Все файлы (*.*)") );
    if( !filename.isNull() )
    {
//      qDebug( filename.toAscii() );
    }

//    _pwwidget->enableDebugger(true);
    bool rc = _mapManager->openMap(filename, 100);
//    qDebug() << rc;

}

void MapController::onMapReady()
{
    QMap<int, IMapClient *>::iterator it;

    for (it = _map_clients.begin(); it != _map_clients.end(); ++it)
    {
        if(!it.value())
            continue;
        (it.value())->set_Point();
    }
	get_panel_widget()->setMouseTracking(true);
	emit mapOpened();
	_layerManager = _controller_widget->getPwGis()->mapProvider()->layerManager();
}

void MapController::_slot_station_visible(bool state)
{
    _map_clients.value(1)->show_layer(0, state);
}

PwGisWidget *MapController::get_pwwidget()
{
	return _controller_widget->getPwGis();
}

QWidget *MapController::get_widget()
{
	return _controller_widget->getWidget();
}

bool MapController::eventFilter(QObject *obj, QEvent *e)
{
    //_panel_widget->show();
    //_panel_widget->setCursor(Qt::CrossCursor);
//    qDebug() << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";

    return true;
}


/// get map client by name
IMapClient *MapController::get_map_client(int id)
{
    return _map_clients[id];
}

/// get panel widget
QWidget *MapController::get_panel_widget()
{
	return _controller_widget->getPanelWidget();
}
