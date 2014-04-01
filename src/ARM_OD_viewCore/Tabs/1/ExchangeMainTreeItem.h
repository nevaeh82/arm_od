#ifndef EXCHANGEMAINTREEITEM_H
#define EXCHANGEMAINTREEITEM_H

#include "../Interfaces/core/IExchangeMainTreeItem.h"
#include "../Interfaces/ITCPClient.h"
#include "../TCP/TCPPacketStruct.h"
#include "../Common/CRCs.h"

#include "../Interfaces/core/ITCPController.h"
#include "../Interfaces/core/IRouter.h"
#include <QObject>
#include <QMap>
#include "TabsProperty.h"

class ExchangeMainTreeItem : public QObject, public IExchangeMainTreeItem
{
    Q_OBJECT
public:
    ExchangeMainTreeItem(IRouter *router, QMap<int, TabsProperty *> map_settings);
    ~ExchangeMainTreeItem();

public:
    void push_data(QVariant station_name, QVariant name, QVariant value);
    QVariant get_data();

    void set_id(unsigned int id);

private:
    IRouter  *_router;
    QMap<int, TabsProperty *> _map_settings;
    unsigned int _id;

private:
    QByteArray _encode(unsigned int type, QByteArray *data);

signals:
    void signalWriteData(QByteArray);
};

#endif // EXCHANGEMAINTREEITEM_H
