#include <Logger.h>
#include <TreeModel/TreeItem.h>
#include <Settings/SettingsNode.h>

#include "UavTreeModel.h"
#include "Tabs/DbBla/Defines.h"

#define LAT_PROPERTY_ID		1
#define LON_PROPERTY_ID		2
#define ALT_PROPERTY_ID		3

#define LAT_KTR_PROPERTY_ID		4
#define LON_KTR_PROPERTY_ID		5

UavTreeModel::UavTreeModel(const QStringList &headers, QObject *parent) :
	TreeModelBase(headers, parent)
{
	m_treeUpdater.setInterval(500);
	m_treeUpdater.start();

	connect(&m_treeUpdater,SIGNAL(timeout()),this,SLOT(updateData()));

	m_isNeedRedraw = false;

	m_targetRole = OUR_UAV_ROLE;
}

UavTreeModel::~UavTreeModel()
{

}

void UavTreeModel::updateData()
{
	if(m_isNeedRedraw){

		refreshModel();

		m_isNeedRedraw = false;
	}
}

QVariant UavTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	TreeItem* item =  static_cast<TreeItem *>(index.internalPointer());

	if (!item) {
		return QVariant();
	}

	if (role == Qt::CheckStateRole) {

		if ((item->data().pid != 0) || (index.column() != 0)) {
			return QVariant();
		}

		return ((item->data().state > 0) ? Qt::Checked : Qt::Unchecked);

	}

	return TreeModelBase::data(index, role);
}


QString UavTreeModel::getTranslateItemNameFromReal(const QString &itemName) const
{
	return itemName;
}

bool UavTreeModel::updateModelData(TreeItem *item)
{
	Q_UNUSED(item)

	return false;
}

void UavTreeModel::addSourceNode(TreeItem* item, uint sourceType, QString name, uint uavId)
{
	SettingsNode nodeSolverAuto;

	Property inProperty;
	inProperty.pid = uavId;
	inProperty.isEditable = false;
	inProperty.state = 0;

	nodeSolverAuto.object.id = sourceType;
	nodeSolverAuto.object.name = name;
	nodeSolverAuto.object.isEditable = false;
	nodeSolverAuto.object.pid = 0;
	nodeSolverAuto.object.state = 0;

	inProperty.id = LAT_PROPERTY_ID;
	inProperty.name = tr("lat");
	inProperty.value = 0;
	nodeSolverAuto.properties.append(inProperty);

	inProperty.id = LON_PROPERTY_ID;
	inProperty.name = tr("lon");
	inProperty.value = 0;
	nodeSolverAuto.properties.append(inProperty);

	if ( sourceType != UAV_SLICES_SOURCE ) {
		inProperty.id = ALT_PROPERTY_ID;
		inProperty.name = tr("alt");
		inProperty.value = 0;
		nodeSolverAuto.properties.append(inProperty);
	}

	TreeItem* sourceItem =  new TreeItem(nodeSolverAuto.object, item);

	foreach (Property property, nodeSolverAuto.properties) {
		TreeItem* childItem =  new TreeItem(property, sourceItem);
		sourceItem->appendChild(childItem);
	}

	item->appendChild(sourceItem);
}

void UavTreeModel::onUavAdded(const Uav &uav, const QString& uavRole)
{
	if (uavRole != m_targetRole){
		return;
	}

	//QString name = uav.name;
	SettingsNode inSettingsNode;
	inSettingsNode.object.id = uav.uavId;
	inSettingsNode.object.name = uav.name;
	inSettingsNode.object.isEditable = false;
	inSettingsNode.object.pid = 0;
	inSettingsNode.object.state = 0;

	emit layoutAboutToBeChanged();

	TreeItem* item =  new TreeItem(inSettingsNode.object, m_rootItem);

	foreach (Property property, inSettingsNode.properties) {
		TreeItem* childItem =  new TreeItem(property, item);
		item->appendChild(childItem);
	}

	m_rootItem->appendChild(item);

	if ( uavRole == OUR_UAV_ROLE ) {
		addSourceNode( item, UAV_AUTOPILOT_SOURCE, tr("Autopilot"), uav.uavId );
		addSourceNode( item, UAV_SLICES_SOURCE, tr("KTR"), uav.uavId );
	} else
	if ( uavRole == ENEMY_UAV_ROLE ) {
		addSourceNode( item, UAV_SOLVER_AUTO_SOURCE, tr("Auto mode"), uav.uavId );
		addSourceNode( item, UAV_SOLVER_MANUAL_SOURCE, tr("Manual mode"), uav.uavId );
		addSourceNode( item, UAV_SOLVER_SINGLE_1_SOURCE, tr("Single mode: Point 1"), uav.uavId );
		addSourceNode( item, UAV_SOLVER_SINGLE_2_SOURCE, tr("Single mode: Point 2"), uav.uavId );
	}

	emit layoutChanged();
	emit onItemAddedSignal();
}

