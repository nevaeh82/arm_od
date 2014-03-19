#ifndef ITCPCLIENTOLD_H
#define ITCPCLIENTOLD_H

#include <QObject>
#include <QByteArray>

#include "ITCPParser.h"

class ITCPClientOld: public QObject
{
public:
	virtual ~ITCPClientOld(){};

    virtual void set_parser(ITCPParser* parser) = 0;

    virtual void set(QByteArray data)           = 0;
//    virtual QByteArray get()                = 0;
    virtual bool get_state()                    = 0;

public slots:
    virtual void slotDestroy()                  = 0;

signals:
    void signalConnected(bool state);
};

#endif // ITCPCLIENTOLD_H
