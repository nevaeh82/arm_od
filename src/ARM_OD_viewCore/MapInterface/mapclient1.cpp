#include "MapClient1.h"

const double m_zone[52] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
									   5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
									   18, 20, 22, 24, 28, 30, 2.5, 3, 4, 5,
									   6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
									   22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};


MapClient1::MapClient1(PwGisWidget *pwwidget, Station *station)
	:_mux(QMutex::Recursive),
	  _niipp_layer_id(0),
	_sector(NULL),
	_layersCounter(0),
	  _bis123(0)
{

	_point_uvoda_niipp.setX(0);
	_point_uvoda_niipp.setY(0);
	_peleng = NULL;
	_cicle = NULL;
	_circleRadius = 0;
	_circleChanged = false;
	_circleExists  = false;

	_rad = 0;
	_sliceRadius = 0;
	_sliceChanged = false;
	_sliceExists  = false;

	_niipp_layer_name = "NIIPP";

	_line_id = 0;
	_pwwidget = pwwidget;
	_mapBounds = _pwwidget->mapProvider()->mapBounds();
	m_station = station;
	_main_latitude = m_station->latitude;
	_main_longitude = m_station->longitude;
	//_set_Point();
	_last_coord = new QMap<int, PwGisPointList *>;

	QString niipp_layer_id = _niipp_layer_name + QString::number(_niipp_layer_id);
	_add_niipp_layer(niipp_layer_id);


	_update_timer = new QTimer();
	connect(this, SIGNAL(signal_new_coord_bla(int,QByteArray)), this, SLOT(_slot_set_cur_point(int,QByteArray)));
	connect(this, SIGNAL(signal_new_coord_evil(int,QByteArray)), this, SLOT(_slot_set_cur_point(int,QByteArray)));
	connect(this, SIGNAL(signal_new_ais_data(QMap<int, QVector<QString> >)), this, SLOT(_slot_set_ais_data(QMap<int, QVector<QString> >)));
	connect(_update_timer, SIGNAL(timeout()), this, SLOT(_slot_update_points()));
	connect(this, SIGNAL(signal_peleng(int, int, double, double, double)), this, SLOT(_slot_update_peleng(int, int, double, double, double)));
	connect(this, SIGNAL(signalUpdateCicle(int, double, QByteArray)), this, SLOT(_slot_update_cicle(int, double, QByteArray)));

	connect(this, SIGNAL(signalUpdateSector(int,double,double, QByteArray)), this, SLOT(_slot_update_sector(int,double,double, QByteArray)));

	connect(this, SIGNAL(signalAddPerehvat(int, int)), this, SLOT(_slot_add_perehvat(int, int)));
	connect(this, SIGNAL(signalRemovePerehvat(int, int)), this, SLOT(_slot_remove_perehvat(int, int)));



//    _update_timer->start(1000);
//    connect(this, SIGNAL((int,QPointF)), this, SLOT(_slot_set_point_bla(int,QPointF)));
//    connect(this, SIGNAL(signal_new_coord_evil(int,QPointF)), this, SLOT(_slot_set_point_evil(int,QPointF)));

	_timer_simulator = new QTimer();
	connect(_timer_simulator, SIGNAL(timeout()), this, SLOT(_slot_simulator()));

	_uiTimer = new QTimer(this);

	connect( _uiTimer, SIGNAL(timeout()), this, SLOT(updateCircle()));
	_uiTimer->setInterval(100);
	_uiTimer->start();


	_uiTimerslice = new QTimer(this);

	connect( _uiTimerslice, SIGNAL(timeout()), this, SLOT(updateSlice()));

	_uiTimerslice->setInterval(100);
//    _uiTimerslice->start();

	QObject::connect(&_pwwidget->mapProvider()->mapManager()->events(), SIGNAL(mapReady()), this, SLOT(onMapReady()));

	_layerManager = _pwwidget->mapProvider()->layerManager();
//    _addMarkerLayer(0, "Zaviruha");

	_perehvat = new ZInterception(this);

	QThread *thread = new QThread;
//        connect(client, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
//    connect(thread, SIGNAL(started()), client, SLOT(proccess()));
	connect(_perehvat, SIGNAL(finished()), thread, SLOT(quit()));
	connect(_perehvat, SIGNAL(finished()), _perehvat, SLOT(deleteLater()));

	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));



	_perehvat->moveToThread(thread);
	thread->start();


	connect(this, SIGNAL(signalPerehvatPoint(int, int, QPointF, float, float, int, float, float)), this, SLOT(_slot_perehvat_point(int, int, QPointF, float, float, int, float, float)));
}

MapClient1::~MapClient1()
{
}

void MapClient1::set_niipp_controller(INiiPPController *niipp_controller)
{
	_map_niipp_Controller.insert(niipp_controller->get_id(), niipp_controller);
//    _niipp_Controller = niipp_controller;
}

void MapClient1::show_layer(int index, bool state)
{
	if(_map_layers.contains(index))
	{
		_layerManager->setLayerVisibility(_map_layers.value(index), state);
	}
}

