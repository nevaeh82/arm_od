#include "TreeModel.h"

#include <QSize>

#include "TreeItem.h"

TreeModel::TreeModel(IDbManager *dbManager, const QStringList &headers, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_dbManager = dbManager;

	m_headersList= headers;

	m_rootItem = new TreeItem(NULL);
}

TreeModel::~TreeModel()
{
	if (m_rootItem != NULL) {
		delete m_rootItem;
		m_rootItem = NULL;
	}
}


int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
	return m_headersList.count();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	TreeItem* item =  static_cast<TreeItem *>(index.internalPointer());

	if (!item) {
		return QVariant();
	}

	if(role == Qt::DisplayRole) {
		switch(index.column())
		{
			case 0:
			{
				return getTranslateItemNameFromReal(item->data().name);
			}
			case 1:
			{
				return item->data().value;
			}
		}
	}
	else if (role == Qt::SizeHintRole) {
		return QSize(150, 18);
	}
	else if (role == Qt::CheckStateRole) {
		return QVariant();
	}

	else if (role == Qt::EditRole) {

		if(index.column() == 0){
			return item->data().name;
		}
		else if(index.column() == 1){
			return item->data().value;
		}
		else {
			return QVariant();
		}
	}

	return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
	///only 1 column will be editable
	if((index.column() != 0 )&&(item->data().isEditable))
	{
		flags |= Qt::ItemIsEditable;
	}

	return flags;
}


QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (! (orientation == Qt::Horizontal && role == Qt::DisplayRole)) {
		return QVariant();
	}

	if (section > m_headersList.count()) {
		return QVariant();
	}

	return m_headersList.at(section);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem = NULL;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	if (!parentItem)
		return QModelIndex();

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());

	if (!childItem)
		return QModelIndex();

	TreeItem *parentItem = childItem->parent();

	if (parentItem == m_rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);

}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.isValid()) {
		parentItem = static_cast<TreeItem *>(parent.internalPointer());
	}
	else {
		parentItem = m_rootItem;
	}

	if(parentItem == 0) {
		return 0;
	}

	return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
						int role)
{
	if (role != Qt::EditRole)
		return false;

	if (!value.isValid()){
		return false;
	}

	TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
	BaseItem data = item->data();
	data.value = value;

	item->updateData(data);

	m_dbManager->updateProperty(Property(item->data()));

	emit dataChanged(index, index);

	return true;
}


void TreeModel::refreshModel()
{
	int index = m_rootItem->childCount();

	for (int i = 0; i < m_rootItem->childCount(); i++){
		index += m_rootItem->child(i)->childCount();

	}

	emit dataChanged(createIndex(0,0), createIndex(index,columnCount()-1));
}


void TreeModel::onSettingsNodeChanged(const SettingsNode& settingsNode)
{
	SettingsNode inSettingsNode = settingsNode;

	if (settingsNode.object.pid == 0) {
		emit layoutAboutToBeChanged();

		TreeItem* item =  new TreeItem(inSettingsNode.object, m_rootItem);

		foreach (Property property, settingsNode.properties) {

			TreeItem* childItem =  new TreeItem(property, item);
			item->appendChild(childItem);
		}

		m_rootItem->appendChild(item);

		emit layoutChanged();
	}

	emit onItemAddedSignal();
}

void TreeModel::onPropertyChanged(const Property &property)
{
	Property inProperty = property;

	if (inProperty.pid == 0) {
		return;
	}

	for (int i = 0; i< m_rootItem->childCount(); i++) {
		TreeItem* stationItem = m_rootItem->child(i);

		for (int j = 0; j< stationItem->childCount(); j++) {
			if (stationItem->child(j)->data().id == inProperty.id) {
				stationItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}

	refreshModel();
}

void TreeModel::onCleanSettings()
{
	if (m_rootItem)
		delete m_rootItem;

	m_rootItem = new TreeItem(NULL);
	reset();
}

void TreeModel::setStationsList(const QStringList &stationsList)
{
	foreach (QString stationName, stationsList) {
		SettingsNode settingsNode = m_dbManager->getSettingsNode(stationName);
		onSettingsNodeChanged(settingsNode);
	}
}

QString TreeModel::getTranslateItemNameFromReal(const QString& itemName) const
{
	if (itemName == "Frequency") {
		return tr("Frequency");
	} else if (itemName == "Selected") {
		return tr("Selected");
	} else if (itemName == "Center") {
		return tr("Center");
	} else if (itemName == "Start") {
		return tr("Start");
	} else if (itemName == "Stop") {
		return tr("Stop");
	} else if (itemName == "Averaging") {
		return tr("Averaging");
	} else if (itemName == "Leading OP") {
		return tr("Leading OP");
	} else if (itemName == "Panorama start") {
		return tr("Panorama start");
	} else if (itemName == "Panorama end") {
		return tr("Panorama end");
	}

	return itemName;
}

