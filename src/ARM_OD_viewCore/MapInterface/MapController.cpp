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



MapController::MapController()
{
	_layersCounter = 0;
	_pwwidget = new PwGisWidget();
	_panel_widget = new QWidget();
	_com_widget = new QWidget();

	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::gray);
	_panel_widget->setAutoFillBackground(true);
	_panel_widget->setPalette(p);

	QHBoxLayout *hbox = new QHBoxLayout();
	IconsGenerator gen;
	QPushButton *pb = gen.set_centred();
	QPushButton *pb_home = gen.set_home();
	QPushButton *pb_spiral = gen.set_spiral();
	QPushButton *pb_tablet = gen.set_tablet();
	QPushButton *pb_web = gen.set_web_mail();

	pb_show_BLA_tree = new QPushButton(tr("Список БЛА"));//gen.set_spiral();
	pb_show_BPLA_tree = new QPushButton(tr("Список целей"));//gen.set_spiral();
	pb_show_NiiPP = new QPushButton(tr("СПИП ДД"));//gen.set_spiral();

	hbox->addWidget(pb);
//    hbox->addWidget(pb_home);
//    hbox->addWidget(pb_spiral);
//    hbox->addWidget(pb_tablet);
//    hbox->addWidget(pb_web);

	hbox->addWidget(pb_show_BLA_tree);
	hbox->addWidget(pb_show_BPLA_tree);
	hbox->addWidget(pb_show_NiiPP);

//    _chb_stations = new QCheckBox(tr("Пункты"));
//    _chb_stations->setChecked(true);
//    hbox->addWidget(_chb_stations);

//    connect(_chb_stations, SIGNAL(clicked(bool)), this, SLOT(_slot_station_visible(bool)));

	hbox->setAlignment(Qt::AlignLeft);
	_panel_widget->setLayout(hbox);
	_panel_widget->setMaximumHeight(50);
//    _panel_widget->setMinimumWidth(350);

	QVBoxLayout *hbox2 = new QVBoxLayout();
	hbox2->addWidget(_panel_widget, Qt::AlignTop);
	hbox2->addWidget(_pwwidget, Qt::AlignTop);

	_mapManager = _pwwidget->mapProvider()->mapManager();
	QObject::connect(&_mapManager->events(), SIGNAL(mapReady()), this, SLOT(onMapReady()));

	_mapManager = _pwwidget->mapProvider()->mapManager();
	_profileManager = _pwwidget->mapProvider()->profileManager();

    _profileManager->registerProfile("Zav", ":/images/");
//    _profileManager->registerProfile("Zav", ":/images/UAV/BLA.png");

//    _pwwidget->enableDebugger(true);
	_mapManager->setProfile("Zav");
	_pwwidget->enableDebugger(false);

	_com_widget->setLayout(hbox2);

//    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}

MapController::~MapController()
{
//    _mapManager->closeMap();
}

void MapController::init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil)
{
	_db_bla = db_bla;
	_db_evil = db_evil;

	_m_settings = map_settings;
	QMap<int, TabsProperty *>::iterator it;
	for (it = _m_settings.begin(); it != _m_settings.end(); ++it)
	{
		_map_clients[(it.value())->get_id()] = new MapClient1(_pwwidget, it.value(), _db_bla, _db_evil);
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
		tr("Формат цифровых карт (*.chart *.sxf *.sit *.map *.gc *.gst);;Все файлы (*.*)") );
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
	_panel_widget->setMouseTracking(true);
	emit signalOpenedMap();
	_layerManager = _pwwidget->mapProvider()->layerManager();
}

void MapController::_slot_station_visible(bool state)
{
	_map_clients.value(1)->show_layer(0, state);
}

PwGisWidget *MapController::get_pwwidget()
{
	return _pwwidget;
}

QWidget *MapController::get_widget()
{
	return _com_widget;
}

bool MapController::eventFilter(QObject *obj, QEvent *e)
{
	//_panel_widget->show();
	//_panel_widget->setCursor(Qt::CrossCursor);
//    qDebug() << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";

	return true;
}

void MapController::mouseMoveEvent ( QMouseEvent * event )
{
	_panel_widget->setMouseTracking(true);

//    qDebug() << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
}

/// get map client by name
IMapClient *MapController::get_map_client(int id)
{
	return _map_clients[id];
}

/// get panel widget
QWidget *MapController::get_panel_widget()
{
	return _panel_widget;
}
