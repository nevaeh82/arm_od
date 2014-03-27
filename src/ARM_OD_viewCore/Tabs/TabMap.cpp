#include "TabMap.h"
#include <QDebug>

TabMap::TabMap(TabsProperty* prop, IRouter* router, ITabManager* tab_manager, QMap<int, QDockWidget* > *map_niipp, IDBManager* db_bla, IDBManager* db_evil)
{
    _router = router;
    _tab_manager = tab_manager;

	_map_niipp = map_niipp;
    _tab_property = prop;
    _id = _tab_property->get_id();
    _name = _tab_property->get_name();

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    _rpc_client1 = NULL;
    _tree_view = NULL;
    _tree_view_BLA = NULL;
    _dock_tree_BLA = NULL;
    _dock_tree_view = NULL;

    _vboxlayout = new QVBoxLayout();
    _hboxlayout = new QHBoxLayout();

    /// create common database manager for spectrum tabs
    _db_manager = static_cast<DBManager *>(db_bla);//new DBManager(_tab_manager);

    QStringList headers_BLA;
    headers_BLA << tr("Название") << tr("Свойство2");
    _model_BLA = new TreeModel(headers_BLA);

    QStringList headers;
    headers << tr("Название") << tr("Свойство1");
    _model = new TreeModel(headers);
    _db_manager_target = static_cast<DBManager *>(db_evil);//new DBManager(_tab_manager);

	_db_manager->set_model(_model_BLA);
	_db_manager_target->set_model(_model);

    _model->set_db(_db_manager_target);
    _model_BLA->set_db(_db_manager);

    _delegate_BLA = new BLAWidgetDelegate();

    start();

    this->setLayout(_hboxlayout);

    connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QVector<QPointF>)), this, SLOT(_slot_get_points_from_rpc(QVector<QPointF>)));

    _init();
}

TabMap::~TabMap()
{
    int error = closeRPC();
    delete _delegate_BLA;
    delete _tree_view;
    delete _tree_view_BLA;
    delete _dock_tree_BLA;
    delete _dock_tree_view;

    delete _vboxlayout;
    delete _hboxlayout;

    delete _db_manager;
	delete _db_manager_target;
}

int TabMap::_init()
{
    int error = createView(this);
    if(error != 0)
    {
        qDebug() << "Cannot create view";
        return error;
    }

    error = createTree();
    if(error != 0)
    {
        qDebug() << "Cannot create tree view";
        return error;
    }

    error = createRPC();
    if(error != 0)
    {
        qDebug() << "Cannot create RPC";
        return error;
    }
    return 0;
}

int TabMap::start()
{
    emit signalStartRPC();

    return 0;
}

int TabMap::stop()
{    
    emit signalStopRPC();
    return 0;
}

void TabMap::show()
{
    emit signalStartRPC();
    this->show();
}

void TabMap::hide()
{
    emit signalStopRPC();
    this->hide();
}

int TabMap::createRPC()
{
	readSettings();
	_rpc_client1 = new RPCClient(_tab_property, _db_manager, _db_manager_target, _tab_manager->get_map_controller(), this, _tab_manager, this);
	_rpc_client1->start(m_rpcHostPort, QHostAddress(m_rpcHostAddress));
	/*
	QThread *thread_rpc_client = new QThread;
    qDebug() << "create thread for rpc client ";

    connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

    connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
    connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
    connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

    connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
    connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
    connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));

    _rpc_client1->setParent(0);
    _rpc_client1->moveToThread(thread_rpc_client);
	thread_rpc_client->start();*/

    return 0;
}

void TabMap::readSettings()
{
	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("./Tabs/RPC.ini");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings settings(settingsFile, QSettings::IniFormat);

	settings.setIniCodec(codec);

	m_rpcHostAddress = settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_rpcHostPort = settings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toInt();
}

int TabMap::closeRPC()
{
    emit signalFinishRPC();
    return 0;
}

int TabMap::createView(QWidget* view)
{
    _hboxlayout->insertWidget(1, _tab_manager->get_map_controller()->get_widget() , Qt::AlignJustify);
    return 0;
}

