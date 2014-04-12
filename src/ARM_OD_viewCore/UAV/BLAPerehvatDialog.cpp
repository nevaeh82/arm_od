#include "BLAPerehvatDialog.h"
#include "ui_blaperehvatdialog.h"

BLAPerehvatDialog::BLAPerehvatDialog(IMapClient* mapClient, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::BLAPerehvatDialog)
{
	ui->setupUi(this);

	m_mapClient = mapClient;

	m_id = 0;
	m_dbUav = NULL;

	//Creating db bla settings manager
	m_dbBlaSettingsManager = DbBlaSettingsManager::instance();
	m_dbBlaSettingsManager->setIniFile("./DB/db_uav.ini");

	//Creating db bla controller
	m_dbUavController = new DbUavController("DBPEREHVATDIALOGCONNECTION", "QMYSQL", this);
	m_dbUavController->connectToDB(getDbBlaConnectionSettings());

	//Creating db bla manager and set its controller
	m_dbUav = new DbUavManager(this);
	m_dbUav->setDbController(m_dbUavController);
}

BLAPerehvatDialog::~BLAPerehvatDialog()
{
	delete ui;
}

void BLAPerehvatDialog::init(int id, IDbUavManager* dbUav)
{
	m_id = id;
//	m_dbUav = dbUav;

	ui->blaLE->setText(tr("UAV #") + QString::number(m_id));

	QList<Uav> enemyUavs;
	m_dbUav->getUavsByRole(ENEMY_UAV_ROLE, enemyUavs);

	QList<QTreeWidgetItem *> items;
	foreach (Uav enemyUav, enemyUavs){
		QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_enemy: #%1")).arg(enemyUav.uavId)));

		QList<Target> targets;
		m_dbUav->getTargetsByUavId(enemyUav.uavId, targets);

		if (targets.count() > 0){
			it->setCheckState(0, Qt::Checked);
		} else {
			it->setCheckState(0, Qt::Unchecked);
		}

		items.append(it);
	}

	ui->bplaTreeWidget->insertTopLevelItems(0, items);

	connect(ui->bplaTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChangedSlot(QTreeWidgetItem*, int)));
}

void BLAPerehvatDialog::treeItemChangedSlot(QTreeWidgetItem *item, int id)
{
	int enemyUavId = item->text(0).split(tr("#")).at(1).toInt();

	if(item->checkState(0) == Qt::Checked) {
		int interceptTargetTypeId = m_dbUav->getTargetTypeByName("InterceptType");

		if (INVALID_INDEX == interceptTargetTypeId){
			TargetType interceptTargetType;
			interceptTargetType.name = "InterceptType";
			interceptTargetTypeId = m_dbUav->addTargetType(interceptTargetType);
		}

		Target target;
		Uav uav = m_dbUav->getUavByUavId(enemyUavId);

		target.uavId = uav.id;
		target.ip = uav.ip;
		target.port = 1000;
		target.type = interceptTargetTypeId;

		int targetId = m_dbUav->addTarget(target);
		m_mapClient->addInterception(m_id, enemyUavId);
	} else {

		if (m_dbUav->deleteTargetsByUavId(enemyUavId)){
			m_mapClient->removeInterception(m_id, enemyUavId);
		}
	}
}

DBConnectionStruct BLAPerehvatDialog::getDbBlaConnectionSettings()
{
	DBConnectionStruct connectionStruct;

	connectionStruct.dbName = m_dbBlaSettingsManager->getBlaDbName();
	connectionStruct.host = m_dbBlaSettingsManager->getBlaDbHost();
	connectionStruct.login = m_dbBlaSettingsManager->getBlaDbLogin();
	connectionStruct.password = m_dbBlaSettingsManager->getBlaDbPassword();
	connectionStruct.port = m_dbBlaSettingsManager->getBlaDbPort();

	return connectionStruct;
}
