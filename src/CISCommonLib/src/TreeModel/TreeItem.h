#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

#include "Settings/BaseItem.h"

class TreeItem
{
public:
	TreeItem(const BaseItem& itemData, TreeItem *parent = 0);
	TreeItem(TreeItem *parent = 0);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	BaseItem data() const;
	int row() const;
	TreeItem *parent();

	bool removeChildren(int position, int count);
	bool removeChild(TreeItem* item);

	void updateData(const BaseItem &itemData);

private:
	QList<TreeItem*>            m_childItems;
	BaseItem				    m_itemData;
	TreeItem*                   m_parentItem;
};

#endif // TREEITEM_H
