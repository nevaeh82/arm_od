#include "UavTreeModel.h"

#include <TreeModel/TreeItem.h>
#include <Settings/SettingsNode.h>

#include "../DbBla/Defines.h"

#define LAT_PROPERTY_ID		1
#define LON_PROPERTY_ID		2
#define ALT_PROPERTY_ID		3


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
	onPropertyChanged(uavInfo, 1, tr("lat"), QString::number(uavInfo.lat, 'g', 6));
	onPropertyChanged(uavInfo, 2, tr("lon"), QString::number(uavInfo.lon, 'g', 6));
	onPropertyChanged(uavInfo, 3, tr("alt"), QString::number(uavInfo.alt, 'g', 6));

	refreshModel();
}

void UavTreeModel::onPropertyChanged(const UavInfo &uavInfo, const uint propId, const QString &name, const QVariant &value)
{
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

		for (int j = 0; j< stationItem->childCount(); j++) {
			if (stationItem->child(j)->data().id == inProperty.id) {
				stationItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}

}
