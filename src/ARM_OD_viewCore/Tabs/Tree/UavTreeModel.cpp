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

void UavTreeModel::onBlaAdded(const Bla &bla)
{
	SettingsNode inSettingsNode;
	inSettingsNode.object.id = bla.blaId;
	inSettingsNode.object.name = bla.name;
	inSettingsNode.object.isEditable = false;
	inSettingsNode.object.pid = 0;
	inSettingsNode.object.state = 0;

	Property inProperty;
	inProperty.pid = bla.blaId;
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

void UavTreeModel::onBlaRemoved(const Bla &bla)
{
	Q_UNUSED(bla);
}

void UavTreeModel::onBlaInfoChanged(const BlaInfo &blaInfo)
{
	onPropertyChanged(blaInfo, 1, tr("lat"), QString::number(blaInfo.lat, 'g', 6));
	onPropertyChanged(blaInfo, 2, tr("lon"), QString::number(blaInfo.lon, 'g', 6));
	onPropertyChanged(blaInfo, 3, tr("alt"), QString::number(blaInfo.alt, 'g', 6));

	refreshModel();
}

void UavTreeModel::onPropertyChanged(const BlaInfo &blaInfo, const uint propId, const QString &name, const QVariant &value)
{
	Property inProperty;
	inProperty.pid = blaInfo.blaId;
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
