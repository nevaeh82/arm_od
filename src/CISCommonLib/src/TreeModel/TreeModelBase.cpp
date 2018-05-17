#include "TreeModelBase.h"

#include <QSize>

#include "TreeItem.h"

TreeModelBase::TreeModelBase(const QStringList &headers, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_headersList= headers;

	m_rootItem = new TreeItem(NULL);
}

TreeModelBase::~TreeModelBase()
{
	if (m_rootItem != NULL) {
		delete m_rootItem;
		m_rootItem = NULL;
	}
}


int TreeModelBase::columnCount(const QModelIndex & /* parent */) const
{
	return m_headersList.count();
}

QVariant TreeModelBase::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags TreeModelBase::flags(const QModelIndex &index) const
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


QVariant TreeModelBase::headerData(int section, Qt::Orientation orientation,
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

QModelIndex TreeModelBase::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex TreeModelBase::parent(const QModelIndex &index) const
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

bool TreeModelBase::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());

	beginRemoveRows(parent, position, position + rows - 1);
	bool success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeModelBase::rowCount(const QModelIndex &parent) const
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

bool TreeModelBase::updateModelData(TreeItem *item)
{
	// should be implemented in inhereted class
	Q_UNUSED(item);

	return false;
}

bool TreeModelBase::setData(const QModelIndex &index, const QVariant &value,
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

	if (!updateModelData(item)) {
		return false;
	}

	emit dataChanged(index, index);

	return true;
}


void TreeModelBase::refreshModel()
{
	int index = m_rootItem->childCount();

	for (int i = 0; i < m_rootItem->childCount(); i++){
		index += m_rootItem->child(i)->childCount();

	}

	emit dataChanged(createIndex(0,0), createIndex(index,columnCount()-1));
}


QString TreeModelBase::getTranslateItemNameFromReal(const QString& itemName) const
{
	return itemName;
}

