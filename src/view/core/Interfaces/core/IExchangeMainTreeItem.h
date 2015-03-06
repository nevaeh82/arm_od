#ifndef IEXCHANGEMAINTREEITEM_H
#define IEXCHANGEMAINTREEITEM_H

#include <QVariant>

class IExchangeMainTreeItem
{
public:
    virtual ~IExchangeMainTreeItem(){};

    virtual void push_data(QVariant station_name, QVariant name, QVariant value)  = 0;
    virtual QVariant get_data()             = 0;
};

#endif // IEXCHANGEMAINTREEITEM_H