void MapClient1::add_perehvat(int bla_id, int bpla_id)
{
	emit signalAddPerehvat(bla_id, bpla_id);
}

/// set central control point
void MapClient1::set_Point()
{
	//define style
	PwGisStyle* StationStyle = _pwwidget->createStyle("stations");

	StationStyle->setProperty( PwGisStyle::mapFontColor, "red" );

	StationStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	StationStyle->setProperty( PwGisStyle::externalGraphic,  "/profiles/Zav/tabs/images/Punkts/StationRed128.png" );
	StationStyle->setProperty( PwGisStyle::fillColor, "red" );
	StationStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	StationStyle->setProperty( PwGisStyle::graphicHeight, "40" );

	this->_addMarkerLayer(0, tr("OP"));

	StationStyle->setProperty( PwGisStyle::layer, _map_layers.value(0));

	StationStyle->apply();

	/// BPLA profile
	PwGisStyle* BPLAStyle = _pwwidget->createStyle("BPLA");

	BPLAStyle->setProperty( PwGisStyle::mapFontColor, "blue" );

	BPLAStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	BPLAStyle->setProperty( PwGisStyle::externalGraphic,  "/profiles/Zav/UAV/images/UAV/BPLA48.png" );
	BPLAStyle->setProperty( PwGisStyle::fillColor, "blue" );
	BPLAStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	BPLAStyle->setProperty( PwGisStyle::graphicHeight, "60" );
	BPLAStyle->setProperty( PwGisStyle::strokeColor, "blue" );

	this->_addMarkerLayer(1, tr("UAV_"));

	BPLAStyle->setProperty( PwGisStyle::layer, _map_layers.value(1));

	BPLAStyle->apply();

	/// BLA profile
	PwGisStyle* BLAStyle = _pwwidget->createStyle("BLA");

	BLAStyle->setProperty( PwGisStyle::mapFontColor, "red" );
	BLAStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	BLAStyle->setProperty( PwGisStyle::externalGraphic,  "/profiles/Zav/UAV/images/UAV/BLA48.png" );
	BLAStyle->setProperty( PwGisStyle::fillColor, "red" );
	BLAStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	BLAStyle->setProperty( PwGisStyle::graphicHeight, "60" );
//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "0");
	BLAStyle->setProperty( PwGisStyle::strokeColor, "red" );

	this->_addMarkerLayer(2, tr("UAV"));
	QString t = _map_layers.value(2);

	BLAStyle->setProperty( PwGisStyle::layer, _map_layers.value(2));

	BLAStyle->apply();

	PwGisStyle* pelengStyle = _pwwidget->createStyle("Pelengators");

	pelengStyle->setProperty( PwGisStyle::mapFontColor, "green" );
	pelengStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
//    pelengStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png"  );
	pelengStyle->setProperty( PwGisStyle::fillColor, "green" );
	pelengStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pelengStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pelengStyle->setProperty( PwGisStyle::strokeColor, "green" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(3, tr("Atlant"));

	pelengStyle->setProperty( PwGisStyle::layer, _map_layers.value(3));
	pelengStyle->apply();

	PwGisStyle* pelengPointStyle = _pwwidget->createStyle("PelengatorsPoint");

	pelengPointStyle->setProperty( PwGisStyle::mapFontColor, "green" );
	pelengPointStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	pelengPointStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png"  );
	pelengPointStyle->setProperty( PwGisStyle::fillColor, "green" );
	pelengPointStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pelengPointStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pelengPointStyle->setProperty( PwGisStyle::strokeColor, "green" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(4, tr("Atlant target"));

	pelengPointStyle->setProperty( PwGisStyle::layer, _map_layers.value(4));
	pelengPointStyle->apply();


	PwGisStyle* gridStyle = _pwwidget->createStyle("Grid");

	gridStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	gridStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
//    gridStyle->setProperty( PwGisStyle::externalGraphic,  "/profiles/Zav/UAV/images/UAV/robot.png" );
	gridStyle->setProperty( PwGisStyle::fillColor, "black" );
	gridStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	gridStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	gridStyle->setProperty( PwGisStyle::strokeColor, "black" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(5, tr("Grid"));

	gridStyle->setProperty( PwGisStyle::layer, _map_layers.value(5));

	gridStyle->apply();
	show_layer(5, false);


	PwGisStyle* pointsStyle = _pwwidget->createStyle("Control_points");

	pointsStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	pointsStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
//    pointsStyle->setProperty( PwGisStyle::fillColor, "black" );
	pointsStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pointsStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pointsStyle->setProperty( PwGisStyle::strokeColor, "black" );


	this->_addMarkerLayer(6, tr("Checkpoints"));

	pointsStyle->setProperty( PwGisStyle::layer, _map_layers.value(6));

	pointsStyle->apply();


	PwGisStyle* perehvatStyle = _pwwidget->createStyle("Perehvat");

	perehvatStyle->setProperty( PwGisStyle::mapFontColor, "red" );
	perehvatStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	perehvatStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png"  );
//    perehvatStyle->setProperty( PwGisStyle::fillColor, "green" );
	perehvatStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	perehvatStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	perehvatStyle->setProperty( PwGisStyle::strokeColor, "red" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(7, tr("Interception point"));

	perehvatStyle->setProperty( PwGisStyle::layer, _map_layers.value(7));
	perehvatStyle->apply();

	PwGisStyle* AISStyle = _pwwidget->createStyle("AIS");

	AISStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	AISStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	AISStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/ais/images/AIS/PostPlaneBlack.png"  );
	AISStyle->setProperty( PwGisStyle::fillColor, "black" );
	AISStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	AISStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	AISStyle->setProperty( PwGisStyle::strokeColor, "black" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(8, tr("Civil ships"));

	AISStyle->setProperty( PwGisStyle::layer, _map_layers.value(8));
	AISStyle->apply();

	PwGisStyle* NIPPPointStyle = _pwwidget->createStyle("NIIPPPoint");

	NIPPPointStyle->setProperty( PwGisStyle::mapFontColor, "brown" );
	NIPPPointStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	NIPPPointStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/Points/images/Points/redmark48.png"  );
	NIPPPointStyle->setProperty( PwGisStyle::fillColor, "brown" );
	NIPPPointStyle->setProperty( PwGisStyle::graphicWidth, "20" );
	NIPPPointStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	NIPPPointStyle->setProperty( PwGisStyle::strokeColor, "brown" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(9, tr("Diversion points"));

	NIPPPointStyle->setProperty( PwGisStyle::layer, _map_layers.value(9));
	NIPPPointStyle->apply();



	PwGisStyle* NiiPPStyle = _pwwidget->createStyle("NIIPP");
	NiiPPStyle->setProperty( PwGisStyle::mapFontColor, "brown" );
	NiiPPStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	NiiPPStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/Points/images/Points/redmark48.png"  );
	NiiPPStyle->setProperty( PwGisStyle::fillColor, "brown" );
	NiiPPStyle->setProperty( PwGisStyle::graphicWidth, "20" );
	NiiPPStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	NiiPPStyle->setProperty( PwGisStyle::strokeColor, "brown" );

//    BLAStyle->setProperty( PwGisStyle::graphicXOffset, "10");

	this->_addMarkerLayer(10, tr("SPIP DD"));

	NiiPPStyle->setProperty( PwGisStyle::layer, _map_layers.value(10));
	NiiPPStyle->apply();




	QString map_setting_file = QCoreApplication::applicationDirPath();
	map_setting_file.append("/Map/map_objects.ini");
	_read_settings(map_setting_file);

	QString map_setting_file1 = QCoreApplication::applicationDirPath();
	map_setting_file1.append("/Map/map_points.ini");


//    _read_settings1(map_setting_file1);

	/// niip slice
//    _timer_simulator->start(1000);

	double left, rigth, top, bottom;
	if(_mapBounds->getMapBounds()->getLeft() < _mapBounds->getMapBounds()->getRight())
	{
		left = _mapBounds->getMapBounds()->getLeft();
		rigth = _mapBounds->getMapBounds()->getRight();
	}
	else
	{
		rigth = _mapBounds->getMapBounds()->getLeft();
		left = _mapBounds->getMapBounds()->getRight();
	}

	if(_mapBounds->getMapBounds()->getBottom() < _mapBounds->getMapBounds()->getTop())
	{
		top = _mapBounds->getMapBounds()->getTop();
		bottom = _mapBounds->getMapBounds()->getBottom();
	}
	else
	{
		bottom = _mapBounds->getMapBounds()->getTop();
		top = _mapBounds->getMapBounds()->getBottom();
	}

	QVariant var = (_pwwidget->executeScript("map.baseLayer.projection.getCode();"));
//    _pwwidget->executeScript()

	double step_x = 10/(111.111*cos(top/(180/3.14)));
	double step_y = 10/(111.111);
	int x = 0, y = 0;
//    for( double i = left; i < rigth; i += step_x)
//    {
//        _pwwidget->addLine(QString("Grid_x_%1").arg(x++), i,top,i,bottom, QString(tr("Д_%1")).arg(x), "", "Grid");
//    }

//    for( double i = bottom; i < top; i += step_y)
//    {
//        _pwwidget->addLine(QString("Grid_x_%1").arg(x++), left,i,rigth,i, QString(tr("Ш_%1")).arg(y++), "", "Grid");
//    }
}

/// set map centred to point
void MapClient1::center_map()
{
	_mapBounds->setMapCenter(_main_longitude, _main_latitude);
//    justify_map();
}

/// set justify map
void MapClient1::justify_map()
{
	MapZoomInfo* info = _mapBounds->getMapZoomInfo();
	GeoRectangle* rect = _mapBounds->getMapBounds();

	int h = _pwwidget->maximumHeight();
	int w = _pwwidget->maximumWidth();
	_mapBounds->zoomMapTo(0, 0, w, h/*rect->getLeft(), rect->getBottom(), rect->getRight(), rect->getTop()*/);
}

/// set point from another thread
void MapClient1::slot_add_BLA(int id, QByteArray data)
{
	QMutexLocker lock(&_mux);
	emit signal_new_coord_bla(id, data);
}

/// set cur point
void MapClient1::_slot_set_cur_point(int id, QByteArray data)
{
	QMutexLocker lock(&_mux);



//    if(!_map_cur_points.contains(id))
//    {
//        tt *t1 = new tt;
//        t1->point = new QPointF(point);
//        t1->alt = alt;
//        t1->speed = speed;
//        t1->course = course;
//        t1->state = state;


		_map_cur_points.insert(id, data);
//    }
//    else
//    {
//        tt *t2 = _map_cur_points.value(id);
//        QPointF *point_ = t2->point;
//        point_->setX(point.x());
//        point_->setY(point.y());
//        t2->alt = alt;
//        t2->speed = speed;
//        t2->course = course;
//        t2->state = state;
//    }
}

void MapClient1::_slot_update_points()
{
	QMutexLocker lock(&_mux);

//    _bis123 += 5;
//    this->_slot_update_sector(100, 29, _bis123, NULL);

	QMap<int, QByteArray>::iterator it;
	for(it = _map_cur_points.begin(); it != _map_cur_points.end(); ++it)
	{
		int id = it.key();
		QString s = QString::number(id);
		s = s.left(2);
//        tt *t1 = it.value();
		if(id > 100)
		{
			QDataStream ds(&it.value(), QIODevice::ReadOnly);
			QPointF point;
			ds >> point;
			double alt;
			ds >> alt;
			double speed;
			ds >> speed;
			double course;
			ds >> course;
			int state;
			ds >> state;
			_slot_set_point_bla(id, point, alt, speed, course, state);
		}
		else if(id >= 50)
		{
			_slot_set_point_evil(id, it.value());
		}
		else if(id > 0)
		{
			QDataStream ds1(&it.value(), QIODevice::ReadOnly);
			QPointF point;
			ds1 >> point;
			_slot_set_point_evil_peleng(id, point);
		}

		if(_map_battle.contains(id))
		{
		   int per_id = id;
		   int target_id = _map_battle.value(per_id);
		   QByteArray target_ba = _map_cur_points.value(target_id);
		   QByteArray per_ba = _map_cur_points.value(per_id);
		   if(!(target_ba.size() == 0 || per_ba.size() == 0))
		   {
				_perehvat->set(per_id, target_id, per_ba, target_ba);
		   }
		}


	}

//    _pwwidget->update();
////    _pwwidget->mapProvider()
//    QMap<QString, Sector* >::iterator it1;
//    for(it1 = _map_peleng.begin(); it1 != _map_peleng.end(); ++it1)
//    {
//        it1.value()->updateMap();
//    }
}

/// set point in this thread
void MapClient1::_slot_set_point_bla(int id, QPointF point, double alt, double speed, double course, int state)
{
	QMutexLocker lock(&_mux);
	PwGisPointList *p;
	if(_last_coord->contains(id))
	{
		p = _last_coord->value(id);
	}
	else
	{
		p = new PwGisPointList();
		_last_coord->insert(id, p);
	}

	QString name;
	if(id == 1044)
	{
	   name  = tr("UAV-C");
	}
	else
	{
		name = tr("UAV (#") + QString::number(id) + ")";
	}
//    name.append(QString::number(point.x(), 'f', 4));
//    name.append("\\n");
//    name.append(QString::number(point.y(), 'f', 4));
//    name.append("\\n");
//    name.append(QString::number(alt, 'f', 1));


	double y = point.y();
	double x = point.x();

	PwGisLonLat *platlon = new PwGisLonLat(y, x, this);
	p->append(platlon);
	QString path_id = "path_" + QString::number(id);
	if ( p->length() > 100 )
	{
		p->removeFirst();
	}

	if(p->first() == p->last())
	{
		_pwwidget->removeObject(QString::number(id));
		return;
	}

	_pwwidget->addMarker(QString::number(id), y, x, name, "", 0, "BLA");

	_pwwidget->removeObject(path_id);
	_pwwidget->addPath(path_id, p, "", "", "BLA");

}

/// set point from another thread
void MapClient1::slot_add_evil(int id, QByteArray data)
{
	QMutexLocker lock(&_mux);
	emit signal_new_coord_evil(id, data);

}

void MapClient1::slot_add_ais(QMap<int, QVector<QString> > vec)
{
	QMutexLocker lock(&_mux);
	emit signal_new_ais_data(vec);

}

void MapClient1::slot_niipp_power_cicle(int id, double radius, QByteArray ba)
{
	QMutexLocker lock(&_mux);
	emit signalUpdateCicle(id, radius, ba);
}

void MapClient1::_add_niipp_layer(QString id)
{
	_pwwidget->mapProvider()->layerManager()->addVectorLayer(id, _niipp_layer_name);
}


void MapClient1::slot_update_sector(int id, double radius, double bis, QByteArray ba)
{

	QMutexLocker lock(&_mux);
	emit signalUpdateSector(id, radius, bis, ba);
}

void MapClient1::_slot_update_sector(int id, double radius, double bis, QByteArray ba)
{
	QString post_id = QString::number(id) + " - niipp";

	if(_map_niip_circle.contains(post_id))
	{
		QMap<QString, PwGisLonLat *>::iterator it;
		for(it = _map_niip_circle.begin(); it != _map_niip_circle.end(); ++it)
		{
			QString name = QString::number(id) + " - niipp";
			if(it.key() == name)
			{
				_pwwidget->removeObject(name);
				_map_niip_circle.remove(name);
				break;
			}
		}
	}

	if(!_map_niip_sector.contains(post_id))
	{
		QDataStream ds(&ba, QIODevice::ReadOnly);
		QString name;
		ds >> name;
		QPointF latlon;
		ds >> latlon;
		double width_angle;
		ds >> width_angle;
		_map_niip_sector.insert(post_id, new PwGisLonLat(latlon.y(), latlon.x()));
	}
	PwGisLonLat *l = _map_niip_sector.value(post_id);

	double direction = bis + 12.5;
//    if(_map_niipp_Controller.value(100) != NULL)
//    {
//        _map_niipp_Controller.value(100)->set_angle(direction);
//    }

	if ( direction < 0 )
	{
		direction = 360 + direction;
	}

	direction *= -1;
	direction += 90;
	double enddir = direction + 25;
	qDebug() << "AAAAAAAAAAAAAAAAAAA = " << post_id << direction << enddir;
	_pwwidget->addSlice(post_id, l->lon, l->lat, radius*1000, direction, enddir, "", "", "NIIPP");

//    _pwwidget->update();
	//    _pwwidget->addSlice(post_id, l->lon, l->lat, 170000, direction, direction+1, "", "", "Pelengators");

//    _sliceRadius = radius;
//    _sliceChanged = true;
//    _bis = bis;

//    if(_map_circle.contains(id))
//    {
//        Circle* circle = _map_circle.value(id);
//        circle->removeFromMap();
//        _pwwidget->removeObject(circle->name());
//        _map_circle.remove(id);
//    }

//    if(!_map_sector.contains(id))
//    {

//        QDataStream ds(&ba, QIODevice::ReadOnly);
//        QString name;
//        ds >> name;
//        QPointF latlon;
//        ds >> latlon;
//        double width_angle;
//        ds >> width_angle;
//        Sector *sector = _pwwidget->mapProvider()->objectsFactory()->createSector();
//        sector->setOriginPoint( new PwGisLonLat(latlon.y(), latlon.x()) );
//        sector->setRadius(radius);
//        sector->setName(name);
//        sector->setObjectName(name);

//        _map_sector.insert(id, sector);
////        MapObjectStyle* st = _pwwidget->mapProvider()->styleFactory()->createObjectStyle();
////        st->fill()->
////        _sector->setStyle(st);
//    }

//    Sector *sector = _map_sector.value(id);
//    double radius1 = /*_zone[_rad]*/radius*1000;

//    sector->setRadius(radius1);

//    double sa = _bis - 12.5;
//    if ( sa < 0 ) {
//        sa = 360 + sa;
//    }
////    sa = -90;
//    sa *= -1;
//    sa += 90;

//    qDebug() << "IIIIIIIIIIIIIIIIIIIIII" << sa;

//    sector->setStartAngle( sa);
//    sector->setEndAngle(sa + 25);
//    sector->updateMap();
}

void MapClient1::_slot_update_cicle(int id, double radius, QByteArray ba)
{
	QString post_id = QString::number(id) + " - niipp";

	if(_map_niip_sector.contains(post_id))
	{
//        Sector* sector = _map_niip_sector.value(id);
//        sector->removeFromMap();
		QMap<QString, PwGisLonLat *>::iterator it;
		for(it = _map_niip_sector.begin(); it != _map_niip_sector.end(); ++it)
		{
			QString name = QString::number(id) + " - niipp";
			qDebug() << it.key();
			if(it.key() == name)
			{
				_pwwidget->removeObject(name);
				_map_niip_sector.remove(name);
				break;
			}
		}
	}


	if(!_map_niip_circle.contains(post_id))
	{
		QDataStream ds(&ba, QIODevice::ReadOnly);
		QString name;
		ds >> name;
		QPointF latlon;
		ds >> latlon;
		double width_angle;
		ds >> width_angle;
		_map_niip_circle.insert(post_id, new PwGisLonLat(latlon.y(), latlon.x()));
//        QDataStream ds(&ba, QIODevice::ReadOnly);
//        QString name;
//        ds >> name;
//        QPointF latlon;
//        ds >> latlon;
//        double width_angle;
//        ds >> width_angle;
//        Circle* circle = _pwwidget->mapProvider()->objectsFactory()->createCircle();
//        circle->setOriginPoint( new PwGisLonLat(latlon.y(), latlon.x()) );
//        circle->setRadius(radius);
//        circle->setName(name);
//        circle->setObjectName(name);
//        _map_circle.insert(id, circle);
	}

	PwGisLonLat *l = _map_niip_circle.value(post_id);

	_pwwidget->addCircle(post_id, l->lon, l->lat, radius*1000, "", "", "NIIPP");
	_pwwidget->update();
//    _pwwidget->addSlice(post_id, l->lon, l->lat, radius*1000, direction, enddir, "", "", "BPLA");

//    Circle* circle = _map_circle.value(id);
//    double radius1 = radius*1000;
//    circle->setRadius(radius1);

//    circle->updateMap();
}

void MapClient1::_slot_add_perehvat(int bla_id, int bpla_id)
{
	_map_battle.insert(bla_id, bpla_id);
}

void MapClient1::_slot_update_peleng(int id, int id_post, double lat, double lon, double direction)
{
	QString post_id = QString::number(id_post);// + "-" + QString::number(id);

	if(!_map_peleng.contains(post_id))
	{
		_map_peleng.insert(post_id, new PwGisLonLat(lon, lat));
//        Sector* peleng = _pwwidget->mapProvider()->objectsFactory()->createSector();
//        peleng->setOriginPoint( new PwGisLonLat(lon, lat) );
//        peleng->setRadius(170000);
//        peleng->setName("Peleng" + post_id);

////        peleng->setStyle(_pelengStyle);

//        peleng->setObjectName("Peleng" + post_id);
//        _map_peleng.insert(post_id, peleng);
	}

//    Sector* peleng = _map_peleng.value(post_id);

	PwGisLonLat *l = _map_peleng.value(post_id);

	direction *= -1;
	direction += 90;
	_pwwidget->addSlice(post_id, l->lon, l->lat, 170000, direction, direction+1, "", "", "Pelengators");

//    peleng->setStartAngle(direction);
//    peleng->setEndAngle(direction+1);
//    peleng->updateMap();
}

void MapClient1::slot_peleng(int id, int id_post, double lat, double lon, double direction)
{
	QMutexLocker lock(&_mux);
	emit signal_peleng(id, id_post, lat, lon, direction);
}

/// set point in this thread
void MapClient1::_slot_set_point_evil(int id, QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int time;
	ds >> time;
	int state;
	ds >> state;
	QPointF sko;
	ds >> sko;
	QVector<QPointF> track;
	ds >> track;
	double speed;
	ds >> speed;
	double alt;
	ds >> alt;
	double bearing;
	ds >> bearing;

	if (!track.size()) return;

	QString name;
	name = tr("UAV (#") + QString::number(id) + ")\\n";
	name.append(QString::number(alt, 'f', 1));
	name.append("\\n");
	double sko1 = qSqrt(sko.x()*sko.x() + sko.y()*sko.y());
	name.append(QString::number(sko1, 'f', 4));
//    name.append("\\n");
	//    name.append(QString::number(point.y(), 'f', 4));
	//    name.append("\\n");
	//    name.append("nan");

//    double y = point.y();
//    double x = point.x();
	PwGisPointList *p = new PwGisPointList();

	for(int i = 0; i < track.size(); ++i)
	{
		PwGisLonLat *platlon = new PwGisLonLat(track.at(i).y(), track.at(i).x(), this);
		p->append(platlon);

	}
//    PwGisLonLat *platlon = new PwGisLonLat(y, x, this);

//    p->append(platlon);
	QString path_id = "path_" + QString::number(id);
//    if ( p->length() > 100 )
//    {
//        p->removeFirst();
////        p->pop_front();
//    }

//    if((p->first()->lat == p->last()->lat) && (p->first()->lon == p->last()->lon))
//    {
//        _pwwidget->removeObject(QString::number(id));
//        _db_evil->removeItem(0, _property->get_id());
//        return;
//    }

		_pwwidget->addMarker(QString::number(id), track.at(track.size()-1).y(), track.at(track.size()-1).x(), name, "", 0, "BPLA");

//        _pwwidget->removeObject(path_id);
//        _pwwidget->addPath(path_id, p, "", "", "BPLA");

//        qDebug() << "PATH COUNT!!! = " << track.size();

		_map_niipp_Controller.value(100)->send_evil(track.at(track.size()-1), _point_uvoda_niipp, alt, bearing);
		_map_niipp_Controller.value(101)->send_evil(track.at(track.size()-1), _point_uvoda_niipp, alt, bearing);

}

void MapClient1::_slot_set_point_evil_peleng(int id, QPointF point)
{
	PwGisPointList *p;
	if(_last_coord->contains(id))
	{
		p = _last_coord->value(id);
	}
	else
	{
		p = new PwGisPointList();
		_last_coord->insert(id, p);
	}
	QString name;
	name = tr("UAV Atlant(#") + QString::number(id) + ")\\n";
//    name.append(QString::number(point.x(), 'f', 4));
//    name.append("\\n");
//    name.append(QString::number(point.y(), 'f', 4));
//    name.append("\\n");
//    name.append("nan");

	double y = point.y();
	double x = point.x();
	PwGisLonLat *platlon = new PwGisLonLat(y, x, this);

	p->append(platlon);
	QString path_id = "path_" + QString::number(id);
	if ( p->length() > 100 )
	{
		p->removeFirst();
	}


	QMap<int, int>::iterator it;
	for(it = _map_peleng_point.begin(); it != _map_peleng_point.end(); ++it)
	{
		if(it.value() != id)
		{
			_pwwidget->removeObject(QString::number(id));
			_map_peleng_point.remove(id);
		}
	}

	_pwwidget->addMarker(QString::number(id), y, x, name, "", 0, "PelengatorsPoint");


//    if(p->first() == p->last())
//    {
//        _pwwidget->removeObject(QString::number(id));
//        return;
//    }


//    p->append(platlon);
//    QString path_id = "path_" + QString::number(id);
//    if ( p->length() > 50 )
//    {
//        p->removeFirst();
////        p->pop_front();
//    }
//        _pwwidget->removeObject(path_id);
//        _pwwidget->addPath(path_id, p, "", "", "Pelengators");

	//        _niipp_Controller->send_evil(point, alt);

}

void MapClient1::_slot_set_ais_data(QMap<int, QVector<QString> > map1)
{

	_remove_ais();
	_map_ais = map1;

//        QString height;
//        QString latitute;
//        QString longinude;
//        QString speed;
//        QString namePlane;
//        QString course;

	QMap<int, QVector<QString> >::iterator it;
	for(it = map1.begin(); it != map1.end(); ++it)
	{
		QVector<QString> vec = it.value();
//        for(int i = 0; i < vec.size(); ++i)
//        {
			QString id_str = vec.at(0);
			double lon = vec.at(1).toDouble();
			double lat = vec.at(2).toDouble();
			QString name = vec.at(0);
//            (speed);
//                            vec_str.push_back(height);
//                            vec_str.push_back(course);

			QString tooltip;
//            tooltip.append(vec.at(0));
//            tooltip.append("\\n");
//            tooltip.append(vec.at(1));
//            tooltip.append("\\n");
//            tooltip.append(vec.at(2));
//            tooltip.append("\\n");
//            tooltip.append(vec.at(3));
//            tooltip.append("\\n");
//            tooltip.append(vec.at(4));
//            tooltip.append("\\n");
			tooltip.append(vec.at(5));

			double course = vec.at(5).toDouble();




			_pwwidget->addMarker(id_str, lon, lat, name, tooltip, course, "AIS");
//        }
	}
}


void MapClient1::_slot_simulator()
{
//    int id = 0;
//    static double _angle;
//    _angle += 0.174532925/4;
//    if ( _angle >= 6.25 ) {
//        _angle = 0.0;
//    }

//    double R = 1;
//    double lon = 30 + R * sin( _angle );
//    double lat = 60 + R * cos( _angle );
//    QPointF point(lat, lon);

	static const double lats[23] = {60.075751,60.075123,60.074381,60.074096,60.073696,60.074153,60.075466,60.076722,60.078035,60.079120,60.080662,60.082089,60.082946,60.083574,60.084145,60.084659,60.085058,60.085515,60.085972,60.083345,60.079805,60.077236,60.076151};
	static const double lons[23] = {30.623432,30.626887,30.629766,30.632990,30.635984,30.638978,30.640369,30.641627,30.642663,30.643584,30.644505,30.642563,30.640475,30.638057,30.635408,30.631493,30.628614,30.625045,30.622051,30.614911,30.615372,30.616408,30.620208};

	int id6706 = 6706;
	static int i6706;
	if(i6706 > 22)
	{
		i6706 = 0;
	}
	QPointF point(lats[i6706], lons[i6706]);
	_slot_set_point_bla(id6706, point, 0, 0, 0, 0);
	i6706++;
}



void MapClient1::updateCircle()
{
	if ( !_circleChanged ) {
		return;
	}
	_circleChanged = false;
	_pwwidget->addClassicPolygon("c1",  30.531368, 60.074592, _circleRadius*1000, 40, 0,  "ОП1 Гроза", "", "yellow selectAndDrag");
}

void MapClient1::updateSlice()
{
	if ( !_sliceChanged ) {
		return;
	}
//    _sliceChanged = false;
//    if(!_sector) {
//        _sector = _pwwidget->mapProvider()->objectsFactory()->createSector();
//        _sector->setOriginPoint( new PwGisLonLat(30.531368, 60.074592) );
//        _sector->setRadius(_sliceRadius*100);
//        _sector->setName("Locator");
//    }
//    double sa = _bis - 24.5;
//    if ( sa < 0 ) {
//        sa = 360 + sa;
//    }
//    _sector->setStartAngle( sa);
//    _sector->setEndAngle(sa + 45);
//    _sector->updateMap();



	//_pwwidget->addSlice("s1",  30.531368, 60.074592, _sliceRadius*1000, _bis - 24.5, _bis + 24.5,  "ОП12 Гроза", "", "yellow selectAndDrag");

}

void MapClient1::onMapReady()
{
	foreach( IMapObjectInfo* value, _map_objects)
	{
		dynamic_cast<Sector*>(value)->updateMap();
	}
//    _ready = true;
	_update_timer->start(1000);

	connect(_pwwidget, SIGNAL(mapClicked(double,double)), this, SLOT(_slot_mouse_clicked(double, double)));

}

/// get coordinates
void MapClient1::_slot_mouse_clicked(double lon, double lat)
{
	QString caption;
	caption.append(tr("Diversion point"));
	caption.append("\\n");
	caption.append(QString::number(lat, 'f', 4));
	caption.append("\\n");
	caption.append(QString::number(lon, 'f', 4));
	_pwwidget->addMarker("NIIPMarker", lon, lat, caption, "", 0, "NIIPPPoint");
	_point_uvoda_niipp.setX(lat);
	_point_uvoda_niipp.setY(lon);
	_map_niipp_Controller.value(100)->set_point(_point_uvoda_niipp);
	_map_niipp_Controller.value(101)->set_point(_point_uvoda_niipp);
}

/// read settings for generated (positions)
int MapClient1::_read_settings(QString path_to_ini_file)
{
	int count = 0;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(path_to_ini_file, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QString name;
	double lat;
	double lon;

	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
//        qDebug() << "m_settings = " << childKey.toLatin1();
		m_settings.beginGroup(childKey);

//        TabsProperty *prop = new TabsProperty();

		name = m_settings.value("Name", 0).toString();
		lat = m_settings.value("Latitude", "0").toDouble();
		lon = m_settings.value("Longitude", "0").toDouble();
//        prop->set_id(m_settings.value("Id", 0).toInt());
//        prop->set_name();
//        prop->set_latitude();
//        prop->set_longitude();


//        _layerManager->
		if(name != tr("Ingur"))
		{
			_pwwidget->addMarker(name,  lon, lat, "ОП " + name, "", 0, "stations");
		}


//        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop);
		m_settings.endGroup();
		count++;
	}

	return count;
}

int MapClient1::_read_settings1(QString path_to_ini_file)
{
	int count = 0;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(path_to_ini_file, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QString name;
	double lat;
	double lon;

	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
//        qDebug() << "m_settings = " << childKey.toLatin1();
		m_settings.beginGroup(childKey);

//        TabsProperty *prop = new TabsProperty();

		name = m_settings.value("Name", 0).toString();
		lat = m_settings.value("Latitude", "0").toDouble();
		lon = m_settings.value("Longitude", "0").toDouble();

		double step_x = 0.5/(111.111*cos(lon/(180/3.14)));
		double step_y = 0.5/(111.111);

		QString idname = name + "_point";
//        _layerManager->
		_pwwidget->addRectangle(idname, lon + step_x, lat - step_y, lon - step_x, lat + step_y, name, "", "Control_points");


//        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop);
		m_settings.endGroup();
		count++;
	}

	return count;
}

void MapClient1::_addMarkerLayer(int id, QString name)
{
	QString layerId = QString("layer_%1").arg(++_layersCounter);
	_layerManager->addMarkerLayer(layerId, name);
	_map_layers.insert(id, layerId);
}

void MapClient1::_remove_ais()
{
	QMap<int, QVector<QString> >::iterator it;
	for(it = _map_ais.begin(); it != _map_ais.end(); ++it)
	{
		QVector<QString> vec = it.value();
		QString id_str = vec.at(0);
		_pwwidget->removeMarker(id_str);
	}
	_map_ais.clear();
}

void MapClient1::remove_perehvat(int bla_id, int bpla_id)
{
	emit signalRemovePerehvat(bla_id, bpla_id);
}

void MapClient1::_slot_remove_perehvat(int bla_id, int bpla_id)
{
	_map_battle.remove(bla_id);
}

void MapClient1::perehvat_point(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed)
{
	emit signalPerehvatPoint(bla_id, bpla_id, aCoord, aHgt, aRadius, aTime, aIntcCourse, aIntcSpeed);
}

void MapClient1::slot_remove_point_uvoda()
{
	_pwwidget->removeMarker("NIIPMarker");
}

void MapClient1::_slot_perehvat_point(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed)
{
	QString point_id = QString::number(bla_id) + "-" + QString::number(bpla_id);

	QString caption = tr("Height = ")  + QString::number(aHgt) + "\\n";
	caption.append(tr("Radius = ")  + QString::number(aRadius) + "\\n");
	caption.append(tr("Course = ")  + QString::number(aIntcCourse) + "\\n");
	caption.append(tr("Speed = ")  + QString::number(aIntcSpeed));// + "\\n";


	_pwwidget->addCircle(point_id, aCoord.y(), aCoord.x(), aRadius, caption, "", "Perehvat");
}


