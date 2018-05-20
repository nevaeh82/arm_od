#include "TreeItem.h"

TreeItem::TreeItem(const BaseItem &itemData, TreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = itemData;
}

TreeItem::TreeItem(TreeItem *parent)
{
	m_parentItem = parent;
}

TreeItem::~TreeItem()
{
	qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
	m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
	return m_childItems.value(row);
}

int TreeItem::childCount() const
{
	return m_childItems.count();
}

BaseItem TreeItem::data() const
{
	return m_itemData;
}

TreeItem *TreeItem::parent()
{
	return m_parentItem;
}

int TreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

bool TreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > m_childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete m_childItems.takeAt(position);

	return true;
}

bool TreeItem::removeChild(TreeItem* item)
{
	m_childItems.removeAll(item);

	return true;
}

void TreeItem::updateData(const BaseItem& itemData)
{
	m_itemData = itemData;
}
