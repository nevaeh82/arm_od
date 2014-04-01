#include "MapTabWidgetController.h"

#include <QDebug>

MapTabWidgetController::MapTabWidgetController(Station *station, QMap<int, Station *> map_settings, ITabManager* tabManager, IDBManager* db_bla, IDBManager* db_evil, QObject* parent) :
	QObject(parent)
{
	m_view = NULL;
	m_niipp1 = NULL;
	m_niipp2 = NULL;
	m_rpcClient = NULL;

	_tab_manager = tabManager;

	/// create map controller
	m_mapController = new MapController(this);

	m_station = station;

	QStringList headers;
	headers << tr("Property") << tr("Value");

	m_blaModel = new TreeModel(headers);
	m_bplaModel = new TreeModel(headers);

	/// TODO: refactor
	m_bplaDbManager = static_cast<DBManager *>(db_evil);
	m_blaDbManager = static_cast<DBManager *>(db_bla);

	/// TODO: refactor
	m_blaDbManager->set_model(m_blaModel);
	m_bplaDbManager->set_model(m_bplaModel);

	m_mapSettings = map_settings;

	m_bplaModel->set_db(m_bplaDbManager);
	m_blaModel->set_db(m_blaDbManager);

	m_treeDelegate = new BLAWidgetDelegate();

	start();
}

MapTabWidgetController::~MapTabWidgetController()
{
	closeRPC();

	/// TODO: delete
	delete m_treeDelegate;
	delete m_blaDbManager;
	delete m_bplaDbManager;
}

int MapTabWidgetController::init()
{
	int error = createTree();

	if(error != 0) {
		qDebug() << "Cannot create tree view";
		return error;
	}

	error = createRPC();
	if(error != 0) {
		qDebug() << "Cannot create RPC";
		return error;
	}

	return 0;
}

int MapTabWidgetController::start()
{
	emit signalStartRPC();

	return 0;
}

int MapTabWidgetController::stop()
{
	emit signalStopRPC();
	return 0;
}

void MapTabWidgetController::show()
{
	emit signalStartRPC();
	this->show();
}

void MapTabWidgetController::hide()
{
	emit signalStopRPC();
	this->hide();
}

int MapTabWidgetController::createRPC()
{
	readSettings();
	m_rpcClient = new RPCClient(m_station, m_blaDbManager, m_bplaDbManager, m_mapController, this, _tab_manager, this);
	m_rpcClient->start(m_rpcHostPort, QHostAddress(m_rpcHostAddress));

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

int MapTabWidgetController::closeRPC()
{
	emit signalFinishRPC();
	return 0;
}


int MapTabWidgetController::createTree()
{
	m_view->getBlaTreeView()->setModel(m_blaModel);
	m_view->getBlaTreeView()->setItemDelegate(m_treeDelegate);

	connect(m_view->getBlaTreeView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onBlaTreeItemDoubleClicked(QModelIndex)));

	m_view->getBplaTreeView()->setModel(m_bplaModel);
	m_view->getBplaTreeView()->setItemDelegate(m_treeDelegate);

	connect(m_view->getControlPanelWidget(), SIGNAL(showBlaClicked()), this, SLOT(onShowBlaTree()));
	connect(m_view->getControlPanelWidget(), SIGNAL(showBplaClicked()), this, SLOT(onShowBplaTree()));
	connect(m_view->getControlPanelWidget(), SIGNAL(showNiippClicked()), this, SLOT(onShowNiipp()));

	return 0;
}

void MapTabWidgetController::set_command(IMessageOld *msg)
{
	m_rpcClient->setCommand(msg);
}

void MapTabWidgetController::appendView(MapTabWidget *view)
{
	m_view = view;

	m_mapController->appendView(m_view->getMapWidget());
	m_mapController->init(m_mapSettings, m_blaDbManager, m_bplaDbManager);

	/// WTF?
	QPointF latlon1;
	latlon1.setX(42.511183);
	latlon1.setY(41.6905);
	m_niipp1 = new NiippController(100, tr("СПИП ДД-1"), latlon1, m_mapController, _tab_manager);

	QPointF latlon2;
	latlon2.setX(42.634183);
	latlon2.setY(41.912167);
	m_niipp2 = new NiippController(101, tr("СПИП ДД-2"), latlon2, m_mapController, _tab_manager);

	m_niipp1->appendView(m_view->getNiippWidget(1));
	m_niipp2->appendView(m_view->getNiippWidget(2));

	init();
}

void MapTabWidgetController::openAtlasSlot()
{
	m_mapController->openMapFromAtlas();
}

void MapTabWidgetController::openMapSlot()
{
	m_mapController->openMapFromLocalFile();
}


void MapTabWidgetController::onBlaTreeItemDoubleClicked(QModelIndex index)
{
	BLAPerehvatDialog *b = new BLAPerehvatDialog(m_mapController->get_map_client(1));
	b->init((index.data()).toInt(), m_blaDbManager, m_bplaDbManager);
	b->show();
	qDebug() << "Got double click!";
}

void MapTabWidgetController::onShowBlaTree()
{
	m_view->changeBlaTreeVisibility();
}

void MapTabWidgetController::onShowBplaTree()
{
	m_view->changeBplaTreeVisibility();
}

void MapTabWidgetController::onShowNiipp()
{
	m_view->changeNiippVisibility();
}

void MapTabWidgetController::onSendDataToNiippControl(int id, QByteArray data)
{
	switch(id)
	{
		case 100:
			m_niipp1->setData(data);
			break;
		case 101:
			m_niipp2->setData(data);
			break;
		default:
			break;
	}
}

void MapTabWidgetController::readSettings()
{
	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("./Tabs/RPC.ini");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings settings(settingsFile, QSettings::IniFormat);

	settings.setIniCodec(codec);

	m_rpcHostAddress = settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_rpcHostPort = settings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toInt();
}