int TabMap::createTree()
{
    QStringList headers;
    headers << tr("Название") << tr("Свойство");
	_tree_view_BLA = new TreeView();

    _tree_view_BLA->setMinimumWidth(250);
    _tree_view_BLA->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    _tree_view_BLA->get_tree()->setMinimumWidth(250);
    _tree_view_BLA->get_tree()->setModel(_model_BLA);
	_tree_view_BLA->get_tree()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(_model_BLA, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(_slot_tree_BLA_Changed(QModelIndex, QModelIndex)));
    connect(this, SIGNAL(signalTreeBLAUpdate(int)), _tree_view_BLA->get_tree(), SLOT(resizeColumnToContents(int)));

    _tree_view_BLA->get_tree()->resizeColumnToContents(0);
    _tree_view_BLA->get_tree()->resizeColumnToContents(1);

    _tree_view_BLA->get_tree()->setItemDelegate(_delegate_BLA);

    _dock_tree_BLA = new QDockWidget(tr("Список БЛА"));
    _dock_tree_BLA->setWidget(_tree_view_BLA);

    connect(_tree_view_BLA->get_tree(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(_slot_tree_bla_double_clicked(QModelIndex)));

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(_dock_tree_BLA, 0, Qt::AlignJustify);
    vbox->addWidget(_map_niipp->value(1), 0, Qt::AlignBottom);

	_hboxlayout->insertLayout(0, vbox);

    _tree_view = new TreeView(/*headers*/);
    _tree_view->setMinimumWidth(250);
    _tree_view->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    _tree_view->get_tree()->setMinimumWidth(250);

    _tree_view->get_tree()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _tree_view->get_tree()->resizeColumnToContents(0);
    _tree_view->get_tree()->resizeColumnToContents(1);

    _tree_view->get_tree()->setItemDelegate(_delegate_BLA);

    _tree_view->get_tree()->setModel(_model);

    _dock_tree_view = new QDockWidget(tr("Список целей"));
    _dock_tree_view->setWidget(_tree_view);

    _vboxlayout->addWidget(_dock_tree_view, 0, Qt::AlignJustify);
    _vboxlayout->addWidget(_map_niipp->value(2), 0, Qt::AlignBottom);
	_hboxlayout->insertLayout(2, _vboxlayout);

    connect(_tab_manager->get_map_controller()->pb_show_BLA_tree, SIGNAL(clicked()), this, SLOT(slot_show_tree_BLA()));
    connect(_tab_manager->get_map_controller()->pb_show_BPLA_tree, SIGNAL(clicked()), this, SLOT(slot_show_tree_BPLA()));

    connect(_tab_manager->get_map_controller()->pb_show_NiiPP, SIGNAL(clicked()), this, SLOT(slot_show_niipp()));
    return 0;
}

TabsProperty* TabMap::get_tab_property()
{
    return _tab_property;
}

void TabMap::set_command(IMessageOld *msg)
{
	_rpc_client1->setCommand(msg);
}

/// in this thread set points from rpc
void TabMap::_slot_get_points_from_rpc(QVector<QPointF> points)
{
}

void TabMap::_slot_tree_bla_double_clicked(QModelIndex index)
{
    BLAPerehvatDialog *b = new BLAPerehvatDialog(_tab_manager->get_map_controller()->get_map_client(1));
    b->init((index.data()).toInt(), _db_manager, _db_manager_target);
    b->show();
    qDebug() << "Got double click!";
}

void TabMap::_slot_tree_BLA_Changed(QModelIndex index, QModelIndex index1)
{
}

/// show/hide BLA tree
void TabMap::slot_show_tree_BLA()
{
    if(_dock_tree_BLA == NULL)
    {
        return;
    }

    _dock_tree_BLA->show();
}

void TabMap::slot_show_tree_BPLA()
{
    if(_dock_tree_view == NULL)
    {
        return;
    }

    _dock_tree_view->show();
}

void TabMap::slot_show_niipp()
{
    QMap<int, QDockWidget* >::iterator it;
    for(it = _map_niipp->begin(); it != _map_niipp->end(); ++it)
    {
        it.value()->show();
    }
}
