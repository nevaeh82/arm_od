#include "UavTreeModel.h"

#include <TreeModel/TreeItem.h>
#include <Settings/SettingsNode.h>

#include "../DbBla/Defines.h"

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

void UavTreeModel::onUavAdded(const Uav &uav, const QString& uavRole)
{
	if (uavRole != m_targetRole){
		return;
	}

	SettingsNode inSettingsNode;
	inSettingsNode.object.id = uav.uavId;
	inSettingsNode.object.name = uav.name;
	inSettingsNode.object.isEditable = false;
	inSettingsNode.object.pid = 0;
	inSettingsNode.object.state = 0;

	Property inProperty;
	inProperty.pid = uav.uavId;
	inProperty.id = LAT_PROPERTY_ID;
	inProperty.isEditable = false;
	inProperty.state = 0;
	inProperty.name = tr("lat");
	inProperty.value = 0;

	inSettingsNode.properties.append(inProperty);

	inProperty.id = LON_PROPERTY_ID;
	inProperty.name = tr("lon");
	inProperty.value = 0;
	inSettingsNode.properties.append(inProperty);

	inProperty.id = ALT_PROPERTY_ID;
	inProperty.name = tr("alt");
	inProperty.value = 0;
	inSettingsNode.properties.append(inProperty);

	inProperty.id = LAT_KTR_PROPERTY_ID;
	inProperty.name = tr("lat (ktr)");
	inProperty.value = 0;
	inSettingsNode.properties.append(inProperty);

	inProperty.id = LON_KTR_PROPERTY_ID;
	inProperty.name = tr("lon (ktr)");
	inProperty.value = 0;
	inSettingsNode.properties.append(inProperty);

	if (inSettingsNode.object.pid == 0) {
		emit layoutAboutToBeChanged();

		TreeItem* item =  new TreeItem(inSettingsNode.object, m_rootItem);

		foreach (Property property, inSettingsNode.properties) {
			TreeItem* childItem =  new TreeItem(property, item);
			item->appendChild(childItem);
		}

		m_rootItem->appendChild(item);

		emit layoutChanged();
	}

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

		emit layoutAboutToBeChanged();
		m_rootItem->removeChild(childItem);
		emit layoutChanged();
		break;
	}
}

void UavTreeModel::onUavInfoChanged(const UavInfo &uavInfo, const QString& uavRole)
{
	if (uavRole != m_targetRole){
		return;
	}

	QString number;

	if (UavAutopilotSource ==  uavInfo.sourceType) {
		onPropertyChanged(uavInfo, LAT_PROPERTY_ID, tr("lat"), number.sprintf( "%.4f", uavInfo.lat ));
		onPropertyChanged(uavInfo, LON_PROPERTY_ID, tr("lon"), number.sprintf( "%.4f", uavInfo.lon ));
		onPropertyChanged(uavInfo, ALT_PROPERTY_ID, tr("alt"), QString::number( (int) uavInfo.alt ));
	} else {
		onPropertyChanged(uavInfo, LAT_KTR_PROPERTY_ID, tr("lat (ktr)"), number.sprintf( "%.4f", uavInfo.lat ));
		onPropertyChanged(uavInfo, LON_KTR_PROPERTY_ID, tr("lon (ktr)"), number.sprintf( "%.4f", uavInfo.lon ));
	}

	if(m_isNeedRedraw) {
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

	if (uavInfo.sourceType == UavAutopilotSource) {
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

		BaseItem item = stationItem->data();
		item.state = (isInFlight ? 1 : 0);
		stationItem->updateData(item);

		for (int j = 0; j< stationItem->childCount(); j++) {
			if (stationItem->child(j)->data().id == inProperty.id) {
				stationItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}
}
