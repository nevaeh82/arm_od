#include <QDebug>
#include <QHeaderView>

#include <TreeModel/TreeItem.h>

#include "MapTabWidgetController.h"

#define TREE_KEYS_COLUMN_MIN_WIDTH 170

MapTabWidgetController::MapTabWidgetController(Station *station, QMap<int, Station *> map_settings, ITabManager* tabManager,
											   DbUavManager* db_bla, SolverSetupWidgetController* solverSetup,
											   QObject* parent) :
	QObject(parent)
{
	m_view = NULL;
	m_niipp1 = NULL;
	m_niipp2 = NULL;
	m_rpcClient = NULL;
	m_station = station;
	m_tabManager = tabManager;

	m_solverSetup = solverSetup;

	/// TODO: refactor
	m_uavDbManager = db_bla;

	/// create map controller
	m_mapController = new MapController( this );
	connect( m_mapController, SIGNAL( mapOpened() ), SIGNAL( mapOpened() ) );
	connect( m_mapController, SIGNAL( atlasOpened() ), SIGNAL( atlasOpened() ) );
	connect( m_mapController, SIGNAL( cancelMapOpen() ), SIGNAL( cancelMapOpen() ) );

	connect( m_mapController, SIGNAL(mapClicked(double,double)), m_solverSetup, SLOT(slotMapClicked(double, double)) );

	m_uavDbManager->registerReceiver( m_mapController );
	m_uavDbManager->getUavHistory()->registerReceiver( m_mapController );

	QStringList headers;
	headers << tr("Property") << tr("Value");

	m_allyUavTreeModel =  new UavTreeModel(headers, this);
	m_allyUavTreeModel->setTargetRole(OUR_UAV_ROLE);
	m_uavDbManager->registerReceiver(m_allyUavTreeModel);
	m_uavDbManager->getUavHistory()->registerReceiver( m_allyUavTreeModel );

	m_enemyUavTreeModel = new UavTreeModel(headers, this);
	m_enemyUavTreeModel->setTargetRole(ENEMY_UAV_ROLE);
	m_uavDbManager->registerReceiver(m_enemyUavTreeModel);
	m_uavDbManager->getUavHistory()->registerReceiver( m_enemyUavTreeModel );

	m_mapSettings = map_settings;

	m_treeDelegate = new TreeWidgetDelegate(this);

	m_controlPanelController =  new ControlPanelController(this);
	m_uavDbManager->getUavHistory()->registerReceiver( m_controlPanelController );

	connect(this, SIGNAL(onDbOutLog(QString)), m_controlPanelController, SLOT(onDbLogReceive(QString)));
	connect(m_uavDbManager, SIGNAL(signalWriteToBd(int)), m_controlPanelController, SLOT(slotStateWriteToBd(int)));

	connect(this, SIGNAL(signalOnExtraBoardInfo(int)), m_allyUavTreeModel, SLOT(onExtraBoardInfo(int)));
	connect(this, SIGNAL(signalOnExtraBoardInfo(int)), m_enemyUavTreeModel, SLOT(onExtraBoardInfo(int)));
	connect(this, SIGNAL(signalOnExtraBoardInfo(int)), m_mapController, SLOT(onExtraBoardInfo(int)));
	connect(this, SIGNAL(signalOnExtraBoardInfo(int)), m_mapController, SIGNAL(signalOnExtraBoardInfo(int)));

	start();
}

MapTabWidgetController::~MapTabWidgetController()
{
	m_rpcClient->deregisterReceiver(m_uavDbManager);
	m_rpcClient->deregisterReceiver(m_mapController);
	m_rpcClient->deregisterReceiver(m_niipp1);
	m_rpcClient->deregisterReceiver(m_niipp2);
	m_rpcClient->deregisterReceiver( m_solverSetup );

	m_rpcClient->stop();

	m_mapController->saveCache();

	m_mapController->closeAtlas();
	m_mapController->closeMap();

	closeRPC();

	m_uavDbManager->deregisterReceiver(m_allyUavTreeModel);
	m_uavDbManager->deregisterReceiver(m_enemyUavTreeModel);
	m_uavDbManager->deregisterReceiver(m_mapController);

	m_uavDbManager->getUavHistory()->deregisterReceiver(m_mapController);
	m_uavDbManager->getUavHistory()->deregisterReceiver(m_allyUavTreeModel);
	m_uavDbManager->getUavHistory()->deregisterReceiver(m_enemyUavTreeModel);
	m_uavDbManager->getUavHistory()->deregisterReceiver( m_controlPanelController );

	m_uavDbManager->deregisterReceiver( m_niipp1 );
	m_uavDbManager->deregisterReceiver( m_niipp2 );

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

	///TODO: fix deleting
	m_rpcClient = new RpcClientWrapper;

	QThread* rpcClientThread = new QThread;
	connect(m_rpcClient, SIGNAL(destroyed()), rpcClientThread, SLOT(terminate()));
	//connect(this, SIGNAL(signalFinishRPC()), rpcClientThread, SLOT(quit()));
	//connect(this, SIGNAL(signalFinishRPC()), m_rpcClient, SLOT(deleteLater()));
	//connect(this, SIGNAL(signalFinishRPC()), rpcClientThread, SLOT(deleteLater()));

	m_rpcClient->moveToThread(rpcClientThread);
	rpcClientThread->start();

	m_rpcClient->init(m_rpcHostPort, QHostAddress(m_rpcHostAddress), m_station);

	QEventLoop loop;
	connect(m_rpcClient, SIGNAL(initFinishedSignal()), &loop, SLOT(quit()));
	loop.exec();

	m_rpcClient->registerReceiver(m_mapController);
	m_rpcClient->registerReceiver(m_uavDbManager);

//	m_rpcClient->registerReceiver(m_niipp1);
//	m_rpcClient->registerReceiver(m_niipp2);

	m_rpcClient->registerReceiver( m_solverSetup );

	return 0;
}

