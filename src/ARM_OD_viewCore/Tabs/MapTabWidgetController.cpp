#include "MapTabWidgetController.h"

#include <QDebug>

MapTabWidgetController::MapTabWidgetController(Station *station, QMap<int, Station *> map_settings, ITabManager* tabManager, DbUavManager* db_bla, DBManager* db_evil, QObject* parent) :
	QObject(parent)
{
	m_view = NULL;
	m_niipp1 = NULL;
	m_niipp2 = NULL;
	m_rpcClient = NULL;

	m_tabManager = tabManager;

	/// create map controller
	m_mapController = new MapController(this);

	m_station = station;

	/// TODO: refactor
	m_bplaDbManager = db_evil;
	m_uavDbManager = db_bla;

	QStringList headers;
	headers << tr("Property") << tr("Value");


	m_allyUavTreeModel =  new UavTreeModel(headers, this);
	m_allyUavTreeModel->setTargetRole(OUR_UAV_ROLE);
	m_uavDbManager->registerReceiver(m_allyUavTreeModel);


	m_enemyUavTreeModel = new UavTreeModel(headers, this);
	m_enemyUavTreeModel->setTargetRole(ENEMY_UAV_ROLE);
	m_uavDbManager->registerReceiver(m_enemyUavTreeModel);

	m_mapSettings = map_settings;

	m_treeDelegate = new BLAWidgetDelegate(this);

	start();
}

MapTabWidgetController::~MapTabWidgetController()
{
	closeRPC();

	/// TODO: delete
	delete m_bplaDbManager;
}

int MapTabWidgetController::init()
{
	int error = createTree();

	if(error != 0) {
		log_debug("Cannot create tree view");
		return error;
	}

	error = createRPC();
	if(error != 0) {
		log_debug("Cannot create RPC");
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

	///TODO: fix deleting

	m_rpcClient = new RPCClient(m_station, m_uavDbManager, m_bplaDbManager, m_mapController, this, m_tabManager);

	m_rpcClient->start(m_rpcHostPort, QHostAddress(m_rpcHostAddress));

	QThread* rpcClientThread = new QThread;
	connect(m_rpcClient, SIGNAL(destroyed()), rpcClientThread, SLOT(terminate()));
	m_rpcClient->moveToThread(rpcClientThread);
	rpcClientThread->start();

	return 0;
}

int MapTabWidgetController::closeRPC()
{
	emit signalFinishRPC();
	return 0;
}


int MapTabWidgetController::createTree()
{
	m_view->getBlaTreeView()->setModel(m_allyUavTreeModel);
	m_view->getBlaTreeView()->setItemDelegate(m_treeDelegate);
	connect(m_allyUavTreeModel, SIGNAL(onItemAddedSignal()), m_view->getBlaTreeView(), SLOT(expandAll()));

	connect(m_view->getBlaTreeView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onBlaTreeItemDoubleClicked(QModelIndex)));

	m_view->getBplaTreeView()->setModel(m_enemyUavTreeModel);
	m_view->getBplaTreeView()->setItemDelegate(m_treeDelegate);
	connect(m_enemyUavTreeModel, SIGNAL(onItemAddedSignal()), m_view->getBplaTreeView(), SLOT(expandAll()));

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
	m_mapController->init(m_mapSettings);

	/// WTF?
	QPointF latlon1;
	latlon1.setX(42.511183);
	latlon1.setY(41.6905);
	m_niipp1 = new NiippController(100, tr("SPIP DD-1"), latlon1, m_mapController, m_tabManager);

	QPointF latlon2;
	latlon2.setX(42.634183);
	latlon2.setY(41.912167);
	m_niipp2 = new NiippController(101, tr("SPIP DD-2"), latlon2, m_mapController, m_tabManager);

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
	BLAPerehvatDialog *b = new BLAPerehvatDialog(m_view);
	b->init((index.data()).toInt(), m_uavDbManager, m_bplaDbManager);

	/// TODO: This dialog should be modal
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