void UavTreeModel::onUavRemoved(const Uav &uav, const QString& uavRole)
{
	if (uavRole != m_targetRole){
		return;
	}

	for (int i= 0; i< m_rootItem->childCount(); ++i) {

		TreeItem* childItem = m_rootItem->child(i);
		if (childItem->data().id != uav.uavId) {
			continue;
		}

		// check is UAV record is loaded from history
		// we do this by checking is name contains only digits
		// because real UAVs should contains only numbers
		bool isReal;
		childItem->data().name.toInt( &isReal );

		if (uav.historical == isReal) continue;

		emit layoutAboutToBeChanged();
		m_rootItem->removeChild(childItem);
		emit layoutChanged();
		break;
	}
}

void UavTreeModel::onUavInfoChanged(const UavInfo &uavInfo, const QString& uavRole,
									const QVector<QPointF>& tail,
									const QVector<QPointF>& tailStdDev)
{
	Q_UNUSED( tail );
	Q_UNUSED( tailStdDev );

	if (uavRole != m_targetRole){
		return;
	}

	QString number;

	onPropertyChanged(uavInfo, LAT_PROPERTY_ID, tr("lat"), number.sprintf( "%.4f", uavInfo.lat ));
	onPropertyChanged(uavInfo, LON_PROPERTY_ID, tr("lon"), number.sprintf( "%.4f", uavInfo.lon ));

	if (UAV_SLICES_SOURCE != uavInfo.source) {
		onPropertyChanged(uavInfo, ALT_PROPERTY_ID, tr("alt"), QString::number( (int) uavInfo.alt ));
	}

	if (m_isNeedRedraw) {
		return;
	}

	m_isNeedRedraw = true;

	//refreshModel();
}

void UavTreeModel::setTargetRole(const QString &role)
{
	m_targetRole = role;
}

void UavTreeModel::onPropertyChanged(const UavInfo &uavInfo, const uint propId, const QString &name, const QVariant &value)
{
	/// TODO: need to uncomment after imlementing of speed receiving from KTR
	bool isInFlight = true;

	if (uavInfo.source != UAV_SLICES_SOURCE) {
		isInFlight = /*(uavInfo.speed > 0) && */(uavInfo.alt > 0);
	}

	Property inProperty;
	inProperty.pid = uavInfo.uavId;
	inProperty.id = propId;
	inProperty.isEditable = false;
	inProperty.state = 0;
	inProperty.name = name;
	inProperty.value = value;

	for (int i = 0; i< m_rootItem->childCount(); i++) {

		TreeItem* stationItem = m_rootItem->child(i);

		if (stationItem->data().id != inProperty.pid) {
			continue;
		}

		// check is UAV record is loaded from history
		// we do this by checking is name contains only digits
		// because real UAVs should contains only numbers
		bool isReal;
		stationItem->data().name.toInt( &isReal );

		if (uavInfo.historical == isReal) continue;

		int sourceState = isInFlight ? 1 : 0;;
		int stationState = sourceState;

		BaseItem data;
		TreeItem* sourceItem = NULL;

		// calculate common station state and try to find suitable source item
		for (int j = 0; j< stationItem->childCount(); j++) {
			data = stationItem->child(j)->data();

			if (stationState == 0 && data.state == 1) {
				stationState = 1;
			}

			if (sourceItem  == NULL && data.id == uavInfo.source) {
				sourceItem  = stationItem->child(j);
			}
		}

		if (sourceItem == NULL) continue;

		// update station item state
		data = stationItem->data();
		data.state = stationState;
		stationItem->updateData(data);

		// update source item state
		data = sourceItem->data();
		data.state = sourceState;
		sourceItem->updateData(data);

		// update source item property
		for (int j = 0; j< sourceItem->childCount(); j++) {
			if (sourceItem->child(j)->data().id == inProperty.id) {
				sourceItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}
}
