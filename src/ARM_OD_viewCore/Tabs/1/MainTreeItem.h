#ifndef MAINTREEITEM_H
#define MAINTREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

#include <QStringList>

#include "../Interfaces/core/IExchangeMainTreeItem.h"

///tree item implimentation
class MainTreeItem
{
public:
     MainTreeItem(const QVector<QVariant> &data, MainTreeItem *parent = 0/*, IExchangeMainTreeItem *exchanger = 0*/);
     ~MainTreeItem();

//     void setExchanger(IExchangeMainTreeItem *exchanger);

     MainTreeItem *child(int number);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     bool insertChildren(int position, int count, int columns);
     bool insertColumns(int position, int columns);
     MainTreeItem *parent();
     bool removeChildren(int position, int count);
     bool removeColumns(int position, int columns);
     int childNumber() const;
     bool setData(int column, const QVariant &value);
     void appendChild(MainTreeItem *child);

     bool isChecked() const;
     void setChecked(bool set);

     void setEditable(int column, bool state);
     bool isEditable(int column);

     void setCheckable(int column, bool state);
     bool isCheckable(int column);

     void set_id(unsigned int id);

 private:
     QList<MainTreeItem*> _childItems;
     QVector<QVariant> _itemData;
     MainTreeItem *_parentItem;



     bool   _isEditable[2];
     bool   _isCheckable[2];

     bool _checked;
     //bool _isEditable;

     unsigned int _id;


public:
//     IExchangeMainTreeItem *_exchanger;
};

#endif // MAINTREEITEM_H
