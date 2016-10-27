#include "BLAPerehvatDialog.h"
#include "ui_blaperehvatdialog.h"

BLAPerehvatDialog::BLAPerehvatDialog(IMapClient* mapClient, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::BLAPerehvatDialog)
{
	ui->setupUi(this);

	QPushButton* btnOk = ui->buttonBox->button(QDialogButtonBox::Ok);
	btnOk->setText(tr("Ok"));

	QPushButton* btnCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
	btnCancel->setText(tr("Cancel"));

	Qt::WindowFlags flags=windowFlags();
	Qt::WindowFlags helpFlag=Qt::WindowContextHelpButtonHint;
	flags=flags&(~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":/images/map/images/Tabs/fix_128.png"));

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
	m_dbUav = new DbUavManager( 0, this );
	m_dbUav->setDbController( m_dbUavController );
	m_dbUav->setLifeTime( m_dbBlaSettingsManager->getBlaLifeTime() );
}

BLAPerehvatDialog::~BLAPerehvatDialog()
{
	m_dbUavController->disconnectFromDb();
	delete ui;
}

void BLAPerehvatDialog::init(int id, IDbUavManager* dbUav)
{
	Q_UNUSED( dbUav );

	m_id = id;

	ui->blaLE->setText(tr("UAV #") + QString::number(m_id));

	QList<Uav> enemyUavs;
	m_dbUav->getUavsByRole(ENEMY_UAV_ROLE, enemyUavs);

	QList<QTreeWidgetItem *> items;

    QMap<QString, Uav> knownUavList = dbUav->getKnownUavList();

    foreach (Uav enemyUav, enemyUavs) {
        if(!knownUavList.contains(enemyUav.name)) {
			continue;
		}
		QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_enemy: #%1")).arg(enemyUav.uavId)));

		QList<Target> targets;
		Uav uavFriend = m_dbUav->getUavByUavId(m_id);
		m_dbUav->getTargetsByUavIdAndTargetType(uavFriend.id, "InterceptType", targets);

		if (targets.count() > 0){
			foreach (Target target, targets) {
				if(target.uavId != enemyUav.id) {
					it->setCheckState(0, Qt::Unchecked);
				}
				else {
					it->setCheckState(0, Qt::Checked);
					break;
				}
			}
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
	Q_UNUSED( id );

	int enemyUavId = item->text(0).split(tr("#")).at(1).toInt();

	if(item->checkState(0) == Qt::Checked) {
		int interceptTargetTypeId = m_dbUav->getTargetTypeByName("InterceptType");

		if (INVALID_INDEX == interceptTargetTypeId){
			TargetType interceptTargetType;
			interceptTargetType.name = "InterceptType";
			interceptTargetTypeId = m_dbUav->addTargetType(interceptTargetType);
		}

		Target target;
		UavMission mission;
		Uav uavEnemy = m_dbUav->getUavByUavId(enemyUavId);
		Uav uavFriend = m_dbUav->getUavByUavId(m_id);

		target.uavId = uavEnemy.id;
		target.ip = uavEnemy.ip;
		target.port = 1000;
		target.type = interceptTargetTypeId;
		int targetId = m_dbUav->addTarget(target);

		//creating mission with current target and friendly bla id
		mission.regionCenterAltitude = 0.0;
		mission.regionCenterLat = 0.0;
		mission.regionCenterLon = 0.0;
		mission.regionRadius = 0.0;
		mission.timeToTarget = QTime(0, 0, 0);
		mission.targetId = targetId;
		mission.uavId = uavFriend.id;;
		m_dbUav->addUavMission(mission);

		QList<UavInfo> uavFriendInfoList;
		QList<UavInfo> uavEnemyInfoList;
		m_dbUav->getUavInfoByUavId(uavFriend.id, uavFriendInfoList);
		m_dbUav->getUavInfoByUavId(uavEnemy.id, uavEnemyInfoList);

		m_mapClient->addInterception(m_id, enemyUavId, uavFriendInfoList, uavEnemyInfoList);
	}
	else {
		Uav uavF = m_dbUav->getUavByUavId(m_id);
		QList<UavMission> missionsList;
		QList<Target> targets;
		m_dbUav->getTargetsByUavId(enemyUavId, targets);
		m_dbUav->getUavMissionsByUavId(uavF.id, missionsList);

		foreach (UavMission mission, missionsList) {
			foreach(Target target, targets) {
				if(mission.targetId == target.id) {
					m_dbUav->deleteUavMissionsByTargetId(target.id);
					m_dbUav->deleteTargetsById(target.id);
					m_mapClient->removeInterception(m_id, enemyUavId);
				}
			}
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