int MapTabWidgetController::closeRPC()
{
	emit signalFinishRPC();
	return 0;
}


int MapTabWidgetController::createTree()
{
	QTreeView *tree;

	tree = m_view->getBlaTreeView();
	tree->setModel(m_allyUavTreeModel);
	//tree->setItemDelegate(m_treeDelegate);
	tree->header()->resizeSection(0, qMax( TREE_KEYS_COLUMN_MIN_WIDTH, tree->header()->width() / 2 ) );
	tree->header()->resizeSection(1, tree->header()->width() - tree->header()->sectionSize(0) );

	connect(m_allyUavTreeModel, SIGNAL(onItemAddedSignal()), tree, SLOT(expandAll()));

	//connect(tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onBlaTreeItemDoubleClicked(QModelIndex)));

	tree = m_view->getBplaTreeView();
	tree->setModel(m_enemyUavTreeModel);
	//tree->setItemDelegate(m_treeDelegate);
	tree->header()->resizeSection(0, qMax( TREE_KEYS_COLUMN_MIN_WIDTH, tree->header()->width() / 2 ) );
	tree->header()->resizeSection(1, tree->header()->width() - tree->header()->sectionSize(0) );

	//connect(m_enemyUavTreeModel, SIGNAL(onItemAddedSignal()), tree, SLOT(expandAll()));

	return 0;
}

void MapTabWidgetController::set_command(IMessageOld *msg)
{
	m_rpcClient->setCommand(msg);
}

void MapTabWidgetController::appendView(MapTabWidget *view)
{
	m_view = view;

	m_controlPanelController->appendView(m_view->getControlPanelWidget());
	m_controlPanelController->setUavHistory(m_uavDbManager->getUavHistory());

	m_mapController->appendView(m_view->getMapWidget());
	m_mapController->init(m_mapSettings);

	connect( this, SIGNAL(initFinished()), m_mapController, SLOT(loadMapSettings()), Qt::QueuedConnection );

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
	connect(this, SIGNAL(signalStartRPC()), m_niipp1, SLOT(onGetConnectionStatus()));
	connect(this, SIGNAL(signalStartRPC()), m_niipp2, SLOT(onGetConnectionStatus()));

	m_uavDbManager->registerReceiver( m_niipp1 );
	m_uavDbManager->registerReceiver( m_niipp2 );

	connect(m_uavDbManager, SIGNAL(protoToNiipp(QByteArray)), m_niipp1, SLOT());

	init();

	emit initFinished();

	connect(this, SIGNAL(signalOnExtraBoardInfo(int)), m_view->getMapWidget(), SIGNAL(signalOnExtraBoardInfo(int)));
}

void MapTabWidgetController::setRpcConfig(const quint16& port, const QString& host)
{
	m_rpcHostAddress = host;
	m_rpcHostPort = port;
}

void MapTabWidgetController::setARMRConnection(bool b)
{
	m_controlPanelController->setARMRConnection(b);
}

void MapTabWidgetController::openAtlasSlot()
{
	m_mapController->openMapFromAtlas();
}

void MapTabWidgetController::openMapSlot()
{
	m_mapController->openMapFromLocalFile();
}

void MapTabWidgetController::clearMapSlot()
{
	m_mapController->clearObjects();
}

void MapTabWidgetController::loadObjetsSlot()
{
	m_mapController->loadObjects();
}

void MapTabWidgetController::saveObjectsSlot()
{
	m_mapController->saveObjects();
}

void MapTabWidgetController::onBlaTreeItemDoubleClicked(QModelIndex index)
{

	return ; //Do not neet now ...


	QModelIndex parent = index.parent();

	if (parent.isValid()){
		parent = parent.sibling(parent.row(), 0);
	} else {
		parent = index.sibling(index.row(), 0);
	}

	// check if it is historical item
	// we do this by checking is name contains only digits
	// because real UAVs should contains only numbers
	bool isNumber;
	TreeItem* item = static_cast<TreeItem *>(parent.internalPointer());
	item->data().name.toInt( &isNumber );
	if( !isNumber ) return;

	int data = parent.data().toInt();

	BLAPerehvatDialog *b = new BLAPerehvatDialog(m_mapController->getMapClient(1), m_view);
	b->init(data, m_uavDbManager);

	/// TODO: This dialog should be modal
	b->show();
	qDebug() << "Got double click!";
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
