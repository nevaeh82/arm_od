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

void UavTreeModel::onBlaAdded(const Bla &bla)
{
	SettingsNode inSettingsNode;
	inSettingsNode.object.id = bla.blaId;
	inSettingsNode.object.name = bla.name;
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

void UavTreeModel::onBlaRemoved(const Bla &bla)
{
	Q_UNUSED(bla);
}

void UavTreeModel::onBlaInfoChanged(const BlaInfo &blaInfo)
{
	onPropertyChanged(blaInfo, "lat", blaInfo.lat);
	onPropertyChanged(blaInfo, "lon", blaInfo.lon);
	onPropertyChanged(blaInfo, "alt", blaInfo.alt);

	refreshModel();
}

void UavTreeModel::onPropertyChanged(const BlaInfo &blaInfo, const QString &name, const QVariant &value)
{
	Property inProperty;
	inProperty.pid = blaInfo.blaId;
	inProperty.id = blaInfo.id;
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
