#include "UavTreeModel.h"

#include <TreeModel/TreeItem.h>
#include <Settings/SettingsNode.h>

#include "../DbBla/Defines.h"

UavTreeModel::UavTreeModel(const QStringList &headers, QObject *parent) :
	TreeModelBase(headers, parent)
{
}

UavTreeModel::~UavTreeModel()
{

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

void UavTreeModel::onUavAdded(const Uav &uav)
{
	SettingsNode inSettingsNode;
	inSettingsNode.object.id = uav.uavId;
	inSettingsNode.object.name = uav.name;
	inSettingsNode.object.isEditable = false;
	inSettingsNode.object.pid = 0;
	inSettingsNode.object.state = 0;

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

void UavTreeModel::onUavRemoved(const Uav &uav)
{
	Q_UNUSED(uav);
}

void UavTreeModel::onUavInfoChanged(const UavInfo &uavInfo)
{
	onPropertyChanged(uavInfo, "lat", uavInfo.lat);
	onPropertyChanged(uavInfo, "lon", uavInfo.lon);
	onPropertyChanged(uavInfo, "alt", uavInfo.alt);

	refreshModel();
}

void UavTreeModel::onPropertyChanged(const UavInfo &uavInfo, const QString &name, const QVariant &value)
{
	Property inProperty;
	inProperty.pid = uavInfo.uavId;
	inProperty.id = uavInfo.id;
	inProperty.isEditable = false;
	inProperty.state = 0;
	inProperty.name = name;
	inProperty.value = value;


	//	if (inProperty.pid == 0) {
	//		return;
	//	}

	for (int i = 0; i< m_rootItem->childCount(); i++) {
		TreeItem* stationItem = m_rootItem->child(i);

		for (int j = 0; j< stationItem->childCount(); j++) {
			if (stationItem->child(j)->data().id == inProperty.id) {
				stationItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}

}
